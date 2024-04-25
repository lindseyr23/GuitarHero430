/*
 * rgb_interface_user.c
 *
 *  Created on: April 13, 2024
 *      Author: Natalia M
 */
#include <rgb_interface_user.h>

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
    P1SEL = BIT2 + BIT4;
    P1SEL2 = BIT2 + BIT4;

    //implements SPI on A0 port
    UCA0CTL1=UCSWRST; //disable serial interface. reset take offline
    UCA0CTL0 |= UCCKPH + UCMSB + UCMST + UCSYNC;    // data cap at 1st clk edge, MSB first, master mode, synchronous
    UCA0CTL1 |= UCSSEL_2;                           // select SMCLK
    UCA0BR0 = 0;                                    //set frequency
    UCA0BR1 = 0;                                    //
    UCA0CTL1 &= ~UCSWRST;           // Initialize USCI state machine. undo reset take online

}

//writes a 32 bit frame to the spi buffer
void rgb_send_frame(const uint8_t *frame1, bool wait_for_completion){
    int byte1;
    for (byte1=0;byte1<4;byte1++){//send 32 bit frame in 8 bit chunks
        UCA0TXBUF=frame1[byte1];  //sends transmit buffer for UCA0
        while (!(IFG2 & UCA0TXIFG));  // USCI_A0 TX buffer ready?
    }
    if (wait_for_completion)
        while (!(IFG2 & UCA0RXIFG));  // USCI_A0 RX buffer ready? (indicates transfer complete)
}

void rgb_send_start() {
    rgb_send_frame(start_frame, false);
}

void rgb_send_end() {
    rgb_send_frame(end_frame, true);
    ready_for_next = 1;//flag to be ready for next
}

void set_temperature(unsigned int led1, unsigned int led2, unsigned int led3, unsigned int led4, unsigned int led5)
{
    //change color
    rgb_send_start();
    rgb_send_frame(&green[led1], false);
    rgb_send_frame(&red[led2], false);
    rgb_send_frame(&yellow[led3], false);
    rgb_send_frame(&blue[led4], false);
    rgb_send_frame(&orange[led5], false);
    rgb_send_end();
}

