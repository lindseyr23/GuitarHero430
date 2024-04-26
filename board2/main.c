#include <msp430.h>
#include <stdint.h>
#include <stdbool.h>
#include "rgb_interface.h"
#include "PlaySong.h"
/**
 * main.c
 */

#define buffer 8

extern unsigned int song1_fortunate_son[];
extern const unsigned int songLength1;//   gets calculated later

unsigned int color_intro[] = {0,11,0,11,0,11,0,11,0};

extern unsigned int song2_mississippi_queen[];
extern const unsigned int songLength2;//   gets calculated later


//note durations for each
const unsigned int introDuration = 10;
const unsigned int song1Duration = 6; //want every 2 notes to  be 133 bpm. beat is the eigth note
const unsigned int song2Duration = 6; //want every 2 notes to  be ~140 bpm. beat is the eigth note

extern const uint8_t green;
extern const uint8_t red;
extern const uint8_t yellow;
extern const uint8_t blue;
extern const uint8_t orange;
//----------------------
volatile unsigned int user_LED_1;
volatile unsigned int user_LED_2;
volatile unsigned int user_LED_3;
volatile unsigned int user_LED_4;
//---------
volatile unsigned int green_next;
volatile unsigned int g_led1;
volatile unsigned int g_led2;
volatile unsigned int g_led3;
volatile unsigned int g_led4;
volatile unsigned int g_led5;
volatile unsigned int g_led6;
volatile unsigned int g_led7;
//----------
volatile unsigned int red_next;
volatile unsigned int r_led1;
volatile unsigned int r_led2;
volatile unsigned int r_led3;
volatile unsigned int r_led4;
volatile unsigned int r_led5;
volatile unsigned int r_led6;
volatile unsigned int r_led7;
//---------
volatile unsigned int yellow_next;
volatile unsigned int y_led1;
volatile unsigned int y_led2;
volatile unsigned int y_led3;
volatile unsigned int y_led4;
volatile unsigned int y_led5;
volatile unsigned int y_led6;
volatile unsigned int y_led7;
//---------
volatile unsigned int blue_next;
volatile unsigned int b_led1;
volatile unsigned int b_led2;
volatile unsigned int b_led3;
volatile unsigned int b_led4;
volatile unsigned int b_led5;
volatile unsigned int b_led6;
volatile unsigned int b_led7;
//--------- no orange ..can add if you have button
volatile unsigned int orange_next;
volatile unsigned int o_led1;
volatile unsigned int o_led2;
volatile unsigned int o_led3;
volatile unsigned int o_led4;
volatile unsigned int o_led5;
volatile unsigned int o_led6;
volatile unsigned int o_led7;
//----------------------

volatile unsigned int new_g;
volatile unsigned int new_r;
volatile unsigned int new_y;
volatile unsigned int new_b;
volatile unsigned int new_o;
volatile unsigned int curr_note;

unsigned int button_press_detected;
unsigned int strummer;
volatile unsigned int counter = 0;


unsigned int current_note;
unsigned int songLength;
//keeps track of notes in array
unsigned int *note_position;

enum state_enum {Intro, Game, Lost, Win} state; // enum to describe state of system

int next = 0;
extern int ready_for_next; //flag to be ready for next
int note_change; // flag that note change occured
int shift_leds_flag;
int play_song1;
int play_song2;
int stop_song;
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
    TA0CTL |= TACLR;
    TA0CCTL0 = CCIE; //interrupt enable Timer 0
    TA0CCR0 = 4000;    //set up Timer 0
    TA0CTL |= TASSEL_1 + MC_1 + ID_0;
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
    if (state == Game & current_note >= songLength-1){
        StopSong();
        play_song1 = 0;
        play_song2 = 0;
//        songLength = sizeof((color_intro)) / sizeof((color_intro)[0]);
//        PlaySound(color_intro, songLength);
        WinEffect();
        state = Win;
    }
    else if ((current_note < songLength-1) & ready_for_next){
        //move to new note
        if(state == Intro){
            curr_note = color_intro[current_note];
        }
        else if(play_song1 == 1){
            curr_note = song1_fortunate_son[current_note];
        }
        else if (play_song2 == 1){
            curr_note = song2_mississippi_queen[current_note];
        }
        note_position = 1 + note_position;

        switch(curr_note) { //checks if note is correct
            case 0: //off
                new_g = 0;
                new_r = 0;
                new_y = 0;
                new_b = 0;
                new_o = 0;
                break;
            case 1: //green
                new_g = 1;
                new_r = 0;
                new_y = 0;
                new_b = 0;
                new_o = 0;
                break;
            case 2://red
                new_g = 0;
                new_r = 1;
                new_y = 0;
                new_b = 0;
                new_o = 0;
                break;
            case 3: //yellow
                new_g = 0;
                new_r = 0;
                new_y = 1;
                new_b = 0;
                new_o = 0;
                break;
            case 4: //blue
                new_g = 0;
                new_r = 0;
                new_y = 0;
                new_b = 1;
                new_o = 0;
                break;
            case 5: //green/red
                new_g = 1;
                new_r = 1;
                new_y = 0;
                new_b = 0;
                new_o = 0;
                break;
            case 6: //green/yellow
                new_g = 1;
                new_r = 0;
                new_y = 1;
                new_b = 0;
                new_o = 0;
                break;
            case 7: //green/blue
                new_g = 1;
                new_r = 0;
                new_y = 0;
                new_b = 1;
                new_o = 0;
                break;
            case 8: //red/yellow
                new_g = 0;
                new_r = 1;
                new_y = 1;
                new_b = 0;
                new_o = 0;
                break;
            case 9: //red/blue
                new_g = 0;
                new_r = 1;
                new_y = 0;
                new_b = 1;
                new_o = 0;
                break;
            case 10:// yellow/blue
                new_g = 0;
                new_r = 0;
                new_y = 1;
                new_b = 1;
                new_o = 0;
                break;
            case 11:// yellow/blue
                new_g = 1;
                new_r = 1;
                new_y = 1;
                new_b = 1;
                new_o = 1;
                break;
        }//end of case

        g_led1 = g_led2;
        g_led2 = g_led3;
        g_led3 = g_led4;
        g_led4 = g_led5;
        g_led5 = g_led6;
        g_led6 = g_led7;
        g_led7 = new_g;
//        g_led7 = *g_note_position

        r_led1 = r_led2;
        r_led2 = r_led3;
        r_led3 = r_led4;
        r_led4 = r_led5;
        r_led5 = r_led6;
        r_led6 = r_led7;
        r_led7 = new_r;
//        r_led7 = *r_note_position

        y_led1 = y_led2;
        y_led2 = y_led3;
        y_led3 = y_led4;
        y_led4 = y_led5;
        y_led5 = y_led6;
        y_led6 = y_led7;
        y_led7 = new_y;
//        y_led7 = *y_note_position;

        b_led1 = b_led2;
        b_led2 = b_led3;
        b_led3 = b_led4;
        b_led4 = b_led5;
        b_led5 = b_led6;
        b_led6 = b_led7;
        b_led7 = new_b;
//        b_led7 = *b_note_position;

        o_led1 = o_led2;
        o_led2 = o_led3;
        o_led3 = o_led4;
        o_led4 = o_led5;
        o_led5 = o_led6;
        o_led6 = o_led7;
        o_led7 = new_o;
//        o_led7 = *o_note_position;

        current_note++; //move to next note of song
    } else{
        // COULD HAVE END SONG ANIMATION? instead of looping
        //start at beginning of song
        note_position = note_position - current_note;
//            g_note_position = g_note_position - current_note;
//            r_note_position = r_note_position - current_note;
//            y_note_position = y_note_position - current_note;
//            b_note_position = b_note_position - current_note;
//            o_note_position = o_note_position - current_note;

//            duration_position = duration_position - current_note;
            current_note = 0; // Reset current_note to start at the first note


    }

}

void init_board_communication() { // 2.7 (accept_input) input, 2.1 (play_song1) output, 1.3 (play_song2) output

    // Set up the input pins
    P2DIR &= ~BIT1; // Set pin 2.1 to input to receive play_song1
    P1DIR &= ~BIT3; // Set pin 1.3 to input to receive play_song2


    // listen for low to high transition to start the songs
    P2IES &= ~BIT1;
    P1IES &= ~BIT3;



    // enable interrupts for these pins
    P2IE |= BIT1;
    P1IE |= BIT3;

    P2REN |= BIT1;
    P2OUT |= BIT1;


    // make sure it starts at low
    P2IN &= ~BIT1;
    P1IN &= ~BIT3;

    // clear any pending interrupts
    P2IFG &= ~BIT1;
    P1IFG &= ~BIT3;

    // Set up the output pin
//    P2SEL &= ~BIT7; // set pin 2.7 to a GPIO pin
//    P2DIR |= BIT7; // Set pin 2.7 to output to communicate accept_input
//    P2OUT &= ~BIT7; // set pin2.7 to 0
//    P2IFG &= ~BIT7;

//    P1SEL &= ~BIT1;
//    P1SEL2 &- ~BIT1;

    P2DIR |= BIT5;
    P2OUT &= ~BIT5;
    P2IE &= ~BIT5;
//    P2IFG &= ~BIT5;
//    P1REN |= BIT1;

}

int main(void)
{
    play_song1 = 0;
    play_song2 = 0;
    stop_song = 0;
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    rgb_init_spi();
    init_buttons();

    init_board_communication();
    init_timerA();

    state = Intro;
    shift_leds_flag = 0;

    _enable_interrupts();

    songLength = sizeof(color_intro) / sizeof(color_intro[0]);
    PlaySound(color_intro, songLength);

    MusicInit();

    while (1){
        if (state == Win){
            win_screen();
            if (stop_song == 1){//stop song sent by board_1
                    StopSong();
                    play_song1 = 0;
                    play_song2 = 0;
                    songLength = sizeof((color_intro)) / sizeof((color_intro)[0]);
                    PlaySound(color_intro, songLength);
                    state = Intro;
                    stop_song = 0;
                }
        }
        if (state == Lost){
            loss_screen();
            if (stop_song == 1){//stop song sent by board_1
                    StopSong();
                    play_song1 = 0;
                    play_song2 = 0;
                    songLength = sizeof((color_intro)) / sizeof((color_intro)[0]);
                    PlaySound(color_intro, songLength);
                    state = Intro;
                    stop_song = 0;
                }
        }
        if (state == Game){
            if (stop_song == 1){
                StopSong();
                play_song1 = 0;
                play_song2 = 0;
                LossEffect();
                state = Lost;
                stop_song = 0;
            }
            next_note(green_next, red_next, yellow_next, blue_next, orange_next);
            if(shift_leds_flag){
                shift_leds();
                shift_leds_flag = 0;
            }
        }
        if (state == Intro){
            next_note(green_next, red_next, yellow_next, blue_next, orange_next);
            if(shift_leds_flag){
                shift_leds();
                shift_leds_flag = 0;
            }
            if (play_song1 == 1){
                state = Game;
                PlaySong(1);
                counter = 0;
                current_note = 0;

                songLength = songLength1;
                PlaySound(song1_fortunate_son, songLength);
//                PlaySound(green_colors_song1, red_colors_song1, yellow_colors_song1, blue_colors_song1, orange_colors_song1, songDuration_song1, songLength);
            }
            else if (play_song2 == 1){
                state = Game;
                PlaySong(2);
                counter = 0;
                current_note = 0;

                songLength = songLength2;
                PlaySound(song2_mississippi_queen, songLength);
//                songLength = sizeof(green_colors_song2) / sizeof(green_colors_song2[0]);
//                PlaySound(green_colors_song2, red_colors_song2, yellow_colors_song2, blue_colors_song2, orange_colors_song2, songDuration_song2, songLength);
            }
        }//end of intro state
    }//end of while

} //end of main

//manages note duration
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A(void)
{
    if (state == Game | state == Intro)
    {
        if (counter == 3){
            P2OUT &= ~BIT5;
//            P2OUT &= ~BIT7; // stop message increment_counter
        }
        counter ++;
        int curr_durr;
        if(play_song1){
            curr_durr = song1Duration;
//            curr_durr = songDuration_song1[current_note];
        }
        else if (play_song2){
            curr_durr = song2Duration;
//            curr_durr = songDuration_song2[current_note];
        }
        else{
            curr_durr = introDuration;
//            curr_durr = songDuration_intro[current_note];
        }
        if (counter > curr_durr){
            shift_leds_flag = 1;
            P2OUT |= BIT5;
//            P2OUT |= BIT7; // send out message increment_counter
            counter = 0;
        }
    }//end of game state
}//end of A0 ISR

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
                stop_song = 1;
                P2IES &= ~BIT1; // change edge to rising (button release)
            } else {// rising edge detected - turning sound on
                play_song1 = 1;
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
                stop_song = 1;
                P1IES &= ~BIT3; // change edge to rising (button release)
            } else {// rising edge detected - turning sound on
                play_song2 = 1;
                P1IES |= BIT3;
            }
            P1IFG &= ~BIT3; // clear interrupt flag
        }

    __bic_SR_register_on_exit(LPM3_bits); // exit LPM3 when returning to program (clear LPM3 bits)
}
