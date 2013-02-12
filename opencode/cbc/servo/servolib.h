#ifndef __SERVOLIB_H__
#define __SERVOLIB_H__

#include "/usr/include/pthread.h"
#include <math.h>
#include <malloc.h>
#include "/usr/include/kovan/kovan.h"

struct servo_properties
{
	int port;
	int max;
	int min;
	int next_tpm;
	int next_position;
	int is_moving;
	pthread_t process_id;
	long next_latency;
}cbcservo[4];

struct servo_movement
{
	int position;
	int tpm;
	long latency;
};

typedef struct servo_movement *servo_movement;
typedef struct servo_properties *servo;
pthread_mutex_t servo_mem = PTHREAD_MUTEX_INITIALIZER;

servo build_servo(int port, int min, int max)
{
	cbcservo[port].port = port;
	cbcservo[port].min = min;
	cbcservo[port].max = max;
	return(&cbcservo[port]);
}
servo_movement build_servo_movement(int position, int tpm, long latency)
{
	servo_movement new_movement = malloc(sizeof(struct servo_movement));
	new_movement->position = position;
	new_movement->tpm = tpm;
	new_movement->latency = latency;
	return(new_movement);
}
void *control_servo(void *ptr_servo)
{
	pthread_mutex_lock(&servo_mem);
	servo build_properties = (struct servo_properties *) ptr_servo;
	int i, delta;
	int initial = get_servo_position(build_properties->port);
	delta = (build_properties->next_position - initial) / (build_properties->next_tpm);
	if(initial == -1)
	{
		set_servo_position(build_properties->port, build_properties->next_position);
	}
	else if(build_properties->max >= build_properties->next_position && build_properties->min <= build_properties->next_position && build_properties->min < build_properties->max)
	{
		if(initial < build_properties->next_position)
		{
			for(i = 0; i < delta; i++)
			{
				set_servo_position(build_properties->port, initial + (build_properties->next_tpm * i));
				msleep(build_properties->next_latency);
			}
		}
		if(initial > build_properties->next_position)
		{
			for(i = 0; i > delta; i--)
			{
				set_servo_position(build_properties->port, initial + (build_properties->next_tpm * i));
				msleep(build_properties->next_latency);
			}
		}
		if(get_servo_position(build_properties->port) != build_properties->next_position)
		{
			set_servo_position(build_properties->port, build_properties->next_position);
		}
	}
	else
	{
		printf("Invalid Movement of Servo: %d", build_properties->port);
	}
	build_properties->is_moving = 0;
	pthread_mutex_unlock(&servo_mem);
}
void move_servo(servo build_properties,servo_movement move_properties)
{
	int thread_num;
	pthread_t this_thread;
	build_properties->is_moving = 1;
	build_properties->next_position = move_properties->position;
	build_properties->next_tpm = move_properties->tpm;
	build_properties->next_latency = move_properties->latency;
	if((thread_num = pthread_create(&this_thread, NULL, &control_servo, (void *)build_properties)))
	{
		printf("Threading Failure: %d\n", thread_num);
	}
	else
	{
		build_properties->process_id = this_thread;
	}
}
void bsd(servo build_properties)
{
	pthread_join(build_properties->process_id, NULL);
}
void wait_servo(servo build_properties, servo_movement move_properties)
{
	build_properties->is_moving = 1;
	build_properties->next_position = move_properties->position;
	build_properties->next_tpm = move_properties->tpm;
	build_properties->next_latency = move_properties->latency;
	move_servo(build_properties, move_properties);
	bsd(build_properties);
}

#endif
