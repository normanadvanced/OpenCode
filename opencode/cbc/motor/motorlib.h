#ifndef __MOTORLIB_H__
#define __MOTORLIB_H__

#include <pthread.h>
#include "../sensor/sensorlib.h"
#include "/usr/include/kovan/kovan.h"
struct motor_attributes{
	int port, min, max;
	long tick_cycle;
	pthread_t thread_id;
	analog_sensor map_sensor;
}cbcmotor[4];
struct motor_movement_attributes
{
	struct motor_position_attributes{
		int speed;
		int relative_position;
	}*motorposition;
};
typedef struct motor_attributes *dcmotor;
typedef struct motor_position_attributes *dcmotor_position;
typedef struct motor_angle_attributes *dcmotor_angle;
typedef struct motor_movement_attributes *dcmotor_movement;
dcmotor build_dcmotor(int port, int min, int max, long tick_cycle){
	cbcmotor[port].port = port;
	cbcmotor[port].min = min;
	cbcmotor[port].max = max;
	cbcmotor[port].tick_cycle = tick_cycle;
	return(&cbcmotor[port]);
}
dcmotor_movement build_dcmotor_position(int speed, int relative_position){
	dcmotor_movement this_movement = malloc(sizeof(struct motor_movement_attributes));
	this_movement->motorposition = malloc(sizeof(struct motor_position_attributes));
	this_movement->motorposition->speed = speed;
	this_movement->motorposition->relative_position = relative_position;
	return(this_movement);
}
void move_motor(dcmotor this_motor, dcmotor_position position){
	mrp(this_motor->port,position->speed,position->relative_position);
}
void wait_motor(dcmotor this_motor, dcmotor_position position){
	mrp(this_motor->port,position->speed,position->relative_position);
	bmd(this_motor->port);
}

void
void kill_motor(dcmotor this_motor){
	off(this_motor->port);
}
#endif
