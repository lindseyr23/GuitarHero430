#include <msp430.h>
#include <stdint.h>
#include <stdbool.h>
#include "rgb_interface.h"
#include "PlaySong.h"
/**
 * main.c
 */

#define Q 10
#define buffer 2



//1 is on, 0 is off
int green_colors[] = {0,1,0,0,0,0,0,0,0};
int red_colors[]   = {0,0,1,0,0,0,0,0,1};
int yellow_colors[]= {0,0,0,1,0,0,0,1,0};
int blue_colors[]  = {0,0,0,0,1,0,1,0,0};
int orange_colors[]= {0,0,0,0,0,1,0,0,0};
int songDuration[] = {Q,Q,Q,Q,Q,Q,Q,Q,Q};


int green_colors_song1[] = {1,1,1,0,0,0,0,0,0};
int red_colors_song1[]   = {0,0,0,1,1,1,0,0,1};
int yellow_colors_song1[]= {0,0,0,0,0,0,0,1,0};
int blue_colors_song1[]  = {0,0,0,0,0,0,0,0,0};
int orange_colors_song1[]= {0,0,0,0,0,0,0,0,0};
int songDuration_song1[] = {Q,Q,Q,Q,Q,Q,Q,Q,Q};

extern const uint8_t green;
extern const uint8_t red;
extern const uint8_t yellow;
extern const uint8_t blue;
extern const uint8_t orange;

int songLength = sizeof(green_colors) / sizeof(green_colors[0]);

//----------------------
int user_LED_1;
int user_LED_2;
int user_LED_3;
int user_LED_4;
//---------
int green_next;
int g_led1;
int g_led2;
int g_led3;
int g_led4;
int g_led5;
int g_led6;
int g_led7;
//--------=0-
int red_next;
int r_led1;
int r_led2;
int r_led3;
int r_led4;
int r_led5;
int r_led6;
int r_led7;
//---------
int yellow_next;
int y_led1;
int y_led2;
int y_led3;
int y_led4;
int y_led5;
int y_led6;
int y_led7;
//---------
int blue_next;
int b_led1;
int b_led2;
int b_led3;
int b_led4;
int b_led5;
int b_led6;
int b_led7;
//---------
int orange_next;
int o_led1;
int o_led2;
int o_led3;
int o_led4;
int o_led5;
int o_led6;
int o_led7;
//-----------

int button_press_detected;
int strummer;
int counter = 0;


int current_note;
int songLength;
//keeps track of notes in array
int *g_note_position;
int *r_note_position;
int *y_note_position;
int *b_note_position;
int *o_note_position;
int *duration_position;

enum state_enum {Intro, Game, Lost, Win} state; // enum to describe state of system

int next = 0;
extern int ready_for_next; //flag to be ready for next
int note_change; // flag that note change occured
int shift_leds_flag;
int play_song1;
int play_song2;
void next_note(int green_next, int red_next, int yellow_next, int blue_next, int orange_next)
{
    rgb_send_start();
    set_green(g_led1, g_led2, g_led3, g_led4, g_led5, g_led6, g_led7);
    set_red(r_led1, r_led2, r_led3, r_led4, r_led5, r_led6, r_led7);
    set_yellow(y_led1, y_led2, y_led3, y_led4, y_led5, y_led6, y_led7);
    set_blue(b_led1, b_led2, b_led3, b_led4, b_led5, b_led6, b_led7);
    set_orange(o_led1, o_led2, o_led3, o_led4, o_led5, o_led6, o_led7);
    rgb_send_end();
}
//----------------------

void init_timerA(void)
{
    TA0CCTL0 = CCIE; //interrupt enable Timer 0
    TA0CCR0 = 4000;    //set up Timer 0
    TA0CTL = TASSEL_1 + MC_1 + ID_0;
}

void init_buttons() {
    P2DIR &= ~(BIT0 + BIT2 + BIT3 + BIT4 + BIT5); // set to input
    P2REN |= BIT0 + BIT2 + BIT3 + BIT4 + BIT5; // enable pullup/down resistors
    P2OUT |= BIT0 + BIT2 + BIT3 + BIT4 + BIT5; // set resistors to pull up

    P2IES |= BIT0 + BIT2 + BIT3 + BIT4 + BIT5; // listen for high to low transitions
    P2IFG &=  ~(BIT0 + BIT2 + BIT3 + BIT4 + BIT5); // clear any pending interrupts
    P2IE |= BIT0 + BIT2 + BIT3 + BIT4 + BIT5; // enable interrupts for these pins
}

void shift_leds(){
    if ((current_note < songLength-1) & ready_for_next){
        //move to new note
        duration_position = 1 + duration_position;

        g_note_position = 1 + g_note_position;
        r_note_position = 1 + r_note_position;
        y_note_position = 1 + y_note_position;
        b_note_position = 1 + b_note_position;
        o_note_position = 1 + o_note_position;

        g_led1 = g_led2;
        g_led2 = g_led3;
        g_led3 = g_led4;
        g_led4 = g_led5;
        g_led5 = g_led6;
        g_led6 = g_led7;
        g_led7 = *g_note_position;


        r_led1 = r_led2;
        r_led2 = r_led3;
        r_led3 = r_led4;
        r_led4 = r_led5;
        r_led5 = r_led6;
        r_led6 = r_led7;
        r_led7 = *r_note_position;

        y_led1 = y_led2;
        y_led2 = y_led3;
        y_led3 = y_led4;
        y_led4 = y_led5;
        y_led5 = y_led6;
        y_led6 = y_led7;
        y_led7 = *y_note_position;

        b_led1 = b_led2;
        b_led2 = b_led3;
        b_led3 = b_led4;
        b_led4 = b_led5;
        b_led5 = b_led6;
        b_led6 = b_led7;
        b_led7 = *b_note_position;

        o_led1 = o_led2;
        o_led2 = o_led3;
        o_led3 = o_led4;
        o_led4 = o_led5;
        o_led5 = o_led6;
        o_led6 = o_led7;
        o_led7 = *o_note_position;

        current_note++; //move to next note of song
    } else{
        // COULD HAVE END SONG ANIMATION? instead of looping
        //start at beginning of song
        g_note_position = g_note_position - current_note;
        r_note_position = r_note_position - current_note;
        y_note_position = y_note_position - current_note;
        b_note_position = b_note_position - current_note;
        o_note_position = o_note_position - current_note;

        duration_position = duration_position - current_note;
        current_note = 0; // Reset current_note to start at the first note
    }

}

int main(void)
{
    play_song1 = 0;
    play_song2 = 0;
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    rgb_init_spi();
    init_buttons();
    init_timerA();

    state = Intro;
    shift_leds_flag = 0;

    _enable_interrupts();

    PlaySound(green_colors, red_colors, yellow_colors, blue_colors, orange_colors, songDuration, songLength);

    // Set pin 2.7 to output to communicate accept_input
    P2DIR |= BIT7;
    // Set pin 2.1 to input to receive play_song1
    P2DIR &= ~BIT1;
    // Set pin 1.3 to input to receive play_song2
    P1DIR &= ~BIT3;

    MusicInit();
        //ways to change difficulty could be no need to change song array maybe?
        // if easy: next_note(green_next, red_next, yellow_next, 0, 0);
        // else if hard: next_note(green_next, red_next, yellow_next, blue_next, orange_next);

    while (1){
        if (state == Game){
            next_note(green_next, red_next, yellow_next, blue_next, orange_next);
            if(shift_leds_flag){
                shift_leds();
                shift_leds_flag = 0;
            }
        }
        else if (state == Intro){
            if (play_song1 == 1){
                state == Game;
                // have code to assign the LEDs to the correct values here
            }
            else if (play_song2 == 1){
                state == Game;
                // have code to assign the LEDs to the correct values here
            }
        }
    }

}

//manages note duration
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A(void)
{
    if (state == Game)
    {
        counter ++;
        if (counter == songDuration[current_note] - buffer){
            P2OUT |= BIT7; // send out message accept_input
        }
        if (counter == buffer){
            P2OUT &= ~BIT7; // stop message accept_input
        }
        if (counter > songDuration[current_note]){
            shift_leds_flag = 1;
            counter = 0;
        }
    }
}

// ISR for the inputs
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=PORT2_VECTOR
__interrupt void playsong1(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(PORT2_VECTOR))) button (void)
#else
#error Compiler not supported!
#endif
{
        if(P2IFG & BIT1)//checks port 2.1 (playsong1)
        {
            if(P2IES & BIT1) { // falling edge detected - turning sound off
                play_song1 = 0;
                StopSong();
                P2IES &= ~BIT1; // change edge to rising (button release)
            } else {// rising edge detected - turning sound on
                play_song1 = 1;
                PlaySong(1);
                P2IES |= BIT1;
            }
            P2IFG &= ~BIT1; // clear interrupt flag
        }

    __bic_SR_register_on_exit(LPM3_bits); // exit LPM3 when returning to program (clear LPM3 bits)
}


#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=PORT1_VECTOR
__interrupt void playsong2(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(PORT1_VECTOR))) button (void)
#else
#error Compiler not supported!
#endif
{
        if(P1IFG & BIT3)//checks port 1.3 (play song2)
        {
            if(P1IES & BIT3){
                play_song2 = 0;
                StopSong();
                P1IES &= ~BIT3; // change edge to rising (button release)
            } else {// rising edge detected - turning sound on
                play_song2 = 1;
                PlaySong(2);
                P1IES |= BIT3;
            }
            P1IFG &= ~BIT3; // clear interrupt flag
        }

    __bic_SR_register_on_exit(LPM3_bits); // exit LPM3 when returning to program (clear LPM3 bits)
}
