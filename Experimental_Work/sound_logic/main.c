#include <msp430.h>
#include <stdbool.h>
#include <PlaySong.h>

#include <time.h>

void waitFor (unsigned int secs) {
    unsigned int retTime = time(0) + secs;   // Get finishing time.
    while (time(0) < retTime);               // Loop until it arrives.
}

void main(void)
{
    MusicInit();
    PlaySong(1);
    waitFor(4);
    StopSong();

}

