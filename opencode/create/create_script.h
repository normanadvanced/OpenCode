#ifndef __CREATE_SCRIPT_H__
#define __CREATE_SCRIPT_H__

#include "create_config.h"

struct create_script_buffer
{
	char script[100];
	int length;
} create_script;

void script_write_byte(char byte)
{
	if(create_script.length < 100)
	{
		create_script.script[create_script.length++] = byte;
	}
	else
	{
		printf("\nError: Script Can Not Excede 100 Bytes\n");
	}
}
void load_script()
{
	
	int i;
	create_connect();
	printf("CHECK POWER LED\n");
	printf("If GREEN, Restart Program\n");
	printf("If YELLOW, Restart Program\n");
	printf("If ORANGE, Press A\n");
	
	while(a_button()) msleep(20);//wait for the a button to not be pressed to be pressed
	while(!a_button()) msleep(20);//wait for the a button to not be pressed
	
	printf("Create connected, loading script");
	
	serial_write_byte(152);
	serial_write_byte(create_script.length);
	printf(".");
	for( i = 0; i < create_script.length; i++)
	{
		serial_write_byte(create_script.script[i]);
	}
	printf(".");
	create_power_led(64,255);// 4 bytes
	printf(".\n");
	printf("Uploaded %d bytes...\nOkay to disconnect download cable!/n", create_script.length);
}
//Create Waits for Light Sensor, plug into create bay ports 20 and 17
void script_wait_for_light()
{
	script_write_byte(147);
	script_write_byte(4);
	script_write_byte(158);
	script_write_byte(18);
}
void script_reboot()
{
	script_write_byte(7);
}
void script_wait_theta(int angle)
{
	script_write_byte(157);
	script_write_byte(get_high_byte(angle));
	script_write_byte(get_low_byte(angle));
}
void script_wait_length(int distance)
{
	script_write_byte(156);
	script_write_byte(get_high_byte(distance));
	script_write_byte(get_low_byte(distance));
}
void script_wait_duration(int dseconds)
{
	script_write_byte(155);
	script_write_byte(get_high_byte(dseconds));
	script_write_byte(get_low_byte(dseconds));
}
//Packet ID's listed in the define's
void script_wait_sensor(int packet_id)
{
	script_write_byte(158);
	script_write_byte(get_high_byte(packet_id));
	script_write_byte(get_low_byte(packet_id));
}
//Create Script Drive Functions
void script_direct(int rspeed, int lspeed)
{
	script_write_byte(145);
	script_write_byte(get_high_byte(rspeed));
	script_write_byte(get_low_byte(rspeed));
	script_write_byte(get_high_byte(lspeed));
	script_write_byte(get_low_byte(lspeed));
}
void script_straight(int speed)
{
	script_write_byte(145);
	script_write_byte(get_high_byte(speed));
	script_write_byte(get_low_byte(speed));
	script_write_byte(get_high_byte(speed));
	script_write_byte(get_low_byte(speed));
}
void script_stop()
{
	script_write_byte(145);
	script_write_byte(0);
	script_write_byte(0);
	script_write_byte(0);
	script_write_byte(0);
}
void script_arc(int speed, int radius)
{
	script_write_byte(get_high_byte(speed));
	script_write_byte(get_low_byte(speed));
	script_write_byte(get_high_byte(radius));
	script_write_byte(get_low_byte(radius));
}
//Smart Drive Functions, Speed always positive
void script_drive_arc(unsigned int speed, int radius, float angle)
{
	script_write_byte(137);
	if((radius * angle) < 0)
	{
		script_write_byte(get_high_byte(-1 * speed));
		script_write_byte(get_low_byte(-1 * speed));
	}
	else
	{
		script_write_byte(get_high_byte(speed));
		script_write_byte(get_low_byte(speed));
	}
	script_write_byte(get_high_byte(radius));
	script_write_byte(get_low_byte(radius));
	script_wait_theta(angle);
}
void script_drive_segment(unsigned int speed, int distance)
{
	script_write_byte(145);
	if(distance < 0)
	{
		script_write_byte(get_high_byte(-1 * speed));
		script_write_byte(get_low_byte(-1 * speed));
		script_write_byte(get_high_byte(-1 * speed));
		script_write_byte(get_low_byte(-1 * speed));
	}
	else
	{
		script_write_byte(get_high_byte(speed));
		script_write_byte(get_low_byte(speed));
		script_write_byte(get_high_byte(speed));
		script_write_byte(get_low_byte(speed));
	}
	script_wait_length(distance);
}
void script_spin_angle(unsigned int speed, int angle)
{
	script_write_byte(145);
	if(angle < 0)
	{
		script_write_byte(get_high_byte(-1 * speed));
		script_write_byte(get_low_byte(-1 * speed));
		script_write_byte(get_high_byte(speed));
		script_write_byte(get_low_byte(speed));
	}
	else
	{
		script_write_byte(get_high_byte(speed));
		script_write_byte(get_low_byte(speed));
		script_write_byte(get_high_byte(-1 * speed));
		script_write_byte(get_low_byte(-1 * speed));
	}
	script_wait_theta(angle);
}
void script_LSD(char b1, char b2, char b3)
{
	script_write_byte(138);
	script_write_byte(b1 + 2 * b2 + 4 * b3);
}
void script_LSDPWM(char s1, char s2, char s3)
{
	
	script_write_byte(144);
	script_write_byte(s1);
	script_write_byte(s2);
	script_write_byte(s3);
}

#endif
