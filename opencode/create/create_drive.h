#ifndef __CREATE_STD_H__
#define __CREATE_STD_H__

#include "create_config.h"
#include "create_sensor.h"

void create_direct(int rspeed, int lspeed)
{
	CREATE_BUSY;
	serial_write_byte(145);
	serial_write_byte(get_high_byte(rspeed));
	serial_write_byte(get_low_byte(rspeed));
	serial_write_byte(get_high_byte(lspeed));
	serial_write_byte(get_low_byte(lspeed));
	CREATE_FREE;
}
void create_straight(int speed)
{
	CREATE_BUSY;
	serial_write_byte(145);
	serial_write_byte(get_high_byte(speed));
	serial_write_byte(get_low_byte(speed));
	serial_write_byte(get_high_byte(speed));
	serial_write_byte(get_low_byte(speed));
	CREATE_FREE;
}
void create_arc(int speed, int radius)
{
	CREATE_BUSY;
	serial_write_byte(137);
	serial_write_byte(get_high_byte(speed));
	serial_write_byte(get_low_byte(speed));
	serial_write_byte(get_high_byte(radius));
	serial_write_byte(get_low_byte(radius));
	CREATE_FREE;
}
void create_spin(int omega)
{
	serial_write_byte(137);
	serial_write_byte(get_high_byte(omega));
	serial_write_byte(get_low_byte(omega));
	if(omega < 0)
	{
		serial_write_byte(255);
		serial_write_byte(255);
	}
	if(omega > 0)
	{
		serial_write_byte(0);
		serial_write_byte(1);
	}
}
//Smart Drive Functions, Speed always positive
void create_cease()
{
	create_stop();
	create_sync();
}
void create_drive_arc(unsigned int speed, int radius, float angle)
{
	CREATE_BUSY;
	serial_write_byte(137);
	if((radius * angle) < 0)
	{
		serial_write_byte(get_high_byte(-1 * speed));
		serial_write_byte(get_low_byte(-1 * speed));
	}
	else
	{
		serial_write_byte(get_high_byte(speed));
		serial_write_byte(get_low_byte(speed));
	}
	serial_write_byte(get_high_byte(radius));
	serial_write_byte(get_low_byte(radius));
	CREATE_FREE;
	create_wait_theta(angle);
}
void create_drive_segment(unsigned int speed, int distance)
{
	CREATE_BUSY;
	serial_write_byte(145);
	if(distance < 0)
	{
		serial_write_byte(get_high_byte(-1 * speed));
		serial_write_byte(get_low_byte(-1 * speed));
		serial_write_byte(get_high_byte(-1 * speed));
		serial_write_byte(get_low_byte(-1 * speed));
	}
	else
	{
		serial_write_byte(get_high_byte(speed));
		serial_write_byte(get_low_byte(speed));
		serial_write_byte(get_high_byte(speed));
		serial_write_byte(get_low_byte(speed));
	}
	CREATE_FREE;
	create_wait_length(distance);
}
void create_spin_angle(unsigned int speed, int angle)
{
	CREATE_BUSY;
	serial_write_byte(145);
	if(angle < 0)
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
	create_wait_theta(angle);
}
void create_translate(float x, float y, unsigned int speed)
{
	float phi = abs(atan((float)y / (float)x));
	float theta = abs((PI / 2.0) - phi);
	float length = sqrt((x * x + y * y) / 4.0);
	float radius = length * sin(phi) / sin(theta);
	phi *= (180.0 / PI);
	theta *= (180.0 / PI);
	if(x > 0)
	{
		if(y > 0)
		{
			create_drive_arc(speed, (int)(radius * -1.0), (int)(theta * -1.0));
			create_drive_arc(speed, (int)radius, (int)theta);
		}
		if(y < 0)
		{
			create_drive_arc(speed, (int)(radius * -1.0), (int)theta);
			create_drive_arc(speed, (int)radius, (int)(theta * -1.0));
		}
	}
	if(x < 0)
	{
		if(y > 0)
		{
			create_drive_arc(speed, (int)radius, (int)theta);
			create_drive_arc(speed, (int)(radius * -1.0), (int)(theta * -1.0));
		}
		if(y < 0)
		{
			create_drive_arc(speed, (int)radius, (int)(theta * -1.0));
			create_drive_arc(speed, (int)(radius * -1.0), (int)theta);
		}
	}
}
#endif
