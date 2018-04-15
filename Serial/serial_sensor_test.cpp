// This program was used to test the functionality of the VL6180x interrupts.
// If one of the sensors reports an interrupt, an LED on the LPC1768 lights up, and the
// sensor that interrupted is reported back via serial.
// LED1 for left interrupt, LED2 and LED 3 for centre interrupt, LED4 for right interrupt.

#include "VL6180x.h"
#include "m3pi.h"
#include "mbed.h"

m3pi m3pi;
Serial pc(USBTX, USBRX);

#define VL6180X_ADDRESS 0x29	// VL6180x resets to address 0x29 on power-off.
#define SENSORL_ADDRESS 0x30	// Each sensor must be initialised individually and have its address changed.
#define SENSORC_ADDRESS 0x31
#define SENSORR_ADDRESS 0x32
#define THRESH_L        0x60
#define THRESH_C        0xC0
#define THRESH_R        0x60

VL6180x sensorL(p28, p27, VL6180X_ADDRESS << 1);	// LPC1768 I2C uses address shifted one bit left. Last bit is R/W.
VL6180x sensorC(p28, p27, VL6180X_ADDRESS << 1);
VL6180x sensorR(p28, p27, VL6180X_ADDRESS << 1);
DigitalOut pinL(p21);
DigitalOut pinC(p22);
DigitalOut pinR(p23);
InterruptIn interruptL(p24);
InterruptIn interruptC(p25);
InterruptIn interruptR(p26);

DigitalOut leds[] = {LED1, LED2, LED3, LED4};

void handlerL()
{
    pc.printf("LEFT\n");
    leds[0] = 1; 
}

void handlerC()
{
    pc.printf("CENTRE\n");
    leds[1] = leds[2] = 1;
}

void handlerR()
{
    pc.printf("RIGHT\n");
    leds[3] = 1;
}

int main()
{
    pinL =  pinC = pinR = 0;
    
    pinL = 1;
    sensorL.VL6180x_SetupSensor(pinL, SENSORL_ADDRESS, THRESH_L);
    interruptL.mode(PullUp);
    interruptL.rise(&handlerL);
    sensorL.VL6180x_Start();
    
    pinC = 1;
    sensorC.VL6180x_SetupSensor(pinC, SENSORC_ADDRESS, THRESH_C);
    interruptC.mode(PullUp);
    interruptC.rise(&handlerC);
    sensorC.VL6180x_Start();
    
    pinR = 1;
    sensorR.VL6180x_SetupSensor(pinR, SENSORR_ADDRESS, THRESH_R);
    interruptR.mode(PullUp);
    interruptR.rise(&handlerR);
    sensorR.VL6180x_Start();
    
    while (true)
    {
        wait_ms(500);
        for (int i=0;i<=3;++i)
        	leds[i] = 0;     
        sensorL.VL6180x_ClearInterrupts();
        sensorC.VL6180x_ClearInterrupts();
        sensorR.VL6180x_ClearInterrupts();
    }
    return 0;
}
