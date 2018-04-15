// A first attempt at a PID avoidance algorithm with a separate interrupt for each sensor.

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

float l_wheel_speed = 0.15;
float r_wheel_speed = 0.15;
float l_sensor_distance;
float r_sensor_distance;

int l_interrupt_count = 0;
int c_interrupt_count = 0;
int r_interrupt_count = 0;

float proportion = 0.022;
float intergral = 0.015;

void interruptHandlerL()
{
    leds[0] = 1;
    l_wheel_speed -= 0.02;
    r_wheel_speed += 0.04;
}

void interruptHandlerC()
{
    leds[1] = leds[2] = 1;
    l_sensor_distance = sensorR.GetDistance();
    r_sensor_distance = sensorL.GetDistance();
    
    if (l_sensor_distance < 150)
    {
        ++l_interrupt_count;
        l_wheel_speed += (proportion - l_sensor_distance / (150 / proportion));
        l_wheel_speed += intergral * l_interrupt_count;
    }
    else 
        l_interrupt_count = 0;

    if (r_sensor_distance < 150)
    {
        ++r_interrupt_count;
        r_wheel_speed += (proportion - r_sensor_distance / (150 / proportion));
        r_wheel_speed += intergral * r_interrupt_count;
    }
    else 
        r_interrupt_count = 0;
}

void interruptHandlerR()
{
    leds[3] = 1;
    l_wheel_speed += 0.04;
    r_wheel_speed -= 0.02;
}

int main()
{
    srand (time(NULL));
    pinL = 0;
    pinC = 0;
    pinR = 0;
    
    pinL = 1;
    sensorL.SetupSensor(pinL, SENSORL_ADDRESS, 0x60); 
    interruptL.mode(PullUp);
    interruptL.rise(&interruptHandlerL);
    sensorL.Start();
    
    pinC = 1;
    sensorC.SetupSensor(pinC, SENSORC_ADDRESS, 0xC0); 
    interruptC.mode(PullUp);
    interruptC.rise(&interruptHandlerC);
    sensorC.Start();

	pinR = 1;
	sensorR.SetupSensor(pinR, SENSORR_ADDRESS, 0x60);
	interruptR.mode(PullUp);
	interruptR.rise(&interruptHandlerR);
	sensorR.Start();

	wait_ms(100);

	while(true)
	{
		l_wheel_speed += (rand()%20 - l_wheel_speed*65)/600;
		r_wheel_speed += (rand()%20 - r_wheel_speed*65)/600;
		wait_ms(60);
		
		if (r_wheel_speed > 0.18) 
			r_wheel_speed -= 0.015;
		else if (r_wheel_speed < 0.15)
			r_wheel_speed += 0.01;
			
		if (l_wheel_speed > 0.18)
			l_wheel_speed -= 0.015;
		else if (l_wheel_speed < 0.15)
			l_wheel_speed += 0.01;

		m3pi.left_motor(l_wheel_speed);
		m3pi.right_motor(r_wheel_speed);
		   
		m3pi.cls();
		m3pi.locate(0, 1);
		m3pi.printf("%2.2f",l_wheel_speed);
		m3pi.locate(0, 0);
		m3pi.printf("%2.2f",r_wheel_speed); 

		for (int i = 0; i <= 3; ++i)
			leds[i] = 0;

		sensorL.ClearInterrupts();
		sensorC.ClearInterrupts();
		sensorR.ClearInterrupts();  
	}
	return 0;
}
