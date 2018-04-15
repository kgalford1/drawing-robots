// Cycles through the colours red, green, and blue. Structure of this file may seem
// unusual since it has been edited from https://developer.mbed.org/users/aswild/code/NeoStrip/
// to simply cycle through colours rather than have more complicated patterns, and the
// ability to change brightness.

#include "mbed.h"
#include "NeoStrip.h"
#include <stdlib.h>

#define N 16
#define PATTERNS 3

int hueToRGB(float h);
void pattern0();
void pattern1();
void pattern2();

void (*patterns[])(void) = {&pattern0, &pattern1, &pattern2};

NeoStrip strip(p18, N);
DigitalIn b3(p21);	// Next pattern

int main()
{
	b3.mode(PullUp);
	
	int pattern = 0;
	float bright = 0.5;	// Brightness 0.0 -> 1.0
	bool b3o = b3;	// Stores old pattern setting
	bool changed = true;

	strip.setBrightness(bright);
	
	while (true)
	{	
		if (b3 != b3o)
		{
			if (b3 && ++pattern == PATTERNS)
				pattern = 0;
			b3o = b3;
			changed = true;
		}
		
		patterns[pattern]();
		if (changed) 
			strip.write();
		if (pattern == 0)
			changed = false;		
		wait_ms(10);
	}
}

void pattern0()
{
	strip.setColor(0xFF0000);
}

void pattern1()
{
	strip.setColor(0x00FF00);
}

void pattern2()
{
	strip.setColor(0x0000FF);
}
