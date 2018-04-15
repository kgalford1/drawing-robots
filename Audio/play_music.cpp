// Uses the onboard buzzer to play music.
// Syntax for songs: https://www.pololu.com/docs/0J18/3

#include "mbed.h"
#include "m3pi.h"
 
m3pi m3pi;
char* four = "L4 O2 T150 V12 bcrr";
 
int main()
{
    m3pi.playBuzzer(go);
    return 0;
}
