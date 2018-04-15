// Allows remote control of the robots using XBee Wi-Fi modules on the m3pi and computer.
// Can change speed and direction, as well as playing progressively scarier renditions
// of the 'duh-duh' from Jaws.

#include "mbed.h"
#include "m3pi.h"
#include "ctype.h"

m3pi m3pi;
Serial device(p28, p27);

char* one = "L4 O2 T60 V2 aa#rr";
char* two = "L4 O2 T80 V4 aa#rr";
char* three = "L4 O2 T110 V7 aa#rr";
char* four = "L4 O2 T150 V12 aa#zrr";

float input;
float speed;
int dir = 0x77;

int main()
{
    m3pi.stop();
    while (true)
    {
        if(device.readable())	
			input = device.getc();
			
		if (isdigit(input))	// Detecting whether key press relates to speed...
			speed = (input - 0x30)/10.0;
		else if (input == 0x69)
		{
			if (speed<0.7)
				speed += 0.1;
		}
		else if (input == 0x6A)
		{
			if (speed>-0.7)
				speed -= 0.1;
		}
		else	// or relates to a direction/song selection.
			dir = input;

        switch (dir)	// A case for each different key press
        {
        	case 0x61 : m3pi.left(speed);
        		device.printf("Left %.2f\n", speed);
             	break;
            case 0x77 : m3pi.forward(speed);
            	device.printf("Forward %.2f\n", speed);
                break;
            case 0x64 : m3pi.right(speed);
                device.printf("Right %.2f\n", speed);
                break;
			case 0x65 :
				m3pi.right_motor(speed*2/3);
				m3pi.left_motor(speed/3);
				device.printf("Circle right %.2f\n", speed);
                break;
			case 0x71 :
				m3pi.left_motor(speed*2/3);
				m3pi.right_motor(speed/3);
				device.printf("Circle left %.2f\n", speed);
                break;
			case 0x7A :
				m3pi.playBuzzer(one);
                break;
            case 0x78 :
            	m3pi.playBuzzer(two);
                break;
            case 0x63 :
            	m3pi.playBuzzer(three);
                break;
			case 0x76 :
				m3pi.playBuzzer(four);
				break;
            }  
        }
        wait_ms(1);
    }
    return 0;
}
