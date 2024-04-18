#include <msp430.h>
#include <stdint.h>
#include <stdbool.h>
#include "rgb_interface.h"
/**
 * main.c
 */

//----when using WDTCTL = WDT_ADLY_250;
    //#define Q 4000  //quarter note duration ... too slow skips notes
    //#define Q 8000  // H Half note ... too slow weird buffering
    //#define Q 12000  // so close yet so far
//    #define Q 16000
//----when usingWDTCTL = WDT_ADLY_16;
#define Q 4000  // colors shift well, just show them all at the same time instead of one on one. watchdog too fast?



//1 is on, 0 is off
int green_colors[] = {0,1,0,0,0,0,0,0,0};
int red_colors[]   = {0,0,1,0,0,0,0,0,1};
int yellow_colors[]= {0,0,0,1,0,0,0,1,0};
int blue_colors[]  = {0,0,0,0,1,0,1,0,0};
int orange_colors[]= {0,0,0,0,0,1,0,0,0};
int songDuration[] = {Q,Q,Q,Q,Q,Q,Q,Q,Q};

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

int debounce_counter = 0;

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

void init_wdt(void)
{
    BCSCTL3 |= LFXT1S_2;     // ACLK = VLO
   WDTCTL = WDT_ADLY_16;   //very fast
//    WDTCTL = WDT_ADLY_250;    // WDT 250ms (~675ms since clk 12khz), ACLK, interval timer
    // WDTCTL = WDT_MDLY_32 ;
    //set up Timer 0
    TA0CCTL0 = CCIE; //interrupt enable Timer 0
    TA0CCR0 = 4000;    //set up Timer 0
    TA0CTL = TASSEL_1 + MC_1 + ID_0;

    IE1 |= WDTIE;            // Enable WDT interrupt
}

//void ripple_notes()   //not working when called into watchdog
//{
//    g_led1 = g_led2;
//    g_led2 = g_led3;
//    g_led3 = g_led4;
//    g_led4 = g_led5;
//    g_led5 = g_led6;
//    g_led6 = g_led7;
//    g_led7 = green_next;
//
//    r_led1 = r_led2;
//    r_led2 = r_led3;
//    r_led3 = r_led4;
//    r_led4 = r_led5;
//    r_led5 = r_led6;
//    r_led6 = r_led7;
//    r_led7 = red_next;
//
//    y_led1 = y_led2;
//    y_led2 = y_led3;
//    y_led3 = y_led4;
//    y_led4 = y_led5;
//    y_led5 = y_led6;
//    y_led6 = y_led7;
//    y_led7 = yellow_next;
//
//    b_led1 = b_led2;
//    b_led2 = b_led3;
//    b_led3 = b_led4;
//    b_led4 = b_led5;
//    b_led5 = b_led6;
//    b_led6 = b_led7;
//    b_led7 = blue_next;
//
//    o_led1 = o_led2;
//    o_led2 = o_led3;
//    o_led3 = o_led4;
//    o_led4 = o_led5;
//    o_led5 = o_led6;
//    o_led6 = o_led7;
//    o_led7 = orange_next;
//}
void init_buttons() {
    P2DIR &= ~(BIT0 + BIT2 + BIT3 + BIT4 + BIT5); // set to input
    P2REN |= BIT0 + BIT2 + BIT3 + BIT4 + BIT5; // enable pullup/down resistors
    P2OUT |= BIT0 + BIT2 + BIT3 + BIT4 + BIT5; // set resistors to pull up

    P2IES |= BIT0 + BIT2 + BIT3 + BIT4 + BIT5; // listen for high to low transitions
    P2IFG &=  ~(BIT0 + BIT2 + BIT3 + BIT4 + BIT5); // clear any pending interrupts
    P2IE |= BIT0 + BIT2 + BIT3 + BIT4 + BIT5; // enable interrupts for these pins
}

int next = 0;
extern int ready_for_next; //flag to be ready for next
int note_change; // flag that note change occured

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    init_wdt();
    rgb_init_spi();
    init_buttons();

    state = Game;

    _enable_interrupts();

    PlaySound(green_colors, red_colors, yellow_colors, blue_colors, orange_colors, songDuration, songLength);

        //ways to change difficulty could be no need to change song array maybe?
        // if easy: next_note(green_next, red_next, yellow_next, 0, 0);
        // else if hard: next_note(green_next, red_next, yellow_next, blue_next, orange_next);

    while (1){
        next_note(green_next, red_next, yellow_next, blue_next, orange_next);
    }

}

//manages note duration
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A(void)
{
    if (state == Game)
    {
        if ((current_note < songLength) & ready_for_next)
        {
        //move to new note
        duration_position = 1 + duration_position;

        g_note_position = 1 + g_note_position;
        r_note_position = 1 + r_note_position;
        y_note_position = 1 + y_note_position;
        b_note_position = 1 + b_note_position;
        o_note_position = 1 + o_note_position;
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

}

//watchdog timer
// Watchdog Timer interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=WDT_VECTOR
__interrupt void watchdog_timer(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(WDT_VECTOR))) watchdog_timer (void)
#else
#error Compiler not supported!
#endif
{
    if (state == Game)
    {
        if ((current_note < songLength))
         {
           TA1CCR2 = *g_note_position >> 1; //spaces between note. divide by 2
           TA1CCR0 = *g_note_position; //update to new note

           green_next  = *g_note_position;
           red_next    = *r_note_position;
           yellow_next = *y_note_position;
           blue_next   = *b_note_position;
           orange_next = *o_note_position;

           TA0CCR0 = *duration_position; //update to new duration

         }
         else{
             TA1CCR2 = 0; //spaces between note
             TA1CCR0 = 0; //update to new note

             reset;
             green_next  = 0;
             red_next    = 0;
             yellow_next = 0;
             blue_next   = 0;
             orange_next = 0;
             TA0CCR0 = 4000; //update to duration
         }
     //   next_note(green_next, red_next, yellow_next, blue_next, orange_next);
     // Update LED positions for the next iteration
        g_led1 = g_led2;
        g_led2 = g_led3;
        g_led3 = g_led4;
        g_led4 = g_led5;
        g_led5 = g_led6;
        g_led6 = g_led7;
        g_led7 = green_next;

        r_led1 = r_led2;
        r_led2 = r_led3;
        r_led3 = r_led4;
        r_led4 = r_led5;
        r_led5 = r_led6;
        r_led6 = r_led7;
        r_led7 = red_next;

        y_led1 = y_led2;
        y_led2 = y_led3;
        y_led3 = y_led4;
        y_led4 = y_led5;
        y_led5 = y_led6;
        y_led6 = y_led7;
        y_led7 = yellow_next;

        b_led1 = b_led2;
        b_led2 = b_led3;
        b_led3 = b_led4;
        b_led4 = b_led5;
        b_led5 = b_led6;
        b_led6 = b_led7;
        b_led7 = blue_next;

        o_led1 = o_led2;
        o_led2 = o_led3;
        o_led3 = o_led4;
        o_led4 = o_led5;
        o_led5 = o_led6;
        o_led6 = o_led7;
        o_led7 = orange_next;

    }

    __bic_SR_register_on_exit(LPM3_bits); // exit LPM3 when returning to program (clear LPM3 bits)

}

//port 2 button stuff
// Button interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=PORT2_VECTOR
__interrupt void button(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(PORT2_VECTOR))) button (void)
#else
#error Compiler not supported!
#endif
{
    if (state == Intro)
    {
        //start song when press any button
        PlaySound(green_colors, red_colors, yellow_colors, blue_colors, orange_colors, songDuration, songLength);

    }
    if (state == Game)
    {
        if(P2IFG & BIT0)//checks port 2.3
                {
                    if(P2IES & BIT0) {
                        if (debounce_counter == 0){
                               button_press_detected = 1;
                        // falling edge detected - button pressed
                            user_LED_1 = green;
                        P2IES &= ~BIT0; // change edge to rising (button release)

                        }
                    } else {
                        user_LED_1 = 0;
                        P2IES |= BIT0;
                    }
                    P2IFG &= ~BIT0; // clear interrupt flag for button
                }

                if(P2IFG & BIT2)//checks port 2.3
                {
                    if(P2IES & BIT2) {
                        if (debounce_counter == 0){
        //                       button_press_detected = 1;
                        // falling edge detected - button pressed
                            user_LED_2 = red;
                        P2IES &= ~BIT2; // change edge to rising (button release)

                        }
                    } else {
                        user_LED_2 = 0;
                        P2IES |= BIT2;
                    }
                    P2IFG &= ~BIT2; // clear interrupt flag for button
                }

               if(P2IFG & BIT3)//checks port 2.3
               {
                   if(P2IES & BIT3) {
                       if (debounce_counter == 0){
        //                   button_press_detected = 1;
                       // falling edge detected - button pressed
                           user_LED_3 = yellow;
                       P2IES &= ~BIT3; // change edge to rising (button release)

                       }
                   } else {
                       user_LED_3 = 0;
                       P2IES |= BIT3;
                   }
                   P2IFG &= ~BIT3; // clear interrupt flag for button
               }

               if(P2IFG & BIT4)//checks port 2.4
               {
                   if(P2IES & BIT4) {
                       if (debounce_counter == 0){
        //                   button_press_detected = 1;
                       // falling edge detected - button pressed
                           user_LED_4 = blue;
                       P2IES &= ~BIT4; // change edge to rising (button release)

                       }
                   } else {
                       // rising edge detected - button released
                       user_LED_4 = 0;
                       P2IES |= BIT4; // change edge back to falling (button press)
                   }
                   P2IFG &= ~BIT4; // clear interrupt flag for button
               }
               //strum logic
               if(P2IFG & BIT5)//checks port 2.4
               {
                   if(P2IES & BIT5) {
                       if (debounce_counter == 0){
        //                   button_press_detected = 1;
                       // falling edge detected - button pressed
                       strummer = 1;
                       P2IES &= ~BIT5; // change edge to rising (button release)

                       }
                   } else {
                       // rising edge detected - button released
                       strummer = 0;
                       P2IES |= BIT5; // change edge back to falling (button press)
                   }
                   P2IFG &= ~BIT5; // clear interrupt flag for button
               }

    }

    __bic_SR_register_on_exit(LPM3_bits); // exit LPM3 when returning to program (clear LPM3 bits)
}
