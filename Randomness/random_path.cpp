// Moves the robot randomly WITHOUT avoiding walls. The robot jolts when moving since
// the SPEED is changed directly, rather than randomising the CHANGE in speed.
// See (./smooth_random_path.cpp), which randomises the CHANGE in speed.

#include "mbed.h"
#include "m3pi.h"

m3pi m3pi;

int main()
{
    while (true)
    {
		int select0 = rand() % 2;
        int select1 = rand() % 4;
        float speed = (rand() % 15 + 5) / 100.0;
        float time = (rand() % 10 + 5) / 10;
        switch (select0)
        {    
        	case 0:
                switch (select1)
                {    
                	case 0:
                		m3pi.forward(speed);
                		break;
	                case 1:
    		            m3pi.backward(speed);
            		    break;
	                case 2:
    		            m3pi.left(speed);
            		    break;
	                case 3:
    		            m3pi.right(speed);
                }    
                
            case 1:
                switch(select1)
                {
                    case 0:
                	    m3pi.right_motor(speed);
                    	m3pi.left_motor(speed - (rand() % 10 / 100.0));
	                    break;                
                    case 1:
	                    m3pi.left_motor(speed);
    	                m3pi.right_motor(speed - (rand() % 10 / 100.0));
        	            break;        
                    case 2:
	                    m3pi.right_motor(speed);
    	                m3pi.left_motor(speed - ((rand() % 30 - 15) / 100.0));
        	            break;        
                    case 3:
	                    m3pi.left_motor(speed);
    	                m3pi.right_motor(speed - ((rand() % 30 - 15) / 100.0));
                }                     
        }
        wait(time);
    }
    return 1;
}
