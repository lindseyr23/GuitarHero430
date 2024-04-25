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

const unsigned int song1_fortunate_son[] = {//Fortunate Son - Creedence Clearwater Revival
                            0,0,0,0,0,0,0,0, //into
                            0,0,0,0,0,0,0,0,
                            0,0,0,0,0,0,0,0,
                            0,0,0,0,
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

const unsigned int songLength1 = sizeof(song1_fortunate_son) / sizeof(song1_fortunate_son[0]);

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
