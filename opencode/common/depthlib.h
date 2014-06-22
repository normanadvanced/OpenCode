#ifndef __DEPTH_H__
#define __DEPTH_H__

struct depth_attributes{
	double dist_x, dist_z, theta, error;
}depth_sensor;

typedef struct depth_attributes *depth_sensor_type;

depth_sensor_type build_depth(double dist_x, double dist_z, double theta, double error){
	depth_sensor.dist_x = dist_x;
	depth_sensor.dist_z = dist_z;
	depth_sensor.theta = theta;
	depth_sensor.error = error;
	return(&depth_sensor);
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
			printf("Bad Depth Data at row %d\n", row);
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
			printf("Bad Depth Data at row %d\n", row);
			return 0;
		}
	}
}

void open_depth_safely(int attepmts)
{
	int times=0;
	while(depth_open()==0 && times<attepmts)
	{
		printf("Cannot open Xtion\n");
		msleep(700);
	}
}

#endif
