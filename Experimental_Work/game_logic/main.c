#include <msp430.h>
#include <stdint.h>
#include <stdbool.h>
#include "rgb_interface.h"
#include "sound_display.h"
#include "serial_comm.h"


void init_wdt(void){
    BCSCTL3 |= LFXT1S_2;     // ACLK = VLO
    WDTCTL = WDT_ADLY_16;    // WDT 16ms (~43.3ms since clk 12khz), ACLK, interval timer
    IE1 |= WDTIE;            // Enable WDT interrupt

    TA1CCTL2 = OUTMOD_7;
    TA1CTL = TASSEL_2 + MC_1; // SMCLK, upmode
}

void init_buttons() { //NEED TO CHECK BUTTON PINS
    P2DIR &= ~(BIT0 + BIT2 + BIT3 + BIT4); // set to input
    P2REN |= BIT0 + BIT2 + BIT3 + BIT4; // enable pullup/down resistors
    P2OUT |= BIT0 + BIT2 + BIT3 + BIT4; // set resistors to pull up

    P2DIR |= BIT5;
    P2SEL |= BIT5; // P2.5 is TA1.2 PWM output

    /* Use interrupts to detect button presses */
    P2IES |= BIT0 + BIT2 + BIT3 + BIT4; // listen for high to low transitions
    P2IFG &=  ~(BIT0 + BIT2 + BIT3 + BIT4); // clear any pending interrupts
    P2IE |= BIT0 + BIT2 + BIT3 + BIT4; // enable interrupts for these pins
}


//global variables

enum state_enum {Intro, PresentingSequence, Game, Lost, Win} state; // enum to describe state of system

void main(){
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
    //Check each button. Plays light and sound per button bases
    if(P2IFG & BIT0)
       {
           if(P2IES & BIT0) {//button pressed

           } else {// rising edge detected - button released

           }
           P2IFG &= ~BIT0; // clear interrupt flag for button
       }
       if(P2IFG & BIT2)
       {
           if(P2IES & BIT2) {//button pressed
           
               }
           } else {// rising edge detected - button released

           }
           P2IFG &= ~BIT2; // clear interrupt flag for button
       }
       
       if(P2IFG & BIT3)
       {
           if(P2IES & BIT3) {//button pressed
               
           } else {// rising edge detected - button released

           }
           P2IFG &= ~BIT3; // clear interrupt flag for button
       }
       

       if(P2IFG & BIT4)
       {
           if(P2IES & BIT4) { //button pressed

           } else {// rising edge detected - button released

           }
           P2IFG &= ~BIT4; // clear interrupt flag for button
       }

    __bic_SR_register_on_exit(CPUOFF); // exit LPM3 when returning to program (clear LPM3 bits)
}

