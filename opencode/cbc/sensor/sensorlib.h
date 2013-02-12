#ifndef __SENSORLIB_H__
#define __SENSORLIB_H__

#include <math.h>
#include <malloc.h>
#include "/usr/include/kovan/kovan.h"
//Structured Memory Initialization
struct analog_sensor_properties
{
	int port;
	int is_float;
	int trials;
	long refresh;
} cbcanalog[8];
struct analog_event_properties
{
	int value;
	int error;
};

typedef struct analog_sensor_properties *analog_sensor;
typedef struct analog_event_properties *analog_event;

//Prototyped Functions
analog_sensor build_analog_sensor(int port, int is_float, int trials, long refresh);
analog_event build_analog_event(int value, int error);
float analog_average(analog_sensor);
int read_analog(analog_sensor sensor, analog_event event);
int wait_analog(analog_sensor sensor_properties, analog_event event_properties);

//Implemented Functions
analog_sensor build_analog_sensor(int port, int is_float, int trials, long refresh)
{
	int i;
	int mask = 0;
	cbcanalog[port].port = port;
	cbcanalog[port].is_float = is_float;
	cbcanalog[port].trials = trials;
	cbcanalog[port].refresh = refresh;
	for(i = 0; i <= 7; i++)
	{
		if(cbcanalog[i].is_float == 1)
		{
			mask += 1 << i;
		}
		else if(cbcanalog[i].is_float != 0)
		{
			printf("Invalid set_float value for Analog: %d\nPlease set to 0 or 1\n", i);
		}
	}
	//set_analog_floats(0); //revmoved for new function name in libkovan
	set_analog_pullup(port, mask);
	return(&cbcanalog[port]);
}
analog_event build_analog_event(int value, int error)
{
	analog_event this_event = malloc(sizeof(struct analog_event_properties));
	
	this_event->value = value;
	this_event->error = error;
	return(this_event);
}
float analog_average(analog_sensor sensor)
{
	int i;
	float sum = 0.0;
	for(i = 0; i < sensor->trials; i++)
	{
		sum += (float)analog10(sensor->port);
		msleep(sensor->refresh);
	}
	return(sum / sensor->trials);
}
int analog_state(analog_sensor sensor, analog_event event)
{
	int initial = analog_average(sensor);
	if(initial <= (event->value + event->error) && initial >= (event->value - event->error))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
float analog_change_rate(analog_sensor sensor)
{
	float ivalue = analog_average(sensor);
	float itime = seconds();
	float fvalue = analog_average(sensor);
	float ftime = seconds();
	
	return((fvalue - ivalue) / (1000 * (ftime - itime)));
}

#endif
