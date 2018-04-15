#include "mbed.h"
#include "ISL29125.h"

ISL29125 sensor(p28,p27);
Serial pc(USBTX, USBRX);
InterruptIn interrupt(p26);
InterruptIn interr(p21);
DigitalOut led(LED1);

void IntHandle()
{
    led = 1;
    pc.printf("interrupted\n");
}

int main () {
    sensor.ISL29125_Setup();
    uint16_t r, b, g;
    int result;
    interrupt.mode(PullUp);
    interrupt.fall(&IntHandle);
    interr.mode(PullUp);
    interr.fall(&IntHandle);
    
    pc.printf("%d\n", ISL29125_HIGH_THRESHOLD&0xFF);
    pc.printf("%d\t%d\n", sensor.ISL29125_getRegister(0x04), sensor.ISL29125_getRegister(0x05));
    pc.printf("%d\t%d\n", sensor.ISL29125_getRegister(0x06), sensor.ISL29125_getRegister(0x07));
    wait(2);
    
    while (true)
    {
    led = 0;
    sensor.ISL29125_getRawData(&r, &g, &b);
    result = sensor.ISL29125_getRegister(0x08);
    pc.printf("green = %d\tred = %d\tblue = %d\tint = %d\n", g, r, b, result);
    wait(0.2);
    }
return 0;
}
