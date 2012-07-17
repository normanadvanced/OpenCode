#ifndef __SENSORLIB_H__
#define __SENSORLIB_H__
#include <stdio.h>
struct analog_sensor_properties{
	int port, min, max;
} cbcanalog[8];
typedef struct analog_sensor_properties *analog_sensor;
struct analog_event_properties{
	int value, error;
};
typedef struct analog_event_properties *analog_event;
analog_sensor build_analog_sensor(int port, int min, int max){
	int i;
	int mask = 0;
	cbcanalog[port].port = port;
	cbcanalog[port].min = min;
	cbcanalog[port].max = max;
	return(&cbcanalog[port]);
}
analog_event build_analog_event(int value, int error){
	analog_event this_event = malloc(sizeof(struct analog_event_properties));
	this_event->value = value;
	this_event->error = error;
	return(this_event);
}
float analog_average(analog_sensor sensor, int trials, long refresh){
	int i;
	float sum = 0.0;
	for(i = 0; i < trials; i++){
		sum += (float)analog10(sensor->port);
		msleep(refresh);
	}
	return(sum / (float)trials);
}
int analog_state(analog_sensor sensor, analog_event event){
	int initial = analog_average(sensor, 3, 1);
	if(initial <= (event->value + event->error) && initial >= (event->value - event->error)) return(1);
	else return 0;
}
float analog_change_rate(analog_sensor sensor, int trials, int refresh){
    float val[2], time[2];
	val[0] = analog_average(sensor, trials, refresh);
	time[0] = seconds();
	val[1] = analog_average(sensor, trials, refresh);
	time[1] = seconds();
	return((val[1] - val[1]) / (1000.0 * (time[1] - time[0])));
}
#endif
