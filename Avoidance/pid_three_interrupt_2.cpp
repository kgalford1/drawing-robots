// A second attempt at a PID avoidance algorithm with a separate interrupt for each sensor.

#include "VL6180x.h"
#include "m3pi.h"
#include "mbed.h"

m3pi m3pi;

#define VL6180X_ADDRESS 0x29
#define SENSORL_ADDRESS 0x30
#define SENSORC_ADDRESS 0x31
#define SENSORR_ADDRESS 0x32
VL6180x sensorL(p28, p27, VL6180X_ADDRESS << 1);
VL6180x sensorC(p28, p27, VL6180X_ADDRESS << 1);
VL6180x sensorR(p28, p27, VL6180X_ADDRESS << 1);

DigitalOut leds[] = {LED1, LED2, LED3, LED4};
DigitalOut pinL(p21);
DigitalOut pinC(p22);
DigitalOut pinR(p23);
InterruptIn interruptL(p24);
InterruptIn interruptC(p25);
InterruptIn interruptR(p26);

float l_sensor_distance;
float r_sensor_distance;

int l_interrupt_count = 0;
int c_interrupt_count = 0;
int r_interrupt_count = 0;

bool l_interrupted = false;
bool c_interrupted = false;
bool r_interrupted = false;

float rand_speed = 0;

float proportion = 0.022;
float intergral = 0.015;

void interruptHandlerL()
{
    leds[0] = 1;
    l_interrupted = true;
    
}

void interruptHandlerC()
{
	leds[1] = leds[2] = 1;
	c_interrupted = true;
}

void interruptHandlerR()
{
    leds[3] = 1;
    r_interrupted = true;
}

int main()
{
	srand (time(NULL));
	pinL = 0;
    pinC = 0;
    pinR = 0;
    
	pinL = 1;
	sensorL.SetupSensor(pinL, SENSORL_ADDRESS, 0xC0); 
	interruptL.mode(PullUp);
	interruptL.rise(&interruptHandlerL);
	sensorL.Start();

	pinC = 1;
	sensorC.SetupSensor(pinC, SENSORC_ADDRESS, 0xB0); 
	interruptC.mode(PullUp);
	interruptC.rise(&interruptHandlerC);
	sensorC.Start();

	pinR = 1;
	sensorR.SetupSensor(pinR, SENSORR_ADDRESS, 0xC0); 
	interruptR.mode(PullUp);
	interruptR.rise(&interruptHandlerR);
	sensorR.Start();

	wait_ms(100);

    float max_speed = 0.27;
    float l_wheel_speed = max_speed / 2;
    float r_wheel_speed = max_speed / 2;
    m3pi.smoothStart(max_speed / 2);
    
    while(1)
    {    
		wait_ms(150);

		if ((!r_interrupted and !c_interrupted) and !l_interrupted)
		{
			rand_speed = ((rand() % 5 - r_wheel_speed * 13.3) / 150);
			r_wheel_speed += rand_speed;
			l_wheel_speed = max_speed - r_wheel_speed;
		
			if (r_wheel_speed > 0.2)
			{
				r_wheel_speed -= 0.015;
				l_wheel_speed = max_speed - r_wheel_speed;
			}
			else if (l_wheel_speed > 0.2)
			{
				r_wheel_speed += 0.015;
				l_wheel_speed = max_speed - r_wheel_speed;
			}
			m3pi.leds(255);
		}
		else if ((r_interrupted and !c_interrupted) and !l_interrupted)
		{
				r_wheel_speed += 0.03;
				l_wheel_speed = max_speed - r_wheel_speed;
		}
		else if ((r_interrupted and !c_interrupted) and l_interrupted)
		{
			r_sensor_distance = sensorR.GetDistance();
			l_sensor_distance = sensorL.GetDistance();
			if (l_sensor_distance > r_sensor_distance)
				r_wheel_speed += -0.02;
			else
				r_wheel_speed += 0.02;
			l_wheel_speed = max_speed - r_wheel_speed;
		}
		else if ((!r_interrupted and !c_interrupted) and l_interrupted)
		{
				r_wheel_speed -= 0.03;
				l_wheel_speed = max_speed - r_wheel_speed;
		}
		else if ((!r_interrupted and c_interrupted) and !l_interrupted)
		{
			r_sensor_distance = sensorR.GetDistance();
			l_sensor_distance = sensorL.GetDistance();
			if (l_sensor_distance > r_sensor_distance)
				r_wheel_speed += -0.02;
			else if (l_sensor_distance < r_sensor_distance)
				r_wheel_speed += 0.02;
			else
			{
				rand_speed = ((rand()%5-r_wheel_speed*13.3)/100);
				r_wheel_speed += rand_speed;
				l_wheel_speed = max_speed - r_wheel_speed;
				l_wheel_speed = max_speed - r_wheel_speed;
			}
		}
		else if ((r_interrupted and c_interrupted) and l_interrupted)
		{
			r_sensor_distance = sensorR.GetDistance();
			l_sensor_distance = sensorL.GetDistance();
			if (l_sensor_distance > r_sensor_distance)
				r_wheel_speed += -0.06;
			else
				r_wheel_speed += 0.06;
			l_wheel_speed = max_speed - r_wheel_speed;
			m3pi.leds(31);
		}
		else if ((!r_interrupted and c_interrupted) and l_interrupted)
		{
			r_wheel_speed -= 0.05;
			l_wheel_speed = max_speed - r_wheel_speed;
		}
		else if ((r_interrupted and c_interrupted) and !l_interrupted)
		{
			r_wheel_speed += 0.05;
			l_wheel_speed = max_speed - r_wheel_speed;
		}

		if (r_wheel_speed > max_speed) 
		{
			l_wheel_speed += (r_wheel_speed-max_speed);
			r_wheel_speed = max_speed;
		}
		else if (l_wheel_speed > max_speed)
		{
			r_wheel_speed += (l_wheel_speed-max_speed);
			l_wheel_speed = max_speed;
		}
		
		m3pi.cls();
		m3pi.locate(0,1);
		m3pi.printf("%2.2f",l_wheel_speed);
		m3pi.locate(0,0);
		m3pi.printf("%2.2f",r_wheel_speed);
		
		m3pi.right_motor(l_wheel_speed);
		m3pi.left_motor(r_wheel_speed);

		for (int i=0;i<=3;++i)
			leds[i] = 0;
		m3pi.leds(0);

		r_interrupted = false;
		c_interrupted = false;
		l_interrupted = false;
		sensorR.ClearInterrupts();
		sensorC.ClearInterrupts();
		sensorL.ClearInterrupts();	
	}
}
