#include <msp430.h>
#include <stdint.h>
#include <stdbool.h>
#include "rgb_interface.h"
#include "sound_display.h"
#include "serial_comm.h"


#define green 1
#define red 2
#define yellow 3
#define blue 4
#define orange 5

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


    _enable_interrupts();


    while(1){
        if (strummer){
            set_temperature(0,user_LED_1, user_LED_2, user_LED_3, user_LED_4); //led only show when strummer is held down...need to think more about logic
        }
        else{
            set_temperature(0,0, 0, 0, 0);
        }
        LPM3;

    }//end of while


}//end of main

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

    __bic_SR_register_on_exit(CPUOFF); // exit LPM3 when returning to program (clear LPM3 bits)
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
                    user_LED_1 = green;
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
                    user_LED_2 = red;
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
                    user_LED_3 = yellow;
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
                   user_LED_4 = blue;
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