#include <stdio.h>
#include "sensorlib.h" //This is the Modular Sensor Library we are using to analyze analog sensors

int main() 
{
	analog_sensor top_hat = build_analog_sensor(7, 0, 3, 0);
	//Build an 'analog_sensor' named 'top_hat'
	//It is in port #7, it is not floating (0), average it using 3 trials, latency of 0 ms	
	
	analog_event black = build_analog_event(900, 90);
	//Build an 'analog_event' named 'black'
	//Its default value is 900, it has an error threshold of 90
	
	while(!analog_state(top_hat, black)) //While the 'analog_state' of 'top_hat' being 'black' is 0
	{
		printf("%f\n", analog_change_rate(top_hat)); //Print the rate of change of 'top_hat'
	}
	
	return 0; //Return 0 to the system
}
