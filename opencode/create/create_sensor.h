#ifndef __CREATE_SENSOR_H__
#define __CREATE_SENSOR_H__

#include "create_config.h"

#define EVENT_WHEEL_DROP 1
#define EVENT_FRONT_WHEEL_DROP 2
#define EVENT_LEFT_WHEEL_DROP 3
#define EVENT_RIGHT_WHEEL_DROP 4
#define EVENT_BUMP 5
#define EVENT_LEFT_BUMP 6
#define EVENT_RIGHT_BUMP 7
#define EVENT_VIRTUAL_WALL 8
#define EVENT_WALL 9
#define EVENT_CLIFF 10
#define EVENT_LEFT_CLIFF 11
#define EVENT_FRONT_LEFT_CLIFF 12
#define EVENT_FRONT_RIGHT_CLIFF 13
#define EVENT_RIGHT_CLIFF 14
#define EVENT_HOME_BASE 15
#define EVENT_ADVANCE_BUTTON 16
#define EVENT_PLAY_BUTTON 17
#define EVENT_DIGITAL_INPUT(port) (18+port)
#define EVENT_OI_MODE_PASSIVE 22

//Create Wait for Light Sensor, plug into create bay ports 20 and 17
void create_wait_for_light()
{
	CREATE_BUSY;
	serial_write_byte(147);
	serial_write_byte(4);
	serial_write_byte(158);
	serial_write_byte(18);
	serial_write_byte(147);
	serial_write_byte(0);
	CREATE_FREE;
}
void create_wait_theta(int angle)
{
	CREATE_BUSY;
	serial_write_byte(157);
	serial_write_byte(get_high_byte(angle));
	serial_write_byte(get_low_byte(angle));
	CREATE_FREE;
}
void create_wait_length(int distance)
{
	CREATE_BUSY;
	serial_write_byte(156);
	serial_write_byte(get_high_byte(distance));
	serial_write_byte(get_low_byte(distance));
	CREATE_FREE;
}
void create_wait_duration(int dseconds)
{
	CREATE_BUSY;
	serial_write_byte(155);
	serial_write_byte(get_high_byte(dseconds));
	serial_write_byte(get_low_byte(dseconds));
	CREATE_FREE;
}
void create_wait_sensor(int packet_id)
{
	CREATE_BUSY;
	serial_write_byte(158);
	serial_write_byte(packet_id);
	CREATE_FREE;
}
void create_drive_bump(int rvel,int lvel)
{
	CREATE_BUSY;
	serial_write_byte(145);
	serial_write_byte(get_high_byte(rvel));
	serial_write_byte(get_low_byte(rvel));
	serial_write_byte(get_high_byte(lvel));
	serial_write_byte(get_low_byte(lvel));
	serial_write_byte(158);
	serial_write_byte(EVENT_BUMP);
	CREATE_FREE;
}
void create_drive_touch(int rspeed, int lspeed, int rport, int lport)
{
	create_drive_direct(rspeed, lspeed);
	while(!digital(rport) || !digital(lport))
	{
		if(digital(rport))
		{
			create_drive_direct(rspeed / 10, lspeed);
			msleep(10);
		}
		if(digital(lport))
		{
			create_drive_direct(rspeed, lspeed / 10);
			msleep(10);
		}
	}
}
void create_sync()
{
	char buffer[1];
	char *bptr = buffer;
	
	int read_count = 0;
	int count = 1;
	
	CREATE_BUSY;
	serial_write_byte(142);
	serial_write_byte(35);
	
	while(read_count < count)
	{
		#ifdef __arm__
		read_count += serial_read(buffer+read_count, count-read_count);
		#endif
	}
	CREATE_FREE;
}
#endif
