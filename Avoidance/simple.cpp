// An initial attempt at a non-PID wall avoidance algorithm. The lack of an aggression
// setting made it struggle in getting out of walls. When the centre sensor triggered
// it stops and turns around. Very much an early sketch and proof of concept.

#include "VL6180x.h"
#include "m3pi.h"
#include "mbed.h"

m3pi m3pi;

#define VL6180X_ADDRESS 0x29
#define SENSORL_ADDRESS 0x30
#define SENSORC_ADDRESS 0x31
#define SENSORR_ADDRESS 0x32
VL6180x sensorL(p28, p27, ADDRESS << 1);
VL6180x sensorC(p28, p27, ADDRESS << 1);
VL6180x sensorR(p28, p27, ADDRESS << 1);

DigitalOut leds[] = {LED1, LED2, LED3, LED4};
DigitalOut pinL(p21);
DigitalOut pinC(p22);
DigitalOut pinR(p23);
InterruptIn interruptL(p24);
InterruptIn interruptC(p25);
InterruptIn interruptR(p26);

bool flag = true;
float l_wheel_speed = 0.15;
float r_wheel_speed = 0.15;

void interruptHandlerL()
{
    leds[0] = 1;
    r_wheel_speed += 0.05 - (sensorR.GetDistance() / 10000);    
}

void interruptHandlerC()
{
	int rand_direction;
	rand_direction = rand() % 2;
    leds[1] = leds[2] = 1;
    l_wheel_speed = 0.15;
    r_wheel_speed = 0.15;
    
    m3pi.smoothStop(0.15);
    
    if (rand_direction == 0)
		m3pi.right(0.15);
    else
		m3pi.left(0.15);
    wait(0.4);
}

void interruptHandlerR()
{
    leds[3] = 1;
    l_wheel_speed += 0.05 - (sensorL.GetDistance() / 10000);
}

int main()
{
    srand (time(NULL));
    pinL = 0;
    pinC = 0;
    pinR = 0;
    
    pinL = 1;
    sensorL.SetupSensor(pinL, SENSORL_ADDRESS, 0xA0); 
    interruptL.mode(PullUp);
    interruptL.rise(&interruptHandlerL);
    sensorL.Start();
    
    pinC = 1;
    sensorC.SetupSensor(pinC, SENSORC_ADDRESS, 0x50); 
    interruptC.mode(PullUp);
    interruptC.rise(&interruptHandlerC);
    sensorC.Start();

    pinR = 1;
    sensorR.SetupSensor(pinR, SENSORR_ADDRESS, 0xA0); 
    interruptR.mode(PullUp);
    interruptR.rise(&interruptHandlerR);
    sensorR.Start();
    
    wait_ms(100);
    
    while(1)
    {
    	l_wheel_speed += (rand() % 100 - l_wheel_speed * 200) / 1400;
    	r_wheel_speed += (rand() % 100 - r_wheel_speed * 200) / 1400;
    	wait_ms(100);
    	
    	if (l_wheel_speed > 0.15)
            l_wheel_speed -= 0.01;
        else if (l_wheel_speed < 0.15)
            l_wheel_speed += 0.01;

        if (r_wheel_speed > 0.15) 
            r_wheel_speed -= 0.01;
        else if (r_wheel_speed < 0.15)
            r_wheel_speed += 0.01;

        m3pi.left_motor(l_wheel_speed);  
        m3pi.right_motor(r_wheel_speed);
        
        for (int i=0;i<=3;++i)
            leds[i] = 0;
        sensorL.ClearInterrupts();
        sensorC.ClearInterrupts();
        sensorR.ClearInterrupts();
    }
    return 0;
}
