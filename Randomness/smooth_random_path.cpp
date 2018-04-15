// Moves the robot randomly WITHOUT avoiding walls. The robot moves smoothly since
// the CHANGED in speed is randomised.
// See (./random_path.cpp), which randomises the SPEED directly, resulting in jolty movement.

#include "mbed.h"
#include "m3pi.h"
#include "time.h"
#include "cmath"

m3pi m3pi;
DigitalOut leds[] = {LED1, LED2, LED3, LED4};	// LPC1768 LED Array
 
int main()
{    
	float r_wheel_speed = 0;
    float l_wheel_speed = 0;
    float l_wheel_change = 0;
    float r_wheel_change = 0;
    srand (time(NULL));
    
    while(true)
	{
		m3pi.cls();
		m3pi.locate(0, 1);
		m3pi.printf("%2.2f", r_wheel_speed);
		m3pi.locate(0, 0);
		m3pi.printf("%2.3f", l_wheel_speed);
		wait(0.1);
	
		////////////////////////////////////////////
		// COMPUTE CHANGES IN SPEED
		////////////////////////////////////////////
		r_wheel_change = ((rand() % 40) - (r_wheel_speed * 90 + 2)) / 1800.0;
		l_wheel_change = ((rand() % 40) - (l_wheel_speed * 90 + 2)) / 1800.0;

		if (r_wheel_speed>0.4)
		{
			r_wheel_change = -abs(r_wheel_change);
		}
		else if (r_wheel_speed < 0.05)
		{
			r_wheel_change = abs(r_wheel_change);
		}
	
		if (l_wheel_speed>0.4)
		{
			l_wheel_change = -abs(l_wheel_change);
		}
		else if (l_wheel_speed < 0.05)
		{
			l_wheel_change = abs(l_wheel_change);
		}

		////////////////////////////////////////////
	 
		for (int i = 0; i < 5 ; i++)
		{
			r_wheel_speed += r_wheel_change;
			l_wheel_speed += l_wheel_change;
			m3pi.right_motor(r_wheel_speed);
			m3pi.left_motor(l_wheel_speed);
			wait(0.1);
		}
	}
	return 0;
}
