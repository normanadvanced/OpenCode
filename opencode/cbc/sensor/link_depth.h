#ifndef __DEPTH_CBC_H__
#define __DEPTH_CBC_H__

#include "../../common/depthlib.h"
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

void cbc_align_depth_spin_one(int speed, int row, int tries)
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

void cbc_align_depth_dist_one(int speed, int distance, float aggressiveness, int row, int error, float timeout)
{
	float time_init=seconds();

	int current_error=1000;

	depth_open();
	depth_update();
	depth_scanline_update(row);

	int current_distance=get_depth_scanline_object_center_z(0);

	int drive_speed=0;

	while(seconds() < (time_init+timeout) && current_distance < (distance - error) || current_distance > (distance + error))
	{
		drive_speed=(current_distance-distance)*aggressiveness;
		cbc_direct(drive_speed, drive_speed);
		depth_update();
		depth_scanline_update(row);
		current_distance=get_depth_scanline_object_center_z(0);
		msleep(5);
	}
	cbc_halt();
}

#endif