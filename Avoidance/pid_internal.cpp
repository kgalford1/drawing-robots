// Kim's failed attempt at a PID algorithm. The idea was to use PID to calculate the
// change in speed, and by using an atan() to restrict the speed to be between -1.0 -> 1.0
// It worked, sort of, but kept beeping bad command errors since the speeds I was sending
// the motors were out of the above bounds. In hindsight it was not a good idea to change
// the motor speeds within the interrupts because interrupts can stack up. It is best to
// have the interrupts change a flag with no delay and have the speed change be calculated
// within the loop, as in (./pid_one_interrupt.cpp).

#include "VL6180x.h"
#include "m3pi.h"
#include "mbed.h"
#include <algorithm>

m3pi m3pi;
Serial pc(USBTX, USBRX);

#define VL6180X_ADDRESS 0x29
#define SENSORL_ADDRESS 0x30
#define SENSORC_ADDRESS 0x31
#define SENSORR_ADDRESS 0x32
#define THRESH_L        0xC0
#define THRESH_C        0xC0
#define THRESH_R        0xC0

VL6180x sensorL(p28, p27, VL6180X_ADDRESS << 1);
VL6180x sensorC(p28, p27, VL6180X_ADDRESS << 1);
VL6180x sensorR(p28, p27, VL6180X_ADDRESS << 1);
DigitalOut pinL(p21);
DigitalOut pinC(p22);
DigitalOut pinR(p23);
InterruptIn interruptL(p24);
InterruptIn interruptC(p25);
InterruptIn interruptR(p26);

DigitalOut leds[] = {LED1, LED2, LED3, LED4};

float P = 0.0001, I = 0.0, D = 0.0;
float tot_L, tot_C, tot_R, last_L, last_C, last_R;

void PID(int flag)	// 0 - turn left, 1 - turn right, 2 - centre
{	
    float left, centre, right, left_speed, right_speed, delta;
    left = sensorL.GetDistance();
    centre = sensorC.GetDistance();
    right = sensorR.GetDistance();
    left_speed = m3pi.left_speed;
    right_speed = m3pi.right_speed;
    
    switch (flag)
    {
        case 0 :
            delta = P * (THRESH_L - left) + I * tot_L + D * (last_L - left);
            left_speed -= delta;
            right_speed += delta;
            break;
        case 1 :
            delta = P * (THRESH_R - right) + I * tot_R + D * (last_R - right);
            left_speed += delta;
            right_speed -= delta;
            break;
        case 2 :
            delta = 10 * P * (THRESH_C - centre) + I * tot_C + D * (last_C - centre);
            if (left < right)
            {
                left_speed -= delta;
                right_speed += delta;
            }
            else
            {
                left_speed += delta;
                right_speed -= delta;
            }
            break;
    }
    pc.printf("Flag = %d, Delta = %.2f\n, L = %.2f, R = %.2f", flag, delta, left_speed, right_speed);
    m3pi.left_motor(left_speed);
    m3pi.right_motor(right_speed);
}

void handlerL()
{
    pc.printf("LEFT\n");
    leds[0] = 1; 
    tot_L += sensorL.GetDistance();
    PID(1);
}

void handlerC()
{
    pc.printf("CENTRE\n");
    leds[1] = leds[2] = 1;
    tot_L += sensorL.GetDistance();
    tot_R += sensorR.GetDistance();
    PID(2);
}

void handlerR()
{
    pc.printf("RIGHT\n");
    leds[3] = 1;
    tot_R += sensorR.GetDistance();
    PID(0);
}

void randPath()
{
    m3pi.left_motor(m3pi.left_speed + (rand() % 20 - m3pi.left_speed * 65) / 600);
    m3pi.right_motor(m3pi.right_speed + (rand() % 20 - m3pi.right_speed * 65) / 600);
    pc.printf("%.2f %.2f\n", m3pi.left_speed, m3pi.right_speed);
}

int main()
{
    tot_L = tot_C = tot_R = last_L = last_C = last_R = 0;
    std::srand(time(0));
    pc.printf("Random seed set.\n");
    
    pinL =  pinC = pinR = 0;
    
    pinL = 1;
    sensorL.SetupSensor(pinL, SENSORL_ADDRESS, THRESH_L);
    interruptL.mode(PullUp);
    interruptL.rise(&handlerL);
    sensorL.Start();
    
    pinC = 1;
    sensorC.SetupSensor(pinC, SENSORC_ADDRESS, THRESH_C);
    interruptC.mode(PullUp);
    interruptC.rise(&handlerC);
    sensorC.Start();
    
    pinR = 1;
    sensorR.SetupSensor(pinR, SENSORR_ADDRESS, THRESH_R);
    interruptR.mode(PullUp);
    interruptR.rise(&handlerR);
    sensorR.Start();
    pc.printf("All sensors initialised.\n");
    
    wait(1);
    m3pi.smoothStart(0.1);
    pc.printf("Loop started.\n");
    pc.printf("%.2f %.2f\n", m3pi.left_speed, m3pi.right_speed);
    
    while (true)
    {
        randPath();
        wait_ms(50);
        for (int i=0;i<=3;++i)
            leds[i] = 0;     
        sensorL.ClearInterrupts();
        sensorC.ClearInterrupts();
        sensorR.ClearInterrupts(); 
        tot_L = tot_C = tot_R = last_L = last_C = last_R = 0;
    }
    return 0;
}
