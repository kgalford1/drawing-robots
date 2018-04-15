// pid_one_interrupt.cpp with an additional light interrupt.

#include "VL6180x.h"
#include "ISL29125.h"
#include "m3pi.h"
#include "mbed.h"

m3pi m3pi;

//======Operation Variables=======
float max_speed = 0.27;
float turning_aggressiveness = 1;
float centre_turning_aggressiveness = 3;
//===============================

#define VL6180X_ADDRESS 0x29
#define SENSORL_ADDRESS 0x30
#define SENSORC_ADDRESS 0x31
#define SENSORR_ADDRESS 0x32
VL6180x sensorL(p28, p27, VL6180X_ADDRESS << 1);
VL6180x sensorC(p28, p27, VL6180X_ADDRESS << 1);
VL6180x sensorR(p28, p27, VL6180X_ADDRESS << 1);

ISL29125 sensorLight(p28,p27);

DigitalOut leds[] = {LED1, LED2, LED3, LED4};

DigitalOut pinL(p21);
DigitalOut pinC(p23);
DigitalOut pinR(p25);
InterruptIn interruptL(p22);
InterruptIn interruptC(p24);
InterruptIn interruptR(p26);

InterruptIn interruptLight(p17);
float l_sensor_distance;
float r_sensor_distance;
float c_sensor_distance;
int state = 0;
float correction_factor = 0;
int light_trigger = 0;

void interruptHandler()
{        
    l_sensor_distance = sensorL.GetDistance();
    c_sensor_distance = sensorC.GetDistance();    
    r_sensor_distance = sensorR.GetDistance();
    pc.printf("L = %d, C = %d, R = %d\n", l_sensor_distance, c_sensor_distance, r_sensor_distance);

    if (c_sensor_distance < 130)
    {
        leds[1] = 1;
        correction_factor = turning_aggressiveness * centre_turning_aggressiveness;
        
        if (c_sensor_distance < 50)
        {
            state = 3;
            leds[2] = 1;
            return;
        }
    }
    else 
        correction_factor = turning_aggressiveness;   

    if (l_sensor_distance >= r_sensor_distance && r_sensor_distance != 255)
    {
        state = 2;
        leds[3] = 1;
    }
    else if (r_sensor_distance > l_sensor_distance)
    {
        state = 1; 
        leds[0] = 1;
    }
}

void light_interrupt()
{
    light_trigger = 1;
    leds[2] = 1;
}

int main()
{
    srand (time(NULL));
    pinL = 0;
    pinC = 0;
    pinR = 0;

    //====Initialise sensors======
    pinL = 1;
    sensorL.SetupSensor(pinL, SENSORL_ADDRESS, 0xC0);    
    interruptL.mode(PullUp);
    interruptL.rise(&interruptHandler);
    sensorL.Start();

    pinC = 1;
    sensorC.SetupSensor(pinC, SENSORC_ADDRESS, 0xC0);    
    interruptC.mode(PullUp);
    interruptC.rise(&interruptHandler);
    sensorC.Start();

    pinR = 1;
    sensorR.SetupSensor(pinR, SENSORR_ADDRESS, 0xC0);    
    interruptR.mode(PullUp);
    interruptR.rise(&interruptHandler);
    sensorR.Start();

    
    sensorLight.ISL29125_Setup();
    interruptLight.mode(PullUp);
    interruptLight.fall(&light_interrupt);
    
    wait_ms(100);
    //================================


    float r_wheel_speed = max_speed / 2;
    float l_wheel_speed = max_speed / 2;
    m3pi.smoothStart(max_speed / 2);
    wait(1);

    while(1)
    {
        wait_ms(90); // allow time for interrupts to occur

        switch (state)
        {
            case 0: // general case (no interrupts)
                if (light_trigger and max_speed > 0.07)
                    max_speed -= 0.01;
                else if (max_speed < 0.3)
                    max_speed += 0.02;
            
                if (abs(r_wheel_speed - max_speed / 2) > max_speed / 9) // if one wheel has a 'large' speed return to average
                    r_wheel_speed -= (max_speed / 2 - l_wheel_speed) / 4;
                else    // else generate a random speed change to apply
                    r_wheel_speed += (rand() % 10 - r_wheel_speed * (8 / max_speed)) / (120 / max_speed);

                // keep condition that r_wheel_speed + l_wheel_speed = max_speed
                l_wheel_speed = max_speed - r_wheel_speed;
                break;
                
        // left turn
            case 1: 
                r_wheel_speed += correction_factor / 100;
                l_wheel_speed = max_speed - r_wheel_speed;
                break;
        
        // right turn
            case 2: 
                l_wheel_speed += correction_factor / 100;
                r_wheel_speed = max_speed - l_wheel_speed;

        // centre too close
            case 3:
                l_wheel_speed += correction_factor / 70;
                r_wheel_speed = max_speed - l_wheel_speed;
        }
    
        // print speeds on m3pi
            m3pi.cls();
            m3pi.locate(0, 1);
            m3pi.printf("%2.2f", l_wheel_speed);
            m3pi.locate(0, 0);
            m3pi.printf("%2.2f", r_wheel_speed);

        // update motor speeds
        m3pi.right_motor(r_wheel_speed);
        m3pi.left_motor(l_wheel_speed);
    
        // clear interrupts/LEDs
        for (int i=0;i<=3;++i)
            leds[i] = 0;
            
        state = 0;
        light_trigger = 0;
        sensorR.ClearInterrupts();
        sensorC.ClearInterrupts();
        sensorL.ClearInterrupts();
        sensorLight.ISL29125_ClearInterrupts();
    }
}
