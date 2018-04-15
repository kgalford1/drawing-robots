// An attempt to test the interrupt functionality of the TCS34725 which was unsuccessful.
// An described in the wiki, we couldn't get the interrupt of the TCS34725 to work at all.

#include "mbed.h"
#include "TCS34725.h"

TCS34725 sensor(p9, p10);
Serial pc(USBTX, USBRX);
InterruptIn interrupt(p8);
DigitalOut led(LED1);

void handler()
{
    led = 0;
    sensor.TCS34725_ClearInterrupt();
}

int main()
{
    led = 1;
    uint16_t r, g, b, c;
    interrupt.mode(PullUp);
    interrupt.fall(&handler);
    
    sensor.TCS34725_Setup();
    sensor.TCS34725_SetInterruptLimits(0x0400, 0x0200);
    sensor.TCS34725_SetInterrupt(true);
    
    while (true)
    {
        led = 1;
        sensor.TCS34725_GetRawData(&r, &g, &b, &c);
        pc.printf("R = %d, G = %d, B = %d, C = %d\n", r, g, b, c);
        wait(0.2);
    }

    return 0;
}
