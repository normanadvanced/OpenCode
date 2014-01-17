#include "create_config.h"

void create_align_depth_spin_two(int speed, int row, int error)
{
	depth_open();

	int theta = -get_point_angle(row);
	
	while(abs(theta) > error)
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
		
		theta = -get_point_angle(row);
		
		msleep(10);
	}
}

int get_point_angle(int row)
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
	}
}
