#ifndef __CREATE_ACCEL_H__
#define __CREATE_ACCEL_H__

#include "create_config.h"
#include "create_drive.h"
#include "create_sensor.h"

float speed_profile[2][15] = {
	{0.112, 0.302, 0.492, 0.669, 0.805, 0.912, 0.977, 1.000, 0.977, 0.912, 0.805, 0.669, 0.492, 0.302, 0.112},
	{0.977, 0.912, 0.805, 0.669, 0.492, 0.302, 0.112, 0.000, -0.112, -0.302, -0.492, -0.669, -0.805, -0.912, -0.977},
};


void create_accel_straight(int profile, float max_velocity, float distance)
{
	int i;
	float interval;
	
	interval = fabs((PI * distance) / (30.0 * max_velocity));
	for( i = 0; i < 15; i++)
	{
		create_straight((int)(max_velocity * speed_profile[profile][i]));
		//sleep(interval - 0.01);
		create_wait_duration((int)(interval*10.0));
	}
}
void create_accel_arc(int profile, float max_velocity, float radius, float angle)
{
	int i;
	float interval;
	
	interval = fabs((PI * angle * radius * PI) / (5400.0 * max_velocity));
	for(i = 0; i < 15; i++)
	{
		create_arc((int)(max_velocity * speed_profile[profile][i]), (int)radius);
		//sleep(interval - 0.01);
		create_wait_duration((int)(interval*10.0));
	}
	
}
void create_accel_spin(int profile, float max_omega, float angle)
{
	int i;
	float interval;
	
	interval = fabs((PI * angle) / (30.0 * max_omega));
	for( i = 0; i < 15; i++)
	{
		create_spin((int)(max_omega * speed_profile[profile][i]));
		//sleep(interval - 0.01);
		create_wait_duration((int)(interval*10.0));
	}
}
#endif
