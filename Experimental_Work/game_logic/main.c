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

enum state_enum {Intro, Game, Lost, Win} state; // enum to describe state of system

//unsigned int green_colors_intro[] = {0,1,0,0,0,0,0,0,0};
//unsigned int red_colors_intro[]   = {0,0,1,0,0,0,0,0,1};
//unsigned int yellow_colors_intro[]= {0,0,0,1,0,0,0,1,0};
//unsigned int blue_colors_intro[]  = {0,0,0,0,1,0,1,0,0};
//unsigned int orange_colors_intro[]= {0,0,0,0,0,1,0,0,0};

unsigned int green_colors_song1[] = {1,1,1,0,0,0,1,1,1};
unsigned int red_colors_song1[]   = {0,0,0,1,1,1,0,0,0};
unsigned int yellow_colors_song1[]= {0,0,0,0,0,0,0,0,0};
unsigned int blue_colors_song1[]  = {0,0,0,0,0,0,0,0,0};
unsigned int orange_colors_song1[]= {0,0,0,0,0,0,0,0,0};
//unsigned int songDuration_song1[] = {Q,Q,Q,Q,Q,Q,Q,Q,Q};

unsigned int green_colors_song2[] = {0,0,0,0,0,0,0,0,0};
unsigned int red_colors_song2[]   = {0,0,0,0,0,0,0,0,0};
unsigned int yellow_colors_song2[]= {0,0,0,0,0,0,0,0,0};
unsigned int blue_colors_song2[]  = {1,1,1,1,1,1,1,1,1};
unsigned int orange_colors_song2[]= {0,0,0,0,0,0,0,0,0};
//unsigned int songDuration_song2[] = {Q,Q,Q,Q,Q,Q,Q,Q,Q};

int songLength1 = sizeof(green_colors_song1) / sizeof(green_colors_song1[0]);
int songLength2 = sizeof(green_colors_song2) / sizeof(green_colors_song2[0]);
int current_note;

/*
 * Connections Necessary:
 *
 * Green Button: 2.0 Red Yellow Blue Orange
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

void init_board_communication() { // 2.7 (accept_input) input, 2.1 (play_song1) output, 1.3 (play_song2) output
    P2DIR &= ~(BIT7); // set 2.7 to input
    P2DIR |= BIT1; // set 2.1 to output
    P1DIR |= BIT3; // set 1.3 to output


    //initialize playsound to 0 to not play anything
    P2OUT &= ~BIT1;
    P1OUT &= ~BIT3;

    P2IES &= ~BIT7; // listen for low to high transition to start
    P2IFG &=  ~BIT7; // clear any pending interrupts
    P2IE |= BIT7; // enable interrupts for these pins
}
//global variables
void set_temperature(int led1, int led2, int led3, int led4, int led5);


int user_LED_1;
int user_LED_2;
int user_LED_3;
int user_LED_4;
int user_LED_5;
int button_press_detected;
int strummer;
int score;
int accept_input;
int led1;
int led2;
int led3;
int led4;
int led5;

int song; //takes 1 for song1, 2 for song 2

// green red yellow blue orange is the order of the LEDS
void check_input(){

    if (song == 1){
        led1 = green_colors_song1[current_note];
        led2 = red_colors_song1[current_note];
        led3 = yellow_colors_song1[current_note];
        led4 = blue_colors_song1[current_note];
        led5 = orange_colors_song1[current_note];
    }
    else if (song == 2){
        led1 = green_colors_song2[current_note];
        led2 = red_colors_song2[current_note];
        led3 = yellow_colors_song2[current_note];
        led4 = blue_colors_song2[current_note];
        led5 = orange_colors_song2[current_note];
    }

    if (led1 == user_LED_1 & led2 == user_LED_2 & led3 == user_LED_3 & led4 == user_LED_4 & led5 == user_LED_5){
        score += 10;
    }
}

void play_song1(int a){ // if a is 1 it starts the song, if a is 0 it stops the song
    if(a==1){
        P2OUT |= BIT1; // Add code here to assign the colorset we are using
    }
    else{
        P2OUT &= ~BIT1;
    }
}

void play_song2(int a){
    if(a==1){
        P1OUT |= BIT3; // Add code here to assign the colorset we are using
    }
    else{
        P1OUT &= ~BIT3;
    }
}
//void PlayIntro(){
//    if (user_LED_1) //if green is pressed
//    {
//        play_song1(1); //starts the song
//        song = 1;
//        button_press_detected = 0; //to prevent overlap
//        state = Game;
//    }
//    else if (user_LED_4)// if blue is pressed
//    {
//        play_song2(1); //starts the song
//        song = 2;
//        button_press_detected = 0; //to prevent overlap
//        state = Game;
//    }
//}

int debounce_counter = 0;

void main(void){
    WDTCTL = WDTPW | WDTHOLD; // stop watchdog timer
    BCSCTL1 = CALBC1_1MHZ;     // Set range
    DCOCTL = CALDCO_1MHZ;      // Set DCO step + modulation */
    BCSCTL3 |= LFXT1S_2;        // ACLK = VLO - This is also called in the init_wdt() function
    TA1CCTL2 = OUTMOD_7;
    TA1CTL = TASSEL_2 + MC_1 + TACLR; // SMCLK, upmode
//    init_wdt();
    rgb_init_spi(); //from rgb_interface.c
    serial_init_spi(); //from serial_comm.c
    init_buttons();
    init_board_communication();
    current_note = 0;
    score = 0;
    accept_input = 0;

    _enable_interrupts();

    state = Intro;

    while(1){

        if (state == Intro)
        {
//            PlayIntro();
            if (user_LED_1) //if green is pressed
            {
                play_song1(1); //starts the song
                song = 1;
//                button_press_detected = 0; //to prevent overlap
                state = Game;
            }
            else if (user_LED_4)// if blue is pressed
            {
                play_song2(1); //starts the song
                song = 2;
//                button_press_detected = 0; //to prevent overlap
                state = Game;
            }
        }
        else if (state == Game)
        {
            if (user_LED_1 & user_LED_2 & user_LED_3 & user_LED_4)
            {
                play_song1(0); //starts the song
                play_song2(0); //starts the song
            }
            if (strummer){
                      check_input();
                  }
        }
//        else if (state == Lost)
//        {
//            PlayLoss();
//        }
//        else if (state == Win)
//        {
//            PlayWin();
//        }

        set_temperature(user_LED_1, user_LED_2, user_LED_3, user_LED_4, 0); //displays user input

        LPM3;

    }//end of while


}//end of main

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
       // accept_input logic
       if(P2IFG & BIT7)//checks port 2.7
       {
           current_note++;
            //           if(P2IES & BIT7) { // falling edge detected - changed to 0
            ////               accept_input = 0;
            ////               P2IES &= ~BIT7; // change edge to rising
            //           } else { // rising edge detected - changed to 1
            ////               accept_input = 1;
            //               current_note++;
            ////               P2IES |= BIT7; // change edge back to falling (button press)
            //           }
           P2IFG &= ~BIT7; // clear interrupt flag
       }

    __bic_SR_register_on_exit(LPM3_bits); // exit LPM3 when returning to program (clear LPM3 bits)
}
