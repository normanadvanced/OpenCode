#ifndef __DEPTH_CREATE_H__
#define __DEPTH_CREATE_H__

#include "../common/depthlib.h"
#include "create_config.h"

void create_align_depth_spin_two(int speed, int row, int tries)
{
	depth_open();

	int theta;
	
	int current_try = 1;

	while(current_try <= tries && abs(theta = get_two_points_angle(row)) > depth_sensor.error)
	{
		CREATE_BUSY;
		serial_write_byte(145);
		if(theta < 0)
		{
			serial_write_byte(get_high_byte(-1 * speed));
			serial_write_byte(get_low_byte(-1 * speed));
			serial_write_byte(get_high_byte(speed));
			serial_write_byte(get_low_byte(speed));
		}
		else
		{
			serial_write_byte(get_high_byte(speed));
			serial_write_byte(get_low_byte(speed));
			serial_write_byte(get_high_byte(-1 * speed));
			serial_write_byte(get_low_byte(-1 * speed));
		}
		CREATE_FREE;
		create_wait_theta(theta);
		create_cease();
		
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
		CREATE_BUSY;
		serial_write_byte(145);
		if(theta < 0)
		{
			serial_write_byte(get_high_byte(-1 * speed));
			serial_write_byte(get_low_byte(-1 * speed));
			serial_write_byte(get_high_byte(speed));
			serial_write_byte(get_low_byte(speed));
		}
		else
		{
			serial_write_byte(get_high_byte(speed));
			serial_write_byte(get_low_byte(speed));
			serial_write_byte(get_high_byte(-1 * speed));
			serial_write_byte(get_low_byte(-1 * speed));
		}
		CREATE_FREE;
		create_wait_theta(theta);
		create_cease();
		
		msleep(10);
		current_try++;
	}
}

#endif
