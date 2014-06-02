#ifndef __DEPTH_CBC_H__
#define __DEPTH_CBC_H__

#include "../common/depthlib.h"
#include "../drive/drivelib.h"

void cbc_align_depth_spin_two(int speed, int row, int tries)
{
	depth_open();

	int theta;
	
	int current_try = 1;

	while(current_try <= tries && abs(theta = get_two_points_angle(row)) > depth_sensor.error)
	{
		cbc_spin(speed, theta);
		
		msleep(10);
		current_try++;
	}
}

void create_align_depth_spin_one(int speed, int row, int tries)
{
	depth_open();

	int theta;
	
	int current_try = 1;

	while(current_try <= tries && abs(theta = get_angle_to_point(row)) > depth_sensor.error)
	{
		cbc_spin(speed, theta);
		
		msleep(10);
		current_try++;
	}
}

#endif