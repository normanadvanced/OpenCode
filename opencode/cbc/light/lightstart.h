#ifndef __LIGHTSTART_H__
#define __LIGHTSTART_H__

#include "../servo/servolib.h"

void lightstart(int port, float kill_time)
{
	int i;
	int light_off = -1;
	int light_on = -1;

	//set_analog_floats(0); //revmoved for new function name in libkovan
	printf("A Button :\tCalibrate for Light Start in Port #%d\n", port);
	printf("B Button :\tRun Now!\n");
	while(1)
	{
		if(a_button())
		{
			printf("\nPlease Turn Light On and Off\n");
			while(light_on == -1 || light_off == -1)
			{
				if(analog10(port) > 512 && light_off == -1)
				{
					printf("Calibrating Off Value:");
					while(light_off == -1)
					{
						light_off = 0;
						for(i = 0; i < 3; i++)
						{
							light_off += analog10(port);
							msleep(250L);
						}
						light_off = light_off / 3;
						if(light_off < 512)
						{
							light_off = -1;
						}
					}
					printf("\t%d\n", light_off);
				}
				if(analog10(port) < 512 && light_on == -1)
				{
					printf("Calibrating On Value:");
					while(light_on == -1)
					{
						light_on = 0;
						for(i = 0; i < 3; i++)
						{
							light_on += analog10(port);
							msleep(250L);
						}
						light_on = light_on / 3;
						if(light_on > 512)
						{
							light_on = -1;
						}
					}
					printf("\t%d\n", light_on);
				}
				if(light_on < 512 && light_off > 512 && (light_off - light_on) > 100 && light_on != -1 && light_off != -1)
				{
					printf("\nCalibrated!\nOn: %d\nOff: %d\nDifference: %d\n\nPress A to Proceed\nPress B to Redo\n", light_on, light_off, light_off-light_on);
					while(1)
					{
						if(a_button())
						{
							break;
						}
						if(b_button())
						{
							light_on = -1;
							light_off = -1;
							break;
						}
						msleep(10L);
					}
				}
			}
			printf("\nWaiting for Light in Port #%d\n", port);
			while(analog10(port) > (light_off - light_on) / 2)
			{
				msleep(10L);
			}
			break;
		}
		if(b_button())
		{
			break;
		}
		msleep(10L);
	}
	printf("Program Running!\nEnding in %.2f seconds!\n", kill_time);
	shut_down_in(kill_time);
}
#endif
