// Used to test the polling functionality of the TSL2561 luminosity sensor.

#include "mbed.h"
#include "TSL2561.h"

TSL2561 sensor(p28, p27, 0x49);
Serial pc(USBTX, USBRX);
InterruptIn interrupt(p21);
DigitalOut led(LED1);

int main()
{
    uint16_t data0, data1;
    
    sensor.setPowerUp();

    while (true)
    {
        sensor.getData(&data0, &data1);
        pc.printf("%d, %d\n", data0, data1);
        wait(0.5);
    }
    return 0;
}
