/*
 * rgb_interface.c
 *
 *  Created on: Mar 27, 2024
 *      
 */
#include <msp430.h>
#include <stdint.h>
#include <stdbool.h>
#include "rgb_interface.h"

uint8_t start_frame[] = {0,0,0,0};

//Used to force the 5th led to be off
uint8_t end_frame[] = {0xFF,0xFF,0xFF,0xFF};

uint8_t red[] = {0xF0, 0, 0, 10};
uint8_t blue[] = {0xF0, 10, 0, 0};
uint8_t green[] = {0xF0, 0, 10, 0};
uint8_t yellow[] = {0xF0, 0, 10, 10};
uint8_t off[] = {0xE0, 0, 0, 0};


void rgb_init_spi(void){
    //SOMI 1.7, SCLK on p1.5
    // Configure pins for UCBx SPI mode
    P1SEL |=  BIT7 + BIT5; //SIMO, CLK
    P1SEL2 |= BIT7 + BIT5;

    UCB0CTL1 = UCSWRST; //software reset enable
    UCB0CTL0 = UCCKPH + UCMSB + UCMST + UCSYNC; //master mode, sync mode
    UCB0CTL1 |= UCSSEL_2; //SMCLK
    UCB0BR0 = 0;
    UCB0BR1 = 0;
    UCB0CTL1 &= ~UCSWRST;

}


//writes a 32 bit frame to the spi buffer
void rgb_send_frame(const uint8_t *frame1, bool wait_for_completion){
    int byte1;
    for (byte1=0;byte1<4;byte1++){//send 32 bit frame in 8 bit chunks
        UCB0TXBUF=frame1[byte1];
        while (!(IFG2 & UCB0TXIFG));  // USCI_B0 TX buffer ready?
    }
    if (wait_for_completion)
        while (!(IFG2 & UCB0RXIFG));  // USCI_B0 RX buffer ready? (indicates transfer complete)
}

void rgb_send_start() {
    rgb_send_frame(start_frame, false);
}

void rgb_send_end() {
    rgb_send_frame(end_frame, true);
}



void rgb_set_LEDs(const uint8_t *LED1, const uint8_t *LED2, const uint8_t *LED3, const uint8_t *LED4) {
    rgb_send_start();
    rgb_send_frame(off, false);
    rgb_send_frame(LED1, false);
    rgb_send_frame(LED2, false);
    rgb_send_frame(LED3, false);
    rgb_send_frame(LED4, false);


    rgb_send_end();
}

