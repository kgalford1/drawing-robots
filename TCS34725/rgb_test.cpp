// Serial prints the RGBC values twice a second. Used to test the correct implementation
// of the sensor using the RGB LED strip (/WS2811/rgb_cycle.cpp)

#include "mbed.h"
#include "TCS34725.h"

Serial pc(USBTX, USBRX);
TCS34725 sensor(p9, p10);

int main()
{
    sensor.TCS34725_Setup();
   	uint16_t r, g, b, c;
   	while(true)
   	{
   		sensor.TCS34725_GetRawData(&r, &g, &b, &c);
   		pc.printf("R = %d, G = %d, B = %d, C = %d\n", r, g, b, c);
   		wait(0.5);
   	}
    return 0;
}
