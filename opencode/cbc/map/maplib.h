#ifndef __MAPLIB_H__
#define __MAPLIB_H__
#include "../sensor/sensorlib.h"
#include "../servo/servolib.h"
#include "../motor/motorlib.h"
void map_servo(analog_sensor this_sensor, servo this_servo){
	this_servo->map_sensor = this_sensor;
	int aval = analog10(this_sensor->port);
	aval > this_servo->map_sensor->max ? aval = this_servo->map_sensor->max : aval;
	aval < this_servo->map_sensor->min ? aval = this_servo->map_sensor->min : aval;
	set_servo_position(this_servo->port, this_servo->min + (int)((float)(this_servo->max - this_servo->min) * (float)(aval - this_sensor->min) / (float)(this_sensor->max - this_sensor->min)));
}
void map_motor_speed(analog_sensor this_sensor, dcmotor this_motor){
	int aval = analog10(this_sensor->port);
	aval > this_sensor->max ? aval = this_sensor->max : aval;
	aval < this_sensor->min ? aval = this_sensor->min : aval;
	mav(this_motor->port, this_motor->min + (int)((float)(this_motor->max - this_motor->min) * (float)(aval - this_sensor->min) / (float)(this_sensor->max - this_sensor->min)));
	msleep(10L);
}
#endif
