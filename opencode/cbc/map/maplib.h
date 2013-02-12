#ifndef __MAPLIB_H__
#define __MAPLIB_H__

#include "../sensor/sensorlib.h"
#include "../servo/servolib.h"
#include "../motor/motorlib.h"
#include "/usr/include/kovan/kovan.h"
struct mapper{
	analog_sensor mapsensor;
	servo mapservo;
	dcmotor mapmotor;
	pthread_t thread_id;
};
void *map_servo_control(void *ptr){
	struct mapper *this_map = (struct mapper *) ptr;
	int aval = analog10(this_map->mapsensor->port);
	aval > this_map->mapsensor->max ? aval = this_map->mapsensor->max : aval;
	aval < this_map->mapsensor->min ? aval = this_map->mapsensor->min : aval;
	while(1){
		set_servo_position(this_map->mapservo->port, this_map->mapservo->min + (int)((float)(this_map->mapservo->max - this_map->mapservo->min) * (float)(aval - this_map->mapservo->min) / (float)(this_map->mapservo->max - this_map->mapservo->min)));
		msleep(5L);
	}
}
void *map_motor_control(void *ptr)
{
	struct mapper *this_map = (struct mapper *) ptr;
	int aval = analog10(this_map->mapsensor->port);
	aval > this_map->mapsensor->max ? aval = this_map->mapsensor->max : aval;
	aval < this_map->mapsensor->min ? aval = this_map->mapsensor->min : aval;
	while(1){
		mav(this_map->mapmotor->port, this_map->mapmotor->min + (int)((float)(this_map->mapmotor->max - this_map->mapmotor->min) * (float)(aval - this_map->mapsensor->min) / (float)(this_map->mapsensor->max - this_map->mapsensor->min)));
		msleep(10L);
	}
}
void map_servo(analog_sensor this_sensor, servo this_servo){
	struct mapper *this_map = malloc(sizeof(struct mapper));
	this_servo->map_sensor = this_sensor;
	this_map->mapsensor = (void *)this_sensor;
	this_map->mapservo = (void *)this_servo;
	pthread_create(&this_map->thread_id, NULL, &map_servo_control, (void *)this_map);
}
void map_motor_speed(analog_sensor this_sensor, dcmotor this_motor){
	struct mapper *this_map = malloc(sizeof(struct mapper));
	this_motor->map_sensor = this_sensor;
	this_map->mapsensor = (void *)this_sensor;
	this_map->mapmotor = (void *)this_motor;
	pthread_create(&this_map->thread_id, NULL, &map_motor_control, (void *)this_map);
}
#endif
