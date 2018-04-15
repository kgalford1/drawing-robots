// Supposedly will light up the led when the sound level is over the threshold.
// Due to the nature of sound, we found problems with this basic approach, even
// without considering frequency.
// UPDATE:
// Have gotten this program to work. Turns out on of the ADMP401 boards is broken.
// The ADMP401 is very directional, whereas the MAX4466 is the opposite.

#include "mbed.h"              

AnalogIn a0(p18);
AnalogIn a1(p19);
AnalogIn a2(p20);
DigitalOut d0(LED1);
DigitalOut d1(LED2);
DigitalOut d2(LED3);
Serial pc(USBTX, USBRX);
 
int main()
{
    float threshold = 0.1;
    
    while (true) {
        d0 = abs(a0.read() - 0.5) > threshold;
        d1 = abs(a1.read() - 0.5) > threshold;
        d2 = abs(a2.read() - 0.5) > threshold;
    }
}
