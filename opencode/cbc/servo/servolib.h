#ifndef __SERVOLIB_H__
#define __SERVOLIB_H__
#define SERVO_COUNT 4
#define SERVO_MINPOS 0
#define SERVO_MAXPOS 2048
#define SERVO_TYPE_POSITION 'P'
#define SERVO_TYPE_ANGLE 'D'
#include <pthread.h>
#include "../sensor/sensorlib.h"
static pthread_mutex_t init_mem = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t servo_mem[SERVO_COUNT] = {
	PTHREAD_MUTEX_INITIALIZER,
	PTHREAD_MUTEX_INITIALIZER,
	PTHREAD_MUTEX_INITIALIZER,
	PTHREAD_MUTEX_INITIALIZER
};
struct servo_movement_properties
{
	struct servo_angle_properties{
		float angle, dpm;
	}*servo_angle;
	struct servo_position_properties{
		int position, tpm;
	}*servo_position;
	char type;
	long latency;
};
typedef struct servo_movement_properties *servo_movement;
struct servo_properties{
	int port, max, min;
	long next_latency;
	float min_d, max_d;
	pthread_t thread_id;
	analog_sensor map_sensor;
	servo_movement next_position;
}cbcservo[SERVO_COUNT];
typedef struct servo_properties *servo;
servo build_servo(int port, int min, int max, float min_d, float max_d){
	cbcservo[port].port = port;
	cbcservo[port].min = min;
	cbcservo[port].max = max;
	cbcservo[port].min_d = min_d;
	cbcservo[port].max_d = max_d;
	cbcservo[port].thread_id = NULL;
	return(&cbcservo[port]);
}
servo_movement build_servo_position(int position, int tpm, long latency){
	servo_movement new_position = malloc(sizeof(struct servo_movement_properties));
	new_position->servo_position = malloc(sizeof(struct servo_position_properties));
	new_position->servo_angle = malloc(sizeof(struct servo_angle_properties));
	new_position->servo_position->position = position;
	new_position->servo_position->tpm = tpm;
	new_position->latency = latency;
	new_position->type = SERVO_TYPE_POSITION;
	return(new_position);
}
servo_movement build_servo_angle(float angle, float dpm, long latency){
	servo_movement new_position = malloc(sizeof(struct servo_movement_properties));
	new_position->servo_position = malloc(sizeof(struct servo_position_properties));
	new_position->servo_angle = malloc(sizeof(struct servo_angle_properties));
	new_position->servo_position->position = -1;
	new_position->servo_angle->angle = angle;
	new_position->servo_angle->dpm = dpm;
	new_position->latency = latency;
	new_position->type = SERVO_TYPE_POSITION;
	return(new_position);
}

void *control_servo(void *ptr_servo){
	pthread_mutex_lock(&init_mem);
	servo this_servo = (struct servo_properties *) ptr_servo;
	pthread_mutex_unlock(&init_mem);
	pthread_mutex_lock(&servo_mem[this_servo->port]);
	int i, delta, initial;
	initial = get_servo_position(this_servo->port);
	delta = (this_servo->next_position->servo_position->position - initial) / (this_servo->next_position->servo_position->tpm);
	delta < 0 ? this_servo->next_position->servo_position->tpm *= -1 : this_servo->next_position->servo_position->tpm;
	delta < 0 ? delta *= -1 : delta;
	if(initial == -1) set_servo_position(this_servo->port, this_servo->next_position->servo_position->position);
	else if(this_servo->max >= this_servo->next_position->servo_position->position && this_servo->min <= this_servo->next_position->servo_position->position){
		for(i = 0; i < delta; i++){
			set_servo_position(this_servo->port, initial + (this_servo->next_position->servo_position->tpm * i));
			msleep(this_servo->next_position->latency);
		}
	}
	if(get_servo_position(this_servo->port) != this_servo->next_position->servo_position->position) set_servo_position(this_servo->port, this_servo->next_position->servo_position->position);
	pthread_mutex_unlock(&servo_mem[this_servo->port]);
	pthread_exit(NULL);
}
void init_servos(int a, int b, int c, int d)
{
	set_servo_position(0, a);
	set_servo_position(1, b);
	set_servo_position(2, c);
	set_servo_position(3, d);
}
void bsd(servo this_servo){
	pthread_join(this_servo->thread_id, NULL);
}
void move_servo(servo this_servo, servo_movement move_properties){
	if(move_properties->servo_position->position >= SERVO_MINPOS && move_properties->servo_position->position <= SERVO_MAXPOS){
		this_servo->next_position = move_properties;
	}
	else if(move_properties->type == SERVO_TYPE_ANGLE){
		move_properties->servo_position->position = this_servo->min + (int)((move_properties->servo_angle->angle - this_servo->min_d) / (this_servo->max_d - this_servo->min_d) * (float)(this_servo->max - this_servo->min));
		move_properties->servo_position->tpm = (int)(move_properties->servo_angle->dpm * ((float)(this_servo->max - this_servo->min) / (this_servo->max_d - this_servo->min_d)));
	}
	pthread_create(&this_servo->thread_id, NULL, &control_servo, (void *)this_servo);
}
void wait_servo(servo this_servo, servo_movement move_properties){
	this_servo->next_position = move_properties;
	move_servo(this_servo, move_properties);
	bsd(this_servo);
}
void kill_servo(servo this_servo){
	pthread_kill(this_servo->thread_id, 1);
}
void kill_servos(){
	kill_servo(&cbcservo[0]);
	kill_servo(&cbcservo[1]);
	kill_servo(&cbcservo[2]);
	kill_servo(&cbcservo[3]);
}
#endif
