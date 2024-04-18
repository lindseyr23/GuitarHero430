#include <msp430.h>
#include <stdbool.h>

int i;
char enter[] = "\r\n";
char song1[] = "Play Song 1";
char stop[] = "Stop Song";
char trigger[] = "start\r\n";
void ser_output(char *str);
void itoa(int n, char s[]);
void reverse(char s[]);

void MusicInit(void){
    BCSCTL1 = CALBC1_1MHZ;
    DCOCTL = CALDCO_1MHZ;
    P1SEL |= BIT1 + BIT2;
    P1SEL2 |= BIT1 + BIT2;
    UCA0CTL1 |= UCSWRST+UCSSEL_2;
    UCA0BR0 = 52; //settings for 19200 baud
    UCA0BR1 = 0;
    UCA0MCTL = UCBRS_0;
    UCA0CTL1 &= ~UCSWRST;
}
void PlaySong(int a)
{
    if(a == 1){
        ser_output(song1);
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
