#ifndef __DRIVELIB_H__
#define __DRIVELIB_H__

#include <stdio.h>
#include <pthread.h>

#include "/usr/include/kovan/kovan.h"
#include "../sensor/sensorlib.h"

#define PI 3.14159
#define DEG_TO_RAD (PI / 180.0)

void build_left_wheel(int port, long ticks_cycle, float speed_proportion, float wheel_diameter, float radial_distance);
// 0 <--> 3 (port), ~1100 (ticks), 1.0 + | - 0.25 (unitless), + in mm, + in mm
//Shortcut to build the left wheel structure to be utilized by various functions

void build_left_wheel(int port, long ticks_cycle, float speed_proportion, float wheel_diameter, float radial_distance);
// 0 <--> 3 (port), ~1100 (ticks), 1.0 + | - 0.25 (unitless), + in mm, + in mm
//Shortcut to build the right wheel structure to be utilized by various functions

int cbc_straight(int speed, float distance);
// 0 <--> 1000, + | - in mm
//Moves the center of the CBC drivetrain in a straight line
//Returns 1 if executed, -1 if error is detected

int cbc_arc(int speed, float radius, float theta);
// 0 <--> 1000 (unitless), + | - in mm, + | - in degrees
//Moves the center of the CBC drivetrain in a constant radial arc
//Returns 1 if executed, -1 if error is detected

int cbc_spin(int speed, float theta);
// 0 <--> 1000, + | - in degrees
//Rotates the center of the CBC drivetrain
//Returns 1 if executed, -1 if error is detected

struct cbc_side
{
	struct wheel_properties
	{
		int port;
		int last_requested_speed;
		long ticks_cycle;
		float speed_proportion;
		float wheel_diameter;
		float radial_distance;
	}wheel;
	struct lincolns_tophat
	{
		int port;
		int black;
		int white;
		int error;
		long timeout;
	}tophat;
	struct touch_me
	{
		int port;
		long timeout;
	}touch;
}left, right;
struct cbc_accel
{
	float x_knaught[3];
	long timeout;
}acceleramator;
void build_left_wheel(int port, long ticks_cycle, float speed_proportion, float wheel_diameter, float radial_distance)
{
	left.wheel.port = port;
	left.wheel.ticks_cycle = ticks_cycle;
	left.wheel.speed_proportion = speed_proportion;
	left.wheel.wheel_diameter = wheel_diameter;
	left.wheel.radial_distance = radial_distance;
}
void build_right_wheel(int port, long ticks_cycle, float speed_proportion, float wheel_diameter, float radial_distance)
{
	right.wheel.port = port;
	right.wheel.ticks_cycle = ticks_cycle;
	right.wheel.speed_proportion = speed_proportion;
	right.wheel.wheel_diameter = wheel_diameter;
	right.wheel.radial_distance = radial_distance;
}
void build_left_tophat(int port, int white, int black, int error, long timeout)
{
	analog_sensor left_drive_tophat = build_analog_sensor(port, 0, 1, 0);
	left.tophat.port = port;
	left.tophat.white = white;
	left.tophat.black = black;
	left.tophat.error = error;
	left.tophat.timeout = timeout;
}
void build_right_tophat(int port, int white, int black, int error, long timeout)
{
	analog_sensor right_drive_tophat = build_analog_sensor(port, 0, 1, 0);
	right.tophat.port = port;
	right.tophat.white = white;
	right.tophat.black = black;
	right.tophat.error = error;
	right.tophat.timeout = timeout;
}
void build_left_touch(int port, long timeout)
{
	right.touch.port = port;
	right.touch.timeout = timeout;
}
void build_right_touch(int port, long timeout)
{
	right.touch.port = port;
	right.touch.timeout = timeout;
}
void cbc_wait()
{
	bmd(left.wheel.port);
	bmd(right.wheel.port);
}
void cbc_halt()
{
	off(left.wheel.port);
	off(right.wheel.port);
}
void cbc_stop()
{
	cbc_wait();
	cbc_halt();
}
int cbc_direct(int lspeed, int rspeed)
{
	mav(left.wheel.port, lspeed);
	mav(right.wheel.port,rspeed);
	left.wheel.last_requested_speed = lspeed;
	right.wheel.last_requested_speed = rspeed;
	return 1;
}
int cbc_straight(int speed, float distance)
{
	float lticks = (distance * left.wheel.ticks_cycle) / (left.wheel.wheel_diameter * PI);
	float rticks = (distance * right.wheel.ticks_cycle) / (right.wheel.wheel_diameter * PI);
	float lspeed = (float)speed * left.wheel.speed_proportion;
	float rspeed = (float)speed * right.wheel.speed_proportion;

	if(rspeed > 1000 || lspeed > 1000 || rspeed < -1000 || lspeed < -1000)
	{
		printf("\nWarning! Invalid Speed\n");
		return -1;
	}
	else
	{
		mrp(left.wheel.port, (int)lspeed, (long)lticks);
		mrp(right.wheel.port, (int)rspeed, (long)rticks);
		cbc_wait();
		left.wheel.last_requested_speed = (int)lspeed;
		right.wheel.last_requested_speed = (int)rspeed;
		return 1;
	}
}
int cbc_arc(int speed, float radius, float theta) // 0 <--> 1000 (unitless), + | - in mm, + | - in degrees
{
	float arc_length = radius * theta * DEG_TO_RAD;
	float ldistance = (radius - left.wheel.radial_distance) * theta * DEG_TO_RAD;
	float rdistance = (radius + right.wheel.radial_distance) * theta * DEG_TO_RAD;
	float lticks = (ldistance * left.wheel.ticks_cycle) / (left.wheel.wheel_diameter * PI);
	float rticks = (rdistance * right.wheel.ticks_cycle) / (right.wheel.wheel_diameter * PI);
	float lspeed = (float)speed * left.wheel.speed_proportion * ldistance / arc_length;
	float rspeed = (float)speed * right.wheel.speed_proportion * rdistance / arc_length;

	if(rspeed > 1000 || lspeed > 1000 || rspeed < -1000 || lspeed < -1000)
	{
		printf("\nWarning! Invalid Speed, Please Lower\n");
		return -1;
	}
	else
	{
		mrp(left.wheel.port, (int)lspeed, (long)lticks);
		mrp(right.wheel.port, (int)rspeed, (long)rticks);
		cbc_wait();
		left.wheel.last_requested_speed = (int)lspeed;
		right.wheel.last_requested_speed = (int)rspeed;
		return 1;
	}
}
int cbc_spin(int speed, float theta)
{
	float ldistance = -1.0 * left.wheel.radial_distance * theta * DEG_TO_RAD;
	float rdistance = right.wheel.radial_distance * theta * DEG_TO_RAD;
	float lticks = (ldistance * left.wheel.ticks_cycle) / (left.wheel.wheel_diameter * PI);
	float rticks = (rdistance * right.wheel.ticks_cycle) / (right.wheel.wheel_diameter * PI);
	float lspeed = (float)speed * left.wheel.speed_proportion * left.wheel.radial_distance / (left.wheel.radial_distance + left.wheel.radial_distance);
	float rspeed = (float)speed * right.wheel.speed_proportion * right.wheel.radial_distance / (right.wheel.radial_distance + right.wheel.radial_distance);

	if(rspeed > 1000 || lspeed > 1000 || rspeed < -1000 || lspeed < -1000)
	{
		printf("\nWarning! Invalid Speed, Please Lower\n");
		return -1;
	}
	else
	{
		mrp(left.wheel.port, (int)lspeed, (long)lticks);
		mrp(right.wheel.port, (int)rspeed, (long)rticks);
		cbc_wait();
		left.wheel.last_requested_speed = (int)lspeed;
		right.wheel.last_requested_speed = (int)rspeed;
		return 1;
	}

}
int cbc_align_touch()
{
	long ltimeout = left.touch.timeout;
	long rtimeout = right.touch.timeout;
	mav(left.wheel.port, left.wheel.last_requested_speed);
	mav(right.wheel.port, right.wheel.last_requested_speed);
	while(ltimeout > 0 && rtimeout > 0 && !digital(left.tophat.port) && !digital(right.tophat.port))
	{

		ltimeout -= 10L;
		rtimeout -= 10L;
	}
	cbc_halt();
}
int cbc_align_black()
{
	long ltimeout = left.tophat.timeout;
	long rtimeout = right.tophat.timeout;
	//set_analog_floats(0); //revmoved for new function name in libkovan
	set_analog_pullup(left.tophat.port, 0);
	set_analog_pullup(right.tophat.port, 0);
	mav(left.wheel.port, left.wheel.last_requested_speed);
	mav(right.wheel.port, right.wheel.last_requested_speed);
	while((ltimeout > 0 || rtimeout > 0) && (analog10(left.tophat.port) < (left.tophat.white + left.tophat.error) || analog10(right.tophat.port) < (right.tophat.white + right.tophat.error)))
	{
		if(analog10(left.tophat.port) > (left.tophat.black - left.tophat.error))
		{
			off(left.wheel.port);
		}
		if(analog10(right.tophat.port) > (right.tophat.black - right.tophat.error))
		{
			off(right.wheel.port);
		}
		msleep(10L);
		ltimeout -= 10L;
		rtimeout -= 10L;
	}
}
int cbc_align_white()
{
	long ltimeout = left.tophat.timeout;
	long rtimeout = right.tophat.timeout;
	//set_analog_floats(0); //revmoved for new function name in libkovan
	set_analog_pullup(left.tophat.port, 0);
	set_analog_pullup(right.tophat.port, 0);
	mav(left.wheel.port, left.wheel.last_requested_speed);
	mav(right.wheel.port, right.wheel.last_requested_speed);
	while((ltimeout > 0 || rtimeout > 0) && (analog10(left.tophat.port) > (left.tophat.black - left.tophat.error) || analog10(right.tophat.port) > (right.tophat.black - right.tophat.error)))
	{
		if(analog10(left.tophat.port) < (left.tophat.white + left.tophat.error))
		{
			off(left.wheel.port);
		}
		if(analog10(right.tophat.port) < (right.tophat.white + right.tophat.error))
		{
			off(right.wheel.port);
		}
		msleep(10L);
		ltimeout -= 10L;
		rtimeout -= 10L;
	}
}
void *accel_bump(void *this_accel)
{
	struct cbc_accel *accel = (struct cbc_accel *)this_accel;
	while(accel->timeout > 0)
	{
		if(accel->x_knaught[0] > (accel_x() + 0.1) || accel->x_knaught[0] < (accel_x() - 0.1))
		{
			cbc_halt();
			break;
		}
		else if(accel->x_knaught[1] > (accel_y() + 0.1) || accel->x_knaught[2] < (accel_y() - 0.1))
		{
			cbc_halt();
			break;
		}
		else if(accel->x_knaught[2] > (accel_z() + 0.1) || accel->x_knaught[2] < (accel_z() - 0.1))
		{
			cbc_halt();
			break;
		}
	}
}
void cbc_sense_accel()
{
	int thread_num;
	pthread_t this_thread;
	acceleramator.x_knaught[1] = accel_x();
	acceleramator.x_knaught[2] = accel_y();
	acceleramator.x_knaught[3] = accel_z();
	struct cbc_accel *here = &acceleramator;
	if((thread_num = pthread_create(&this_thread, NULL, &accel_bump, (void *)here)))
	{
		printf("Threading Failure: %d\n", thread_num);
	}
}
#endif

