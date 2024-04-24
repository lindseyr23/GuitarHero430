#include <msp430.h>
#include <stdbool.h>

int i;
char enter[] = "\r\n";
char song1[] = "Play Song 1";
char song2[] = "Play Song 2";
char stop[] = "Stop Song";
char trigger[] = "start\r\n";
void ser_output(char *str);
void itoa(int n, char s[]);
void reverse(char s[]);

void MusicInit(void){
    // Set clock to 1MHz
    //CSCTL0_H = CSKEY_H; // Unlock clock registers
    CSCTL1 = DCORSEL0; // Set DCO to 1MHz
    //CSCTL2 = SELA__LFXTCLK | SELS__DCOCLK | SELM__DCOCLK; // Set ACLK to LFXTCLK, SMCLK and MCLK to DCOCLK
    //CSCTL3 = DIVA__1 | DIVS__1 | DIVM__1; // Set all dividers to 1

    P1SEL1 |= BIT4 | BIT6; // Configure P1.4 and P1.6 for UART
    P1SEL0 |= BIT4 | BIT6;

    UCA0CTLW0 |= UCSWRST; // Reset USCI
    UCA0CTLW0 |= UCSSEL__SMCLK; // Set clock source to SMCLK
    UCA0BRW = 52; // Baud rate settings for 19200 baud
    UCA0MCTLW = UCBRS0; // Modulation UCBRSx = 1
    UCA0CTLW0 &= ~UCSWRST; // Release USCI
}

void PlaySong(int a)
{
    if(a == 1){
        ser_output(song1);
        ser_output(enter);
    }

    if(a == 2){
        ser_output(song2);
        ser_output(enter);
    }
}

void StopSong(void){
    ser_output(stop);
    ser_output(enter);
}
void ser_output(char *str){
while(*str != 0) {
while (!(IFG2 & UCA0TXIFG));
UCA0TXBUF = *str++;
}
}
/* itoa: convert n to characters in s */
void itoa(int n, char s[])
{
    int i, sign;
    if ((sign = n) < 0) /* record sign */
        n = -n; /* make n positive */
    i = 0;
    do { /* generate digits in reverse order */
        s[i++] = n % 10 + '0'; /* get next digit */
    } while ((n /= 10) > 0); /* delete it */
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
