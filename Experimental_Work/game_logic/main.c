#include <msp430.h>
#include <stdint.h>
#include <stdbool.h>
#include "serial_comm.h"
#include "rgb_interface_user.h"

#define green 1
#define red 2
#define yellow 3
#define blue 4
#define orange 5

int green_colors[] = {0,1,0,0,0,0,0,0,0};
int red_colors[]   = {0,0,1,0,0,0,0,0,1};
int yellow_colors[]= {0,0,0,1,0,0,0,1,0};
int blue_colors[]  = {0,0,0,0,1,0,1,0,0};
int orange_colors[]= {0,0,0,0,0,1,0,0,0};
int songDuration[] = {Q,Q,Q,Q,Q,Q,Q,Q,Q};

int songLength = sizeof(green_colors) / sizeof(green_colors[0]);
int current_note;

/*
 * Connections Necessary:
 *
 * Green Button: ;
 */

void init_wdt(void){
    BCSCTL3 |= LFXT1S_2;     // ACLK = VLO
    WDTCTL = WDT_ADLY_250;
    // WDTCTL = WDT_ADLY_16;    // WDT 16ms (~43.3ms since clk 12khz), ACLK, interval timer
    IE1 |= WDTIE;            // Enable WDT interrupt

    TA1CCTL2 = OUTMOD_7;
    TA1CTL = TASSEL_2 + MC_1; // SMCLK, upmode
}

void init_buttons() {
    P2DIR &= ~(BIT0 + BIT2 + BIT3 + BIT4 + BIT5); // set to input
    P2REN |= BIT0 + BIT2 + BIT3 + BIT4 + BIT5; // enable pullup/down resistors
    P2OUT |= BIT0 + BIT2 + BIT3 + BIT4 + BIT5; // set resistors to pull up

    /* Uncomment the following code if you want to use interrupts to detect button presses */
    P2IES |= BIT0 + BIT2 + BIT3 + BIT4 + BIT5; // listen for high to low transitions
    P2IFG &=  ~(BIT0 + BIT2 + BIT3 + BIT4 + BIT5); // clear any pending interrupts
    P2IE |= BIT0 + BIT2 + BIT3 + BIT4 + BIT5; // enable interrupts for these pins
}


//global variables
void set_temperature(int led1, int led2, int led3, int led4, int led5);

enum state_enum {Intro, Game, Lost, Win} state; // enum to describe state of system

int user_LED_1;
int user_LED_2;
int user_LED_3;
int user_LED_4;
int button_press_detected;
int strummer;

int debounce_counter = 0;

void main(void){
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    BCSCTL1 = CALBC1_1MHZ;     // Set range
    DCOCTL = CALDCO_1MHZ;      // Set DCO step + modulation */
    BCSCTL3 |= LFXT1S_2;        // ACLK = VLO - This is also called in the init_wdt() function
    TA1CCTL2 = OUTMOD_7;
    TA1CTL = TASSEL_2 + MC_1 + TACLR; // SMCLK, upmode
    init_wdt();
    rgb_init_spi(); //from rgb_interface.c
    serial_init_spi(); //from serial_comm.c
    init_buttons();
    current_note = 0;


    _enable_interrupts();


    while(1){
        set_temperature(0,user_LED_1, user_LED_2, user_LED_3, user_LED_4);
//        if (strummer){
//            set_temperature(0,user_LED_1, user_LED_2, user_LED_3, user_LED_4); //led only show when strummer is held down...need to think more about logic
//        }
//        else{
//            set_temperature(0,0, 0, 0, 0);
//        }
        LPM3;

    }//end of while


}//end of main

//// Watchdog Timer interrupt service routine
//#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
//#pragma vector=WDT_VECTOR
//__interrupt void watchdog_timer(void)
//#elif defined(__GNUC__)
//void __attribute__ ((interrupt(WDT_VECTOR))) watchdog_timer (void)
//#else
//#error Compiler not supported!
//#endif
//{
//
//    __bic_SR_register_on_exit(CPUOFF); // exit LPM3 when returning to program (clear LPM3 bits)
//}



// Watchdog Timer interrupt service routine
// TODO: This should update the
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
           TA0CCR0 = *duration_position; //update to new duration

         }
         else{
             TA1CCR2 = 0; //spaces between note
             TA1CCR0 = 0; //update to new note

             //reset
//             green_next  = 0;
//             red_next    = 0;
//             yellow_next = 0;
//             blue_next   = 0;
//             orange_next = 0;
             TA0CCR0 = 4000; //update to duration
         }
     //   next_note(green_next, red_next, yellow_next, blue_next, orange_next);
     // Update LED positions for the next iteration
//        g_led1 = g_led2;
//        g_led2 = g_led3;
//        g_led3 = g_led4;
//        g_led4 = g_led5;
//        g_led5 = g_led6;
//        g_led6 = g_led7;
//        g_led7 = green_next;
//
//        r_led1 = r_led2;
//        r_led2 = r_led3;
//        r_led3 = r_led4;
//        r_led4 = r_led5;
//        r_led5 = r_led6;
//        r_led6 = r_led7;
//        r_led7 = red_next;
//
//        y_led1 = y_led2;
//        y_led2 = y_led3;
//        y_led3 = y_led4;
//        y_led4 = y_led5;
//        y_led5 = y_led6;
//        y_led6 = y_led7;
//        y_led7 = yellow_next;
//
//        b_led1 = b_led2;
//        b_led2 = b_led3;
//        b_led3 = b_led4;
//        b_led4 = b_led5;
//        b_led5 = b_led6;
//        b_led6 = b_led7;
//        b_led7 = blue_next;
//
//        o_led1 = o_led2;
//        o_led2 = o_led3;
//        o_led3 = o_led4;
//        o_led4 = o_led5;
//        o_led5 = o_led6;
//        o_led6 = o_led7;
//        o_led7 = orange_next;

    }

    __bic_SR_register_on_exit(LPM3_bits); // exit LPM3 when returning to program (clear LPM3 bits)

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
        if(P2IFG & BIT0)//checks port 2.0
        {
            if(P2IES & BIT0) { // falling edge detected - button pressed
                if (debounce_counter == 0){ 
                   button_press_detected = 1; 
                    user_LED_1 = 1;
                    //user_LED_1 = green;
                    P2IES &= ~BIT0; // change edge to rising (button release)
                }
            } else {// rising edge detected - button released
                user_LED_1 = 0;
                P2IES |= BIT0;
            }
            P2IFG &= ~BIT0; // clear interrupt flag for button
        }

        if(P2IFG & BIT2)//checks port 2.2
        {
            if(P2IES & BIT2) { // falling edge detected - button pressed
                if (debounce_counter == 0){
                    user_LED_2 = 1;
                    P2IES &= ~BIT2; // change edge to rising (button release)
                }
            } else { // rising edge detected - button released
                user_LED_2 = 0;
                P2IES |= BIT2;
            }
            P2IFG &= ~BIT2; // clear interrupt flag for button
        }
       if(P2IFG & BIT3)//checks port 2.3
       {
           if(P2IES & BIT3) {
               if (debounce_counter == 0){  // falling edge detected - button pressed
                    user_LED_3 = 1;
                    P2IES &= ~BIT3; // change edge to rising (button release)
               }
           } else {// rising edge detected - button released
               user_LED_3 = 0;
               P2IES |= BIT3;
           }
           P2IFG &= ~BIT3; // clear interrupt flag for button
       }

       if(P2IFG & BIT4)//checks port 2.4
       {
           if(P2IES & BIT4) {
               if (debounce_counter == 0){// falling edge detected - button pressed
                   user_LED_4 = 1;
                    P2IES &= ~BIT4; // change edge to rising (button release)
               }
           } else { // rising edge detected - button released
               user_LED_4 = 0;
               P2IES |= BIT4; // change edge back to falling (button press)
           }
           P2IFG &= ~BIT4; // clear interrupt flag for button
       }
       //strum logic
       if(P2IFG & BIT5)//checks port 2.5
       {
           if(P2IES & BIT5) {
               if (debounce_counter == 0){
               // falling edge detected - button pressed
               strummer = 1;
               P2IES &= ~BIT5; // change edge to rising (button release)
               }
           } else { // rising edge detected - button released
               strummer = 0;
               P2IES |= BIT5; // change edge back to falling (button press)
           }
           P2IFG &= ~BIT5; // clear interrupt flag for button
       }


    __bic_SR_register_on_exit(LPM3_bits); // exit LPM3 when returning to program (clear LPM3 bits)
}
