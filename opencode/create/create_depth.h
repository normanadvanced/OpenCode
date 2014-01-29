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

int get_two_points_angle(int row)
{
	int first_center_x=-1;
	int first_center_z=-1;
	
	int second_center_x=-1;
	int second_center_z=-1;
	
	int left_center_x=-1;
	int left_center_z=-1;
	
	int right_center_x=-1;
	int right_center_z=-1;
	
	int theta=0;
	
	depth_update();
	
	depth_scanline_update(row);
	
	if(get_depth_scanline_object_count() > 0)
	{
		first_center_x = get_depth_scanline_object_center_x(0);
		first_center_z = get_depth_scanline_object_center_z(0);
		
		second_center_x = get_depth_scanline_object_center_x(1);
		second_center_z = get_depth_scanline_object_center_z(1);
		
		if(first_center_x < second_center_x)
		{
			left_center_x = first_center_x;
			left_center_z = first_center_z;
			
			right_center_x = second_center_x;
			right_center_z = second_center_z;
		}
		else
		{
			left_center_x = second_center_x;
			left_center_z = second_center_z;
			
			right_center_x = first_center_x;
			right_center_z = first_center_z;
		}
		
		if(left_center_x != -1 && left_center_z != -1 && right_center_x != -1 && right_center_z != -1)
		{
			return atan2(right_center_z - left_center_z, right_center_x - left_center_x) * 180.0 / M_PI;
		}
		else
		{
			return 0;
		}
	}
}

int get_angle_to_point(int row)
{	
	int nearest_x=-1;
	int nearest_z=-1;
	
	depth_update();
	
	depth_scanline_update(row);
	
	if(get_depth_scanline_object_count() > 0)
	{
		nearest_x = get_depth_scanline_object_center_x(0);
		nearest_z = get_depth_scanline_object_center_z(0);

		printf("nearest x = %d\n", nearest_x);
		printf("nearest z = %d\n", nearest_z);
		
		if(nearest_x != -1 && nearest_z > 0)
		{
			printf("%lf\n", asin(-((double)nearest_x-depth_sensor.dist_x)/((double)nearest_z+depth_sensor.dist_z)) * 180 / M_PI);
			return asin(-((double)nearest_x-depth_sensor.dist_x)/((double)nearest_z+depth_sensor.dist_z)) * 180 / M_PI;
		}
		else
		{
			return 0;
			printf("Bad Depth Data\n");
		}
	}
}

#endif
