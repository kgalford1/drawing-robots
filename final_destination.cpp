// Our final, albiet untuned, product. The structure is all here, however the sensor
// setting need some tuning so that the robot consistently follows the light.

#include "VL6180x.h"
#include "ISL29125.h"
#include "TSL2561.h"
#include "m3pi.h"
#include "mbed.h"

m3pi m3pi;
Serial pc(USBTX, USBRX);

//======Operation Variables=======
float max_speed = 0.22;
float light_turning_aggressiveness = 1;
float prox_turning_aggressiveness = 1;
float centre_turning_aggressiveness = 3;
//===============================

#define VL6180X_ADDRESS 0x29
#define PROXL_ADDRESS 0x30
#define PROXC_ADDRESS 0x31
#define PROXR_ADDRESS 0x32

#define LUMOR_ADDRESS   0x39
#define LUMOL_ADDRESS   0x49

#define RBG_ADDRESS     0x88

VL6180x proxL(p28, p27, VL6180X_ADDRESS << 1);
VL6180x proxC(p28, p27, VL6180X_ADDRESS << 1);
VL6180x proxR(p28, p27, VL6180X_ADDRESS << 1);

ISL29125 sensorRGB(p28,p27);

TSL2561 lumoL(p28, p27, LUMOL_ADDRESS);
TSL2561 lumoR(p28, p27, LUMOR_ADDRESS);

DigitalOut leds[] = {LED1, LED2, LED3, LED4};
DigitalOut pinProxL(p21);
DigitalOut pinProxC(p23);
DigitalOut pinProxR(p25);

InterruptIn interruptProxL(p22);
InterruptIn interruptProxC(p24);
InterruptIn interruptProxR(p26);

InterruptIn interruptRGB(p17);

InterruptIn interruptLumoL(p16);
InterruptIn interruptLumoR(p15);

uint16_t lumoL_val, lumoR_val;

float l_prox_distance, r_prox_distance, c_prox_distance;
bool prox_interrupted = false, lumo_interrupted = false, RGB_interrupted;
int prox_state = 0, light_state = 0;
float prox_correction_factor = 0;

void interruptProxHandle()
{
    if (not prox_interrupted)
    {
        prox_interrupted = true;
        c_prox_distance = proxC.GetDistance();
        l_prox_distance = proxL.GetDistance();    
        r_prox_distance = proxR.GetDistance();

        if (c_prox_distance < 130)
        {
            prox_correction_factor = prox_turning_aggressiveness * centre_turning_aggressiveness;
        
            if (c_prox_distance < 50)
                {
                prox_state = 3;
                leds[2] = 1;
                return;
                }
        }
        else
            prox_correction_factor = prox_turning_aggressiveness;   

        if (l_prox_distance > r_prox_distance)
        {
            prox_state = 2;
        }
        else if (r_prox_distance > l_prox_distance)
        {
            prox_state = 1; 
        } 
    
    }
}

void interruptRGBHandle()
{
    RGB_interrupted = true;
    leds[0] = 1;
}

void interruptLumoHandle()
{
    if (not lumo_interrupted)
    {
        lumo_interrupted = true;
        lumoL.getVisData(&lumoL_val);
        lumoR.getVisData(&lumoR_val);
        if (abs(lumoL_val - lumoR_val) > 25 and lumoL_val > lumoR_val)
            light_state = 1;
        else if (abs(lumoL_val - lumoR_val) > 25 and lumoL_val < lumoR_val)
            light_state = 2;
        else
            light_state = 0;
    }
}

int main()
{
    pinProxL = 0;
    pinProxC = 0;
    pinProxR = 0;

    //====Initialise sensors======
    pinProxL = 1;
    proxL.SetupSensor(pinProxL, PROXL_ADDRESS, 0xC0);    
    interruptProxL.mode(PullUp);
    interruptProxL.rise(&interruptProxHandle);
    proxL.Start();

    pinProxC = 1;
    proxC.SetupSensor(pinProxC, PROXC_ADDRESS, 0xC0);    
    interruptProxC.mode(PullUp);
    interruptProxC.rise(&interruptProxHandle);
    proxC.Start();

    pinProxR = 1;
    proxR.SetupSensor(pinProxR, PROXR_ADDRESS, 0xC0);    
    interruptProxR.mode(PullUp);
    interruptProxR.rise(&interruptProxHandle);
    proxR.Start();
 
    sensorRGB.Setup();  // interrupts on red by default
    interruptRGB.mode(PullUp);
    interruptRGB.fall(&interruptRGBHandle);
    
    lumoL.setPowerUp();
    lumoL.defaultSettings(300);
    interruptLumoL.mode(PullUp);
    interruptLumoL.rise(&interruptLumoHandle);
    
    lumoR.setPowerUp();
    lumoR.defaultSettings(300);
    interruptLumoR.mode(PullUp);
    interruptLumoR.rise(&interruptLumoHandle);
    
    wait_ms(100);
    
    //================================
    
    float r_wheel_speed = max_speed / 2;
    float l_wheel_speed = max_speed / 2;
    m3pi.smoothStart(max_speed / 2);
    wait(1);

    while(1)
    {
        wait_ms(70); // allow time for interrupts to occur

        switch (prox_state)
        {
            case 0: // general case (no interrupts)
                if (abs(r_wheel_speed - max_speed / 2) > max_speed / 9) // if one wheel has a 'large' speed return to average
                    r_wheel_speed -= (max_speed / 2 - l_wheel_speed) / 4;
                    
                else if (RGB_interrupted)   // else check for and attempt to follow light
                {   
                    switch (light_state)
                    {
                        case 0: // no lumo bias
                            r_wheel_speed -= (r_wheel_speed - max_speed/2);  //straighten out
                            break;
                        
                        case 1: // left brighter - turn left
                            if (l_wheel_speed < r_wheel_speed)
                                r_wheel_speed -= 0.1 * light_turning_aggressiveness;
                            break;
                        
                        case 2: // right brighter - turn right
                            if (l_wheel_speed > r_wheel_speed)
                                r_wheel_speed += 0.1 * light_turning_aggressiveness;
                            break;
                    }
                }
                else    //else random 
                    r_wheel_speed += (rand() % 10 - r_wheel_speed * (8.5 / max_speed)) / (125 / max_speed);

                // keep condition that r_wheel_speed + l_wheel_speed = max_speed
                l_wheel_speed = max_speed - r_wheel_speed;
                break;
                
        // left turn
            case 1: 
                r_wheel_speed += prox_correction_factor / 100;
                l_wheel_speed = max_speed - r_wheel_speed;
                break;
        
        // right turn
            case 2: 
                l_wheel_speed += prox_correction_factor / 100;
                r_wheel_speed = max_speed - l_wheel_speed;
                break;

        // centre too close
            case 3:
                l_wheel_speed += prox_correction_factor / 70;
                r_wheel_speed = max_speed - l_wheel_speed;
                break;
        }
    
        // print speeds on m3pi
            m3pi.cls();
            m3pi.locate(0, 1);
            m3pi.printf("L %2.2f", l_wheel_speed);
            m3pi.locate(0, 0);
            m3pi.printf("R %2.2f", r_wheel_speed);

        // update motor speeds
        //m3pi.right_motor(r_wheel_speed);
        //m3pi.left_motor(l_wheel_speed);
    
        // clear interrupts/LEDs
        for (int i=0;i<=3;++i)
            leds[i] = 0;
            
        prox_state = 0;
        prox_interrupted = false;
        lumo_interrupted = false;
        RGB_interrupted = false;
        proxR.ClearInterrupts();
        proxC.ClearInterrupts();
        proxL.ClearInterrupts();
        sensorRGB.ClearInterrupts();
        lumoL.clearInterrupt();
        lumoR.clearInterrupt();
    }
}
