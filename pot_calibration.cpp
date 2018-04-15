// The motor function in the m3pi class balances the speeds of the motors depending on the
// potentiometer voltage. By adjusting the potentiometer, the robot can be 'straightened'
// out. This program prints out the current potentiometer voltage while driving the robot.
// Adjust the potentiometer until the robot drives straight, then calibration is complete!

#include "m3pi.h"

m3pi m3pi;

int main()
{
    m3pi.locate(0,0);
    float pot;
    
    while (true)
    {
        m3pi.forward(0.2);	// m3pi::forward uses the pot voltage to set the motor speeds
        pot = m3pi.pot_voltage();
        m3pi.printf("%.2f", pot);
        wait(0.1);
        m3pi.cls();
    }

    return 0;
}