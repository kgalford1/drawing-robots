// Using the stereo microphones the robot rotates on the spot to face the sound source.

#include "mbed.h" 
#include "m3pi.h"

m3pi m3pi;             

AnalogIn aL(p20);
AnalogIn aR(p19);
DigitalOut dL(LED2);
DigitalOut dR(LED3);
Serial pc(USBTX, USBRX);
 
int main()
{
    float threshold_sound = 0.02;
    float threshold_centre = 0.075;
    float l, r;
    
    while (true) {
        m3pi.stop();
        
        l = abs(aL.read() - 0.5);
        r = abs(aR.read() - 0.5);
        dL = l > threshold_sound;
        dR = r > threshold_sound;
        
        if (abs(l - r) > threshold_centre)
        {
            if (l > r)
                m3pi.left(0.1);
            else
                m3pi.right(0.1);
        }
        wait(0.2);
    }
}
