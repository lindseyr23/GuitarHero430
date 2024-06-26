
#include <stdio.h>
#include <string.h>

#include "rgb_interface_user.h"
//------------------------------
#include "lcd.h"
#include "msp430g2553.h"


/*
 * Connections Necessary:
 *
 * Green Button: 2.0 Red 2.2 Yellow 2.3 Blue 2.4
 * Strummer: 2.5
 */
/*
green - 1; red - 2; yellow - 3; blue - 4;
green/red - 5; green/yellow -6; green/blue - 7;
red/yellow - 8; red/blue - 9; yellow/blue - 10;
*/
const unsigned int song1[] = {//Fortunate Son - Creedence Clearwater Revival
                           0,0,0,0,0,0,0,0, //intro
                           0,0,0,0,0,0,0,0,
                           0,0,0,0,0,0,0,0, //
                           0,0,0,0,0,0,0,0,
                            3,4,4,4,4,4,4,4, //0:07
                            2,3,3,3,3,3,3,3,
                            1,3,3,3,3,3,3,3,
                            1,2,2,2,3,1,1,1,
                            3,4,4,4,4,4,4,4,
                            2,3,3,3,3,3,3,3,
                            1,3,3,3,3,3,3,3,
                            1,2,2,2,3,1,1,1,
                            9,0,0,9,0,9,9,0, //verse 1? 0:21
                            6,0,0,6,0,6,0,0,
                            10,0,0,10,0,10,0,0,
                            9,0,4,3,0,2,3,2,
                            2,2,2,2,2,2,2,2,
                            6,0,0,6,0,6,0,0,
                            10,0,0,10,0,10,0,0,
                            2,2,1,2,3,2,4,2,
                            2,2,2,2,2,2,2,2, //0:35
                            1,1,1,3,3,3,3,3,
                            2,2,2,4,4,4,3,3,
                            2,1,3,1,4,1,2,1,
                            3,3,3,3,3,3,3,3,//0:35
                            1,1,1,3,3,3,3,3,
                            2,2,2,4,4,4,3,3,
                            1,1,4,4,3,2,1,3,
                            2,2,2,2,2,2,2,2,//0:36
                            1,1,1,3,3,3,3,3,
                            2,2,2,4,4,4,3,3,
                            1,1,4,4,3,2,1,3,
                            2,2,2,2,2,2,2,2,
                            2,2,2,2,2,2,2,2,//0:43
                            1,1,1,3,3,3,3,3,
                            2,2,2,4,4,4,3,3,
                            1,1,4,4,3,2,1,3,
                            2,2,2,2,2,2,2,2,//0:50
                            6,0,0,6,0,6,0,0,
                            10,0,0,10,0,10,0,0,
                            9,0,4,3,0,2,3,2,
                            2,2,2,2,2,2,2,2, //0:57
                            6,0,0,6,0,6,0,0,
                            10,0,0,10,0,10,0,0,
                            9,0,4,3,0,2,3,2,
                            2,2,2,2,2,2,2,2,//1:04
                            1,1,1,3,3,3,3,3,
                            2,2,2,4,4,4,3,3,
                            1,1,4,4,3,2,1,3,
                            2,2,2,2,2,2,2,2,
                            1,1,1,3,3,3,3,3,
                            2,2,2,4,4,4,3,3,
                            1,1,4,4,3,2,1,3,
                            4,4,4,4,4,4,4,4,//1:20
                            10,10,10,10,10,10,2,2,
                            6,6,6,6,6,6,6,6,
                            1,0,3,3,3,3,3,3,
                            9,0,0,9,0,9,0,0,//1:34
                            6,0,0,6,0,6,0,0,
                            10,0,0,10,0,10,0,0,
                            9,0,3,0,4,0,2,0,
                            2,2,2,2,2,2,2,2,
                            6,0,0,6,0,6,0,0,
                            10,0,0,10,0,10,0,0,
                            6,0,10,0,4,0,2,3,2,
                            2,2,2,2,2,2,2,2,//1:48 it aint me
                             1,1,1,3,3,3,3,3,
                             2,2,2,4,4,4,3,3,
                             1,1,4,4,3,2,1,3,
                             2,2,2,2,2,2,2,2,
                             1,1,1,3,3,3,3,3,
                             2,2,2,4,4,4,3,3,
                             1,1,4,4,4,4,1,3,
                             2,2,2,2,2,2,2,2,
                             1,1,1,3,3,3,3,3,
                             2,2,2,4,4,4,3,3,
                             1,1,4,4,4,4,1,3,
                             2,2,2,2,2,2,2,2,
                             1,1,1,3,3,3,3,3,
                             2,2,2,4,4,4,3,3,
                             1,1,4,4,4,4,1,3,
                             2,2,2,2,2,2,2,2,
                             0,0,0,0,0,0,0,0};

const unsigned int songLength1 = sizeof(song1) / sizeof(song1[0]);

unsigned int song2[] = {//Mississippi Queen - Mountain
                        0,0,0,0,0,0,0,0, //into
                        0,0,0,0,0,0,0,0,
                        0,0,0,0,0,1,2,4,
                        2,2,2,0,0,0,0,0,
                        0,0,0,0,0,1,2,4,
                        2,2,2,0,0,0,0,0,
                        3,4,3,2,1,1,3,1    //0:15
                        0,1,2,3,4,4,2,2,
                        1,2,3,4,1,1,2,3
//                        3,4,3,2,1,1,2,3,
//                        5,5,0,0,0,0,0,0, //0:22
//                        0,1,4,1,2,2,3,3, //0:24
//                        5,0,6,0,6,6,6,6,
//                        0,1,4,1,2,2,2,2,
//                        5,0,6,0,6,6,6,6,
//                        0,1,4,1,2,2,3,3,
//                        5,0,6,0,6,6,6,6,
//                        0,1,4,1,2,2,2,2,
//                        8,0,9,9,9,9,9,9,//0:36
//                        0,2,3,2,3,3,4,4,
//                        8,0,9,9,9,9,9,9,
//                        0,0,0,2,3,2,4,4,
//                        5,0,6,0,6,6,6,6, //0:42
//                        0,1,4,1,2,2,2,2

                                        //0:50 changes slightly
};

const unsigned int songLength2 = sizeof(song2) / sizeof(song2[0]);

volatile unsigned int current_note;
volatile unsigned int missed_note;
volatile unsigned int streak;

int user_LED_1;
int user_LED_2;
int user_LED_3;
int user_LED_4;
int button_press_detected;
int strummer;
int score;
int Q=6;
int duration_counter;

volatile unsigned int curr_note;

int song; //takes 1 for song1, 2 for song 2
int debounce_counter;

void check_input(){

    if (song == 1){
        curr_note = song1[current_note];
    }
    else if (song == 2){
       curr_note = song2[current_note];
    }

        switch(curr_note) { //checks if note is correct
            case 1: //green
                if (1 == user_LED_1 & 0 == user_LED_2 & 0 == user_LED_3 & 0 == user_LED_4){
                    score += 10;
                    streak++;
                }
                else {
                    missed_note++;
                    streak = 0;
                }
                break;
            case 2://red
                if (0 == user_LED_1 & 1 == user_LED_2 & 0 == user_LED_3 & 0 == user_LED_4){
                    score += 10;
                    streak++;
                }
                else {
                    missed_note++;
                    streak = 0;
                }
                break;
            case 3: //yellow
                if (0 == user_LED_1 & 0 == user_LED_2 & 1 == user_LED_3 & 0 == user_LED_4){
                    score += 10;
                    streak++;
                }
                else {
                    missed_note++;
                    streak = 0;
                }
                break;
            case 4: //blue
                if (0 == user_LED_1 & 0 == user_LED_2 & 0 == user_LED_3 & 1 == user_LED_4){
                    score += 10;
                    streak++;
                }
                else {
                    missed_note++;
                    streak = 0;
                }
                break;
            case 5: //green/red
                if (1 == user_LED_1 & 1 == user_LED_2 & 0 == user_LED_3 & 0 == user_LED_4){
                    score += 10;
                    streak++;
                }
                else {
                    missed_note++;
                    streak = 0;
                }
                break;
            case 6: //green/yellow
                if (1 == user_LED_1 & 0 == user_LED_2 & 1 == user_LED_3 & 0 == user_LED_4){
                    score += 10;
                    streak++;
                }
                else {
                    missed_note++;
                    streak = 0;
                }
                break;
            case 7: //green/blue
                if (1 == user_LED_1 & 0 == user_LED_2 & 0 == user_LED_3 & 1 == user_LED_4){
                    score += 10;
                    streak++;
                }
                else {
                    missed_note++;
                    streak = 0;
                }
                break;
            case 8: //red/yellow
                if (0 == user_LED_1 & 1 == user_LED_2 & 1 == user_LED_3 & 0 == user_LED_4){
                    score += 10;
                    streak++;
                }
                else {
                    missed_note++;
                    streak = 0;
                }
                break;
            case 9: //red/blue
                if (0 == user_LED_1 & 1 == user_LED_2 & 0 == user_LED_3 & 1 == user_LED_4){
                    score += 10;
                    streak++;
                }
                else {
                    missed_note++;
                    streak = 0;
                }                
                break;
            case 10:// yellow/blue
                if (0 == user_LED_1 & 0 == user_LED_2 & 1 == user_LED_3 & 1 == user_LED_4){
                    score += 10;
                    streak++;
                }
                else {
                    missed_note++;
                    streak = 0;
                }
                break;
        }//end of case

}

void play_song1(int a){ // if a is 1 it starts the song, if a is 0 it stops the song
    if(a==1){
        P2OUT |= BIT1; //send to board2 from p2.1
    }
    else{
        P2OUT &= ~BIT1; //send to board2 from p2.1 
    }
}

void play_song2(int a){// if a is 1 it starts the song, if a is 0 it stops the song
    if(a==1){
        P1OUT |= BIT3; //send to board2 from p1.3
    }
    else{
        P1OUT &= ~BIT3; //send to board2 from p1.3
    }
}

void init_buttons() {
    P2DIR &= ~(BIT0 + BIT2 + BIT3 + BIT4 + BIT5); // set to input
    P2REN |= BIT0 + BIT2 + BIT3 + BIT4 + BIT5; // enable pullup/down resistors
    P2OUT |= BIT0 + BIT2 + BIT3 + BIT4 + BIT5; // set resistors to pull up

    P2IES |= BIT0 + BIT2 + BIT3 + BIT4 + BIT5; // listen for high to low transitions
    P2IFG &=  ~(BIT0 + BIT2 + BIT3 + BIT4 + BIT5); // clear any pending interrupts
    P2IE |= BIT0 + BIT2 + BIT3 + BIT4 + BIT5; // enable interrupts for these pins
}

void init_board_communication() { // 1.1 (accept_input) input, 2.1 (play_song1) output, 1.3 (play_song2) output

    P1DIR &= ~BIT1; // set 1.1 to input

    P2DIR |= BIT1; // set 2.1 to output
    P1DIR |= BIT3; // set 1.3 to output

    //initialize playsound to 0 to not play anything
    P2OUT &= ~BIT1;
    P1OUT &= ~BIT3;
}
//global variables
void set_temperature(unsigned int led1, unsigned int led2, unsigned int led3, unsigned int led4, unsigned int led5);

void init_timerA(void)
{
    TA0CTL |= TACLR;
    TA0CCTL0 = CCIE; //interrupt enable Timer 0
    TA0CCR0 = 4000;    //set up Timer 0
    TA0CTL |= TASSEL_1 + MC_1 + ID_0;
}


enum state_enum {Intro, Game, Lost, Win} state; // enum to describe state of system


int main()
{
    WDTCTL = WDTPW + WDTHOLD; // Stop watchdog


    rgb_init_spi(); //from rgb_interface.c

    __enable_interrupt(); // enable interrupts

    LcdInit(); // initialize LCD display

    init_buttons();
    init_board_communication();
    init_timerA();

    current_note = 0;
    score = 0;
    duration_counter = 0;
    char game_song_text[20]; // Buffer for display text

    state = Intro;

    // Display introductory text on LCD
    LcdSetPosition(2,2); //row, rect  start
    LcdWriteString("Press G or B");

    LcdSetPosition(1,2);
    LcdWriteString("GuitarHero 430");

    // Stop playing any ongoing songs
    play_song1(0);
    play_song2(0);

    while (1)
    {
        if (state == Intro)
        {
            // Display introductory text on LCD
            LcdSetPosition(2,1); //row, rect  start
            LcdWriteString(" Press G or B  ");

            LcdSetPosition(1,1);
            LcdWriteString(" GuitarHero 430  ");

            if (user_LED_1) //if green is pressed
            {
                play_song1(1); //starts playing song 1
                song = 1;
                state = Game;
                current_note = 0;
                duration_counter = 0;
                missed_note = 0;
            }
            else if (user_LED_4)// if blue is pressed
            {
                play_song2(1); //starts playing song 2
                song = 2;
                state = Game;
                current_note = 0;
                duration_counter = 0;
                missed_note = 0;
            }
        }//end of intro
        else if (state == Game)
                {
                    if (user_LED_1 & user_LED_2 & user_LED_3 & user_LED_4) //press all buttons to end the game
                    {
                        play_song1(0); //ends the song
                        play_song2(0); //ends the song
                        state = Intro;
                    }
                    if (strummer) // Check if the strummer is activated
                    {
                        check_input();
                    }

                    if (streak > 6) // Reset missed_note counter if streak exceeds 6
                    {
                        missed_note = 0;
                    }

                    //Check if current note matches the length of the song to determine win state
                    if (((song == 1) && (current_note == songLength1)) || ((song == 2) && (current_note == songLength2))) {
                        state = Win;
                    }
                    // Check if the missed_note counter reaches 40 to determine loss state
                    if (missed_note == 40)
                    {
                        state = Lost;
                        missed_note = 0;
                    }

                    // Format score into a string for display on LCD
                    snprintf(game_song_text, sizeof(game_song_text), "Score: %d", score); // Format score into string
                    strcat(game_song_text, "                "); // Append spaces to clear any previous text


                    // Display score on LCD
                    LcdSetPosition(1,1); //row, rect 
                    LcdWriteString(game_song_text); // Write score to LCD
                    LcdSetPosition(2,1); 
                    LcdWriteString("                    ");// Clear the second row of LCD (20 character space)

                }//end of game state

                if (state == Lost)
                {
                    // Clear the LCD display
                    LcdSetPosition(1,1); //row, rect  
                    LcdWriteString("                    ");//20 character space
                    LcdSetPosition(2,1); //row, rect  
                    LcdWriteString("                    ");//20 character space

                    // Display "Better Luck" message on LCD
                    LcdSetPosition(1,2); //row, rect  
                    LcdWriteString("Better Luck");
                    LcdSetPosition(2,2); //row, rect  
                    LcdWriteString("Next Time!!");//20 character space

                    // Check if a button press is detected to return to the Intro state
                    if (button_press_detected) 
                    {
                        // Reset game state and variables
                        song = 0;
                        current_note = 0;
                        play_song1(0); //signals end of win
                        play_song2(0); //signals end of win

                        // Clear the LCD display
                        LcdSetPosition(1,1); //row, rect
                        LcdWriteString("                    ");//20 character space
                        LcdSetPosition(2,1); //row, rect
                        LcdWriteString("                    ");//20 character space

                        // Display introductory text on LCD
                        LcdSetPosition(2,1); //row, rect  start
                        LcdWriteString(" Press G or B  ");

                        LcdSetPosition(1,1);
                        LcdWriteString(" GuitarHero 430  ");


                        state = Intro;
                    }
                }//end of loss state
                if (state == Win)
                {
                    // Clear the LCD display
                    LcdSetPosition(1,1); //row, rect  
                    LcdWriteString("                    ");//20 character space
                    LcdSetPosition(2,1); //row, rect  
                    LcdWriteString("                    ");//20 character space
                    LcdSetPosition(1,4); //row, rect
                     // Display "You Rock!!! Awesome Playing!" message on LCD  
                    LcdWriteString("You Rock!!!");
                    LcdSetPosition(2,1); //row, rect  start
                    LcdWriteString("Awesome Playing!");//20 character space

                    // Check if a button press is detected to return to the Intro state
                    if (button_press_detected) //if green is pressed
                    {
                        // Reset game state and variables
                        song = 0;
                        current_note = 0;
                        play_song1(0); //signals end of win
                        play_song2(0); //signals end of win
                        state = Intro;
                    }
                }//end of win state

        set_temperature(user_LED_1, user_LED_2, user_LED_3, user_LED_4, 0); //displays user input
        LPM3;
    }//end of while

//return 0;

} //end of main

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
                    P2IES &= ~BIT0; //change edge to rising (button release)
                }
            } else {// rising edge detected - button released
                user_LED_1 = 0;
                P2IES |= BIT0; // Change edge to falling (button press) 
            }
            P2IFG &= ~BIT0; // clear interrupt flag for button
        }

        if(P2IFG & BIT2)//checks port 2.2
        {
            if(P2IES & BIT2) { // falling edge detected - button pressed
                if (debounce_counter == 0){
                    button_press_detected = 1;
                    user_LED_2 = 1;
                    P2IES &= ~BIT2; // change edge to rising (button release)
                }
            } else { // rising edge detected - button released
                user_LED_2 = 0;
                P2IES |= BIT2; // Change edge to falling (button press)
            }
            P2IFG &= ~BIT2; // clear interrupt flag for button
        }
       if(P2IFG & BIT3)//checks port 2.3
       {
           if(P2IES & BIT3) {
               if (debounce_counter == 0){  // falling edge detected - button pressed
                    button_press_detected = 1;
                    user_LED_3 = 1;
                    P2IES &= ~BIT3; // change edge to rising (button release)
               }
           } else {// rising edge detected - button released
               user_LED_3 = 0;
               P2IES |= BIT3; // Change edge to falling (button press)
           }
           P2IFG &= ~BIT3; // clear interrupt flag for button
       }

       if(P2IFG & BIT4)//checks port 2.4
       {
           if(P2IES & BIT4) {
               if (debounce_counter == 0){// falling edge detected - button pressed
                   button_press_detected = 1;
                   user_LED_4 = 1;
                    P2IES &= ~BIT4; // change edge to rising (button release)
               }
           } else { // rising edge detected - button released
               button_press_detected = 1;
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
                   button_press_detected = 1;
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
}// end of port 2 ISR


#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A(void)
{
    if (state == Game | state == Intro)  // Check if the game is in Game or Intro state
    {
        duration_counter++;
        if (duration_counter > Q){
            current_note++; //move to the next note
            duration_counter = 0;
        }
    }
}//end of A0 ISR
