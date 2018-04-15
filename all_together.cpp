// Reports back data via Serial of all sensors. Used to confirm that all sensors are
// working as expected.

#include "VL6180x.h"
#include "ISL29125.h"
#include "TSL2561.h"
#include "m3pi.h"
#include "mbed.h"

m3pi m3pi;
Serial pc(USBTX, USBRX);

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
bool prox_interrupted = false, lumo_interrupted = false;

void interruptProxHandle()
{
    if (not prox_interrupted)
    {
        c_prox_distance = proxC.GetDistance();
        l_prox_distance = proxL.GetDistance();    
        r_prox_distance = proxR.GetDistance();

        if (c_prox_distance < l_prox_distance)
        {
            if (c_prox_distance < r_prox_distance)
                pc.printf("\nCentre proximity sensor interrupted\n\n");
            else 
                pc.printf("\nRight proximity sensor interrupted\n\n");
        }
        else
        {
            if (l_prox_distance < r_prox_distance)
                pc.printf("\nLeft proximity sensor interrupted\n\n");
            else 
                pc.printf("\nRight proximity sensor interrupted\n\n");
        }
        prox_interrupted = true;
    }
}

void interruptRGBHandle()
{
    pc.printf("\nRGB interrupted\n");
}

void interruptLumoHandle()
{
    if (not lumo_interrupted)
    {
        lumoL.getVisData(&lumoL_val);
        lumoR.getVisData(&lumoR_val);
        if (lumoL_val > lumoR_val)
            pc.printf("\nLeft luminosity triggered\n");
        else
            pc.printf("\nRight luminosity triggered\n");
    lumo_interrupted = true;
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

    
    sensorRGB.Setup();
    interruptRGB.mode(PullUp);
    interruptRGB.fall(&interruptRGBHandle);
    
    lumoL.setPowerUp();
    lumoL.defaultSettings(500);
    interruptLumoL.mode(PullUp);
    interruptLumoL.rise(&interruptLumoHandle);
    
    lumoR.setPowerUp();
    lumoR.defaultSettings(500);
    interruptLumoR.mode(PullUp);
    interruptLumoR.rise(&interruptLumoHandle);
    
    wait_ms(100);
    //================================

    uint16_t dataProxL, dataProxC, dataProxR, dataR, dataG, dataB, dataLumoL0, dataLumoL1, dataLumoR0, dataLumoR1;

    while(1)
    {
        wait_ms(500);
        dataProxL = proxL.GetDistance();
        dataProxC = proxC.GetDistance();
        dataProxR = proxR.GetDistance();
        
        sensorRGB.getRawData(&dataR, &dataG, &dataB);
        
        lumoL.getData(&dataLumoL0, &dataLumoL1);
        lumoR.getData(&dataLumoR0, &dataLumoR1);
        
        pc.printf("ProxL = %d\tProxC = %d\tProxR = %d\nR = %d\tG = %d\tB = %d\nLumoL = %d, %d\tLumoR = %d, %d\n\n", dataProxL, dataProxC, dataProxR, dataR, dataG, dataB, dataLumoL0, dataLumoL1, dataLumoR0, dataLumoR1);

        prox_interrupted = false;
        lumo_interrupted = false;
        proxR.ClearInterrupts();
        proxC.ClearInterrupts();
        proxL.ClearInterrupts();
        sensorRGB.ClearInterrupts();
        lumoL.clearInterrupt();
        lumoR.clearInterrupt();
        
    }
}
