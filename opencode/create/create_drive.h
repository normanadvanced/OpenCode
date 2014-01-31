#ifndef __CREATE_STD_H__
#define __CREATE_STD_H__

#include "create_config.h"
#include "create_sensor.h"

#define CREATE_RADIUS 129.0
#define CREATE_WHEELCIRCUM 65.1
#define DEG_2_RAD (PI / 180.0)

//move the create's wheels at independent speeds
void create_direct(int rspeed, int lspeed)
{
	if(lspeed > 500 || lspeed < -500 || rspeed > 500 || rspeed < -500){
		printf("Invalid Create Speed, create_direct(%d, %d)\n", rspeed, lspeed);
	}
	CREATE_BUSY;
	serial_write_byte(145);
	serial_write_byte(get_high_byte(rspeed));
	serial_write_byte(get_low_byte(rspeed));
	serial_write_byte(get_high_byte(lspeed));
	serial_write_byte(get_low_byte(lspeed));
	CREATE_FREE;
}
//move the create straight at a given speed
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
//move the create in an arc of a given radius at a given speed
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
//turn the create in place at a given speed
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
//turn the create in place at a given speed, alternate
void create_spin_b(float omega){
	int speed = (int)(omega * CREATE_RADIUS * PI / 180.0);
	create_direct(speed, -speed);
}
//move the create straight at a given speed, alternate
void create_straight_b(float velocity){
	int speed = (int)(velocity);
	create_direct(speed, speed);
}
//move the create in an arc of a given radius at a given speed, alternate
void create_arc_b(float radius, float omega){
	int lspeed = (int)(omega * DEG_2_RAD * (radius - CREATE_RADIUS));
	int rspeed = (int)(omega * DEG_2_RAD * (radius + CREATE_RADIUS));
	create_direct(rspeed, lspeed);
}

//Smart Drive Functions, Speed always positive

//stop create and sync with controller
void create_cease()
{
	create_stop();
	create_sync();
}
//move the create in an arc of a given radius at a given speed for a given angle
//if radius and angle are of opposite sign, the create will go backward
//do not put a negative speed
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
//move the create straight at a given speed for a given distance
//use negative distance to go backward, speed is always positive
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
//trun the create in place at a given speed for a given angle
//positive angle is counter-clockwise
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
#endif
