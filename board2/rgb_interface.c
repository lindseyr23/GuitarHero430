/*
 * rgb_interface.c
 *
 *  Created on: April 13, 2024
 *      Author: Natalia M
 */
#include "rgb_interface.h"

int ready_for_next;//flag to be ready for next
uint8_t start_frame[] = {0,0,0,0};
uint8_t end_frame[] = {0xFF,0xFF,0xFF,0xFF};

const uint8_t green[][4] =
{
     {0xE0, 0, 0, 0},        // 0 - Off
     {0xF0, 0, 10, 0}        // 1 - Green
};

const uint8_t red[][4] =
{
     {0xE0, 0, 0, 0},        // 0 - Off
     {0xF0, 0, 0, 10}        // 1 - Red
};

const uint8_t yellow[][4] =
{
     {0xE0, 0, 0, 0},        // 0 - Off
     {0xF0, 0, 10, 10}       // 1 - Yellow CHECK VALUE
};

const uint8_t blue[][4] =
{
     {0xE0, 0, 0, 0},        // 0 - Off
     {0xF0, 10, 0, 0}        // 1 - Blue
};

const uint8_t orange[][4] =
{
     {0xE0, 0, 0, 0},        // 0 - Off
     {0xF0, 0, 6, 10}        // 1 - Orange CHECK VALUE

};



void rgb_init_spi(void){ //initialize serial bus to drive RGB
    //COPI on p1.2, SCLK on p1.4
//    P1SEL = BIT2 + BIT4;
//    P1SEL2 = BIT2 + BIT4;
    P1SEL |= BIT5 + BIT7;
    P1SEL2 |= BIT5 + BIT7;

	//implements SPI on B0 port
    UCB0CTL1=UCSWRST; //disable serial interface. reset take offline
    UCB0CTL0 |= UCCKPH + UCMSB + UCMST + UCSYNC;    // data cap at 1st clk edge, MSB first, master mode, synchronous
    UCB0CTL1 |= UCSSEL_2;                           // select SMCLK
    UCB0BR0 = 0;                                    //set frequency
    UCB0BR1 = 0;                                    //
    UCB0CTL1 &= ~UCSWRST;           // Initialize USCI state machine. undo reset take online

}

//writes a 32 bit frame to the spi buffer
void rgb_send_frame(const uint8_t *frame1, bool wait_for_completion){
    int byte1;
    for (byte1=0;byte1<4;byte1++){//send 32 bit frame in 8 bit chunks
        UCB0TXBUF=frame1[byte1];  //sends transmit buffer for UCA0
        while (!(IFG2 & UCB0TXIFG));  // USCI_A0 TX buffer ready?
    }
    if (wait_for_completion)
        while (!(IFG2 & UCB0RXIFG));  // USCI_A0 RX buffer ready? (indicates transfer complete)
}

void rgb_send_start() {
    rgb_send_frame(start_frame, false);
}

void rgb_send_end() {
    rgb_send_frame(end_frame, true);
    ready_for_next = 1;//flag to be ready for next
}


//led7 is the led farthest from the cable.
//direction of arrows on the board along G go from led1 - led7
void set_green(unsigned int led1, unsigned int led2, unsigned int led3, unsigned int led4, unsigned int led5, unsigned int led6, unsigned int led7)
{
    //change color - 1st row 7 led.
    rgb_send_frame(&green[led1], false);
    rgb_send_frame(&green[led2], false);
    rgb_send_frame(&green[led3], false);
    rgb_send_frame(&green[led4], false);
    rgb_send_frame(&green[led5], false);
    rgb_send_frame(&green[led6], false);
    rgb_send_frame(&green[led7], false);
}

void set_red(unsigned int led1, unsigned int led2, unsigned int led3, unsigned int led4, unsigned int led5, unsigned int led6, unsigned int led7)
{
    //change color - 1st row 7 led.
    rgb_send_frame(&red[led1], false);
    rgb_send_frame(&red[led2], false);
    rgb_send_frame(&red[led3], false);
    rgb_send_frame(&red[led4], false);
    rgb_send_frame(&red[led5], false);
    rgb_send_frame(&red[led6], false);
    rgb_send_frame(&red[led7], false);
}

void set_yellow(unsigned int led1, unsigned int led2, unsigned int led3, unsigned int led4, unsigned int led5, unsigned int led6, unsigned int led7)
{
    //change color - 1st row 7 led.
    rgb_send_frame(&yellow[led1], false);
    rgb_send_frame(&yellow[led2], false);
    rgb_send_frame(&yellow[led3], false);
    rgb_send_frame(&yellow[led4], false);
    rgb_send_frame(&yellow[led5], false);
    rgb_send_frame(&yellow[led6], false);
    rgb_send_frame(&yellow[led7], false);
}
void set_blue(unsigned int led1, unsigned int led2, unsigned int led3, unsigned int led4, unsigned int led5, unsigned int led6, unsigned int led7)
{
    //change color - 1st row 7 led.
    rgb_send_frame(&blue[led1], false);
    rgb_send_frame(&blue[led2], false);
    rgb_send_frame(&blue[led3], false);
    rgb_send_frame(&blue[led4], false);
    rgb_send_frame(&blue[led5], false);
    rgb_send_frame(&blue[led6], false);
    rgb_send_frame(&blue[led7], false);
}
void set_orange(unsigned int led1, unsigned int led2, unsigned int led3, unsigned int led4, unsigned int led5, unsigned int led6, unsigned int led7)
{
    //change color - 1st row 7 led.
    rgb_send_frame(&orange[led1], false);
    rgb_send_frame(&orange[led2], false);
    rgb_send_frame(&orange[led3], false);
    rgb_send_frame(&orange[led4], false);
    rgb_send_frame(&orange[led5], false);
    rgb_send_frame(&orange[led6], false);
    rgb_send_frame(&orange[led7], false);
}

void g_ripple(unsigned int next_seq, unsigned int led_1, unsigned int led_2, unsigned int led_3, unsigned int led_4, unsigned int led_5, unsigned int led_6, unsigned int led_7)
{
    // Shift the color values among the LEDs
    //toward cable
    int temp = next_seq; //led_1;
    led_1 = led_2;
    led_2 = led_3;
    led_3 = led_4;
    led_4 = led_5;
    led_5 = led_6;
    led_6 = led_7;
    led_7 = temp;
//    // away from cables
//    int temp = next_seq;
//    led_7 = led_6;
//    led_6 = led_5;
//    led_5 = led_4;
//    led_4 = led_3;
//    led_3 = led_2;
//    led_2 = led_1;
//    led_1 = temp;
    set_green(led_1,led_2,led_3,led_4,led_5,led_6,led_7);
}

void r_ripple(unsigned int next_seq, unsigned int led_1, unsigned int led_2, unsigned int led_3, unsigned int led_4, unsigned int led_5, unsigned int led_6, unsigned int led_7)
{
    // Shift the color values among the LEDs
    //toward cable
    unsigned int temp = next_seq; //led_1;
    led_1 = led_2;
    led_2 = led_3;
    led_3 = led_4;
    led_4 = led_5;
    led_5 = led_6;
    led_6 = led_7;
    led_7 = temp;
//    // away from cables
//    int temp = next_seq;
//    led_7 = led_6;
//    led_6 = led_5;
//    led_5 = led_4;
//    led_4 = led_3;
//    led_3 = led_2;
//    led_2 = led_1;
//    led_1 = temp;
    set_red(led_1,led_2,led_3,led_4,led_5,led_6,led_7);
    //set_red(r_led1, r_led2, r_led3, r_led4, r_led5, r_led6, r_led7);
}

void y_ripple(unsigned int next_seq, unsigned int led_1, unsigned int led_2, unsigned int led_3, unsigned int led_4, unsigned int led_5, unsigned int led_6, unsigned int led_7)
{
    // Shift the color values among the LEDs
    //toward cable
    unsigned int temp = next_seq; //led_1;
    led_1 = led_2;
    led_2 = led_3;
    led_3 = led_4;
    led_4 = led_5;
    led_5 = led_6;
    led_6 = led_7;
    led_7 = temp;
//    // away from cables
//    int temp = next_seq;
//    led_7 = led_6;
//    led_6 = led_5;
//    led_5 = led_4;
//    led_4 = led_3;
//    led_3 = led_2;
//    led_2 = led_1;
//    led_1 = temp;
    set_yellow(led_1,led_2,led_3,led_4,led_5,led_6,led_7);
}

void b_ripple(unsigned int next_seq, unsigned int led_1, unsigned int led_2, unsigned int led_3, unsigned int led_4, unsigned int led_5, unsigned int led_6, unsigned int led_7)
{
    // Shift the color values among the LEDs
    //toward cable
    unsigned int temp = next_seq; //led_1;
    led_1 = led_2;
    led_2 = led_3;
    led_3 = led_4;
    led_4 = led_5;
    led_5 = led_6;
    led_6 = led_7;
    led_7 = temp;
//    // away from cables
//    int temp = next_seq;
//    led_7 = led_6;
//    led_6 = led_5;
//    led_5 = led_4;
//    led_4 = led_3;
//    led_3 = led_2;
//    led_2 = led_1;
//    led_1 = temp;
    set_blue(led_1,led_2,led_3,led_4,led_5,led_6,led_7);
}
void o_ripple(unsigned int next_seq, unsigned int led_1, unsigned int led_2, unsigned int led_3, unsigned int led_4, unsigned int led_5, unsigned int led_6, unsigned int led_7)
{
    // Shift the color values among the LEDs
    //toward cable
    unsigned int temp = next_seq; //led_1;
    led_1 = led_2;
    led_2 = led_3;
    led_3 = led_4;
    led_4 = led_5;
    led_5 = led_6;
    led_6 = led_7;
    led_7 = temp;
//    // away from cables
//    int temp = next_seq;
//    led_7 = led_6;
//    led_6 = led_5;
//    led_5 = led_4;
//    led_4 = led_3;
//    led_3 = led_2;
//    led_2 = led_1;
//    led_1 = temp;
    set_orange(led_1,led_2,led_3,led_4,led_5,led_6,led_7);
}



void PlaySound(unsigned int *notes, unsigned int length)
{
    songLength = length;
    note_position = &notes[0]; //gets address of first note

    current_note = 0;
}

void win_screen(void)
{
        //change red
    rgb_send_start();
    //row 1
    rgb_send_frame(&green[1], false);
    rgb_send_frame(&green[1], false);
    rgb_send_frame(&green[1], false);
    rgb_send_frame(&green[1], false);
    rgb_send_frame(&green[1], false);
    rgb_send_frame(&green[1], false);
    rgb_send_frame(&green[1], false);
    //row 2
    rgb_send_frame(&green[1], false);
    rgb_send_frame(&green[1], false);
    rgb_send_frame(&green[1], false);
    rgb_send_frame(&green[1], false);
    rgb_send_frame(&green[1], false);
    rgb_send_frame(&green[1], false);
    rgb_send_frame(&green[1], false);
    //row 3
    rgb_send_frame(&green[1], false);
    rgb_send_frame(&green[1], false);
    rgb_send_frame(&green[1], false);
    rgb_send_frame(&green[1], false);
    rgb_send_frame(&green[1], false);
    rgb_send_frame(&green[1], false);
    rgb_send_frame(&green[1], false);
    //row 4
    rgb_send_frame(&green[1], false);
    rgb_send_frame(&green[1], false);
    rgb_send_frame(&green[1], false);
    rgb_send_frame(&green[1], false);
    rgb_send_frame(&green[1], false);
    rgb_send_frame(&green[1], false);
    rgb_send_frame(&green[1], false);
    //row 5
    rgb_send_frame(&green[1], false);
    rgb_send_frame(&green[1], false);
    rgb_send_frame(&green[1], false);
    rgb_send_frame(&green[1], false);
    rgb_send_frame(&green[1], false);
    rgb_send_frame(&green[1], false);
    rgb_send_frame(&green[1], false);
    rgb_send_end();
}

void loss_screen(void)
{
    //change red
    rgb_send_start();
    //row 1
    rgb_send_frame(&red[1], false);
    rgb_send_frame(&red[1], false);
    rgb_send_frame(&red[1], false);
    rgb_send_frame(&red[1], false);
    rgb_send_frame(&red[1], false);
    rgb_send_frame(&red[1], false);
    rgb_send_frame(&red[1], false);
    //row 2
    rgb_send_frame(&red[1], false);
    rgb_send_frame(&red[1], false);
    rgb_send_frame(&red[1], false);
    rgb_send_frame(&red[1], false);
    rgb_send_frame(&red[1], false);
    rgb_send_frame(&red[1], false);
    rgb_send_frame(&red[1], false);
    //row 3
    rgb_send_frame(&red[1], false);
    rgb_send_frame(&red[1], false);
    rgb_send_frame(&red[1], false);
    rgb_send_frame(&red[1], false);
    rgb_send_frame(&red[1], false);
    rgb_send_frame(&red[1], false);
    rgb_send_frame(&red[1], false);
    //row 4
    rgb_send_frame(&red[1], false);
    rgb_send_frame(&red[1], false);
    rgb_send_frame(&red[1], false);
    rgb_send_frame(&red[1], false);
    rgb_send_frame(&red[1], false);
    rgb_send_frame(&red[1], false);
    rgb_send_frame(&red[1], false);
    //row 5
    rgb_send_frame(&red[1], false);
    rgb_send_frame(&red[1], false);
    rgb_send_frame(&red[1], false);
    rgb_send_frame(&red[1], false);
    rgb_send_frame(&red[1], false);
    rgb_send_frame(&red[1], false);
    rgb_send_frame(&red[1], false);
    rgb_send_end();
}