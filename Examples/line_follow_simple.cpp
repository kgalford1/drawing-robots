// A basic line following algorithm from:
// https://developer.mbed.org/cookbook/m3pi-LineFollowing

#include "mbed.h"
#include "m3pi.h"
 
m3pi m3pi;
 
int main()
{
    // Operation Parameters
    float speed = 0.1;
    float correction = 0.2;   
    float threshold = 0.25;
 
    m3pi.locate(0,1);
    m3pi.printf("Line Flw");
    wait(2);
    m3pi.sensor_auto_calibrate();
    
    while (true)
    {
        // -1.0 is far left, 0.0 in the middle, 1.0 is far right 
        float position_of_line = m3pi.line_position();
 
        // Line is more than the threshold to the right, slow the left motor
        if (position_of_line > threshold)
        {
            m3pi.right_motor(speed);
            m3pi.left_motor(speed-correction);
        }
 
        // Line is more than the threshold to the left, slow the right motor
        else if (position_of_line < -threshold)
        {
            m3pi.left_motor(speed);
            m3pi.right_motor(speed-correction);
        }
 
        // Line is in the middle
        else
        {
            m3pi.forward(speed);
        }
    }
}