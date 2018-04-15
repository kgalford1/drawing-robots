// Used to test the interrupt functionality of the TSL2561 luminosity sensor.

#include "mbed.h"
#include "TSL2561.h"

TSL2561 sensor(p28, p27, 0x49);
Serial pc(USBTX, USBRX);
InterruptIn interruptLumo(p26);
DigitalOut led(LED1);

void IntHandle()
{
    led = 1;
    pc.printf("interrupted\n");
}

int main()
{
    uint16_t data0, data1;
    
    sensor.setPowerUp();
    sensor.defaultSettings(500);
    
    interruptLumo.mode(PullUp);
    interruptLumo.rise(&IntHandle);
    while (true)
    {
        sensor.getData(&data0, &data1);
        pc.printf("%d, %d\n", data0, data1);
        wait(0.5);
        sensor.clearInterrupt();
    }
    return 0;
}
