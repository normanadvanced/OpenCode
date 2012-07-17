#ifndef __MOTORLIB_H__
#define __MOTORLIB_H__
#include <pthread.h>
struct motor_attributes{
	int port, min, max;
	long tick_cycle;
	pthread_t thread_id;
}cbcmotor[4];
struct motor_position_attributes{
	int speed;
	int relative_position;
};

struct motor_angle_attributes{
	float omega;
	float angle;
};
typedef struct motor_attributes *dcmotor;
typedef struct motor_position_attributes *dcmotor_position;
typedef struct motor_angle_attributes *dcmotor_angle;
dcmotor build_dcmotor(int port,int min, int max, long tick_cycle){
	cbcmotor[port].port = port;
	cbcmotor[port].min = min;
	cbcmotor[port].max = max;
	cbcmotor[port].tick_cycle = tick_cycle;
	return(&cbcmotor[port]);
}
dcmotor_position build_dcmotor_position(int speed, int relative_position){
	dcmotor_position position_prop = malloc(sizeof(struct motor_position_attributes));
	position_prop->speed = speed;
	position_prop->relative_position = relative_position;
	return(position_prop);
}
dcmotor_angle build_motor_angle(float omega, float angle){
	dcmotor_angle angle_prop = malloc(sizeof(struct motor_angle_attributes));
	angle_prop->omega = omega;
	angle_prop->angle = angle;
	
	return(angle_prop);
}
void move_motor(dcmotor this_motor, dcmotor_position position){
	mrp(this_motor->port,position->speed,position->relative_position);
}
void wait_motor(dcmotor this_motor, dcmotor_position position){
	mrp(this_motor->port,position->speed,position->relative_position);
	bmd(this_motor->port);
}
void move_motor_angle(dcmotor this_motor, dcmotor_angle angle){
	long ticks = (long)((float)this_motor->tick_cycle * angle->angle / 360.0);
	int speed = (int)(angle->omega * (float)this_motor->tick_cycle / 360.0);
	if(speed > 1000 || speed < -1000) printf("\nWarning! Invalid Angular Speed.\n");
	else mrp(this_motor->port,speed,ticks);
}

void wait_motor_angle(dcmotor this_motor, dcmotor_angle angle){
	move_motor_angle(this_motor, angle);
	bmd(this_motor->port);
}
void kill_motor(dcmotor this_motor){
	off(this_motor->port);
}
#endif
