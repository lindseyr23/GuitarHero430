/*
 * serial_comm.c
 *
 *  Created on: Mar 27, 2024
 *      Author: Natalia Mendiola
 */
#include <msp430.h>
//#include <msp430.h>
#include <stdint.h>
#include <stdbool.h>
#include "serial_comm.h"


extern int button; //defines temperature

int i;
//Add game statements here if sending to terminal/display
char charmemval[] = "The latest color added to the LED strip has temperature: ";
char charlost[] = "Game over. Press button to try again!";

char charreturn[] = "\r\n";
char mv_char[5];

// Initialize serial communication via UART
void serial_init_spi(void){
    //configure UART pins
    P1SEL |= BIT1|BIT2;
    P1SEL2 |= BIT1|BIT2;
    //Configure UART settings
    UCA0CTL1 |= UCSWRST+UCSSEL_2;
    UCA0BR0 = 52;  //settings for 19200 baud
    UCA0BR1 = 0;
    UCA0MCTL = UCBRS_0;
    UCA0CTL1 &= ~UCSWRST;
}
//send a string over UART
void ser_output(char *str){
    while(*str != 0) {
        while (!(IFG2&UCA0TXIFG));
        UCA0TXBUF = *str++;
        }
}

/* itoa:  convert n to characters in s */
void itoa(int n, char s[])
{
    int i, sign;

    if ((sign = n) < 0)  /* record sign */
        n = -n;          /* make n positive */
    i = 0;
    do {       /* generate digits in reverse order */
        s[i++] = n % 10 + '0';   /* get next digit */
    } while ((n /= 10) > 0);     /* delete it */
    if (sign < 0)
        s[i++] = '-';
    s[i] = '\0';
    reverse(s);
}

void reverse(char s[])
{
    int i, j;
    char c;

    for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}
