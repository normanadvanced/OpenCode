#ifndef __MAPLIB_H__
#define __MAPLIB_H__
#include "../sensor/sensorlib.h"
#include "../servo/servolib.h"
#include "../motor/motorlib.h"
struct mapper
{
	analog_sensor mapsensor;
	servo mapservo;
	pthread_t thread_id;
};
void *map_servo_control(void *ptr)
{
	struct mapper *this_map = (struct mapper *) ptr;
	int aval = analog10(this_map->mapsensor->port);
	aval > this_map->mapsensor->max ? aval = this_map->mapsensor->max : aval;
	aval < this_map->mapsensor->min ? aval = this_map->mapsensor->min : aval;
	set_servo_position(this_map->mapservo->port, this_map->mapservo->min + (int)((float)(this_map->mapservo->max - this_map->mapservo->min) * (float)(aval - this_map->mapservo->min) / (float)(this_map->mapservo->max - this_map->mapservo->min)));
}
void map_servo(analog_sensor this_sensor, servo this_servo){
	struct mapper *this_map = malloc(sizeof(struct mapper));
	this_servo->map_sensor = this_sensor;
	this_map->mapsensor = (void *)this_sensor;
	this_map->mapservo = (void *)this_servo;
	pthread_create(&this_map->thread_id, NULL, &map_servo_control, (void *)this_map);
}
void map_motor_speed(analog_sensor this_sensor, dcmotor this_motor){
	int aval = analog10(this_sensor->port);
	aval > this_sensor->max ? aval = this_sensor->max : aval;
	aval < this_sensor->min ? aval = this_sensor->min : aval;
	mav(this_motor->port, this_motor->min + (int)((float)(this_motor->max - this_motor->min) * (float)(aval - this_sensor->min) / (float)(this_sensor->max - this_sensor->min)));
	msleep(10L);
}
#endif
