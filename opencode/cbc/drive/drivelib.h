#ifndef __DRIVELIB_H__
#define __DRIVELIB_H__
#include <stdio.h>
#include <pthread.h>
#define PI 3.14159
#define DEG_TO_RAD (PI / 180.0)
struct cbc_side{
	struct wheel_properties{
		int port, last_requested_speed;
		long ticks_cycle;
		float sprop, wheel_diameter, radial_distance;
	}wheel;
	struct lincolns_tophat{
		int port, black, white, error;
		long timeout;
	}tophat;
	struct touch_me{
		int port;
		long timeout;
	}touch;
}l, r;
void build_left_wheel(int port, long ticks_cycle, float sprop, float wheel_diameter, float radial_distance){
	l.wheel.port = port;
	l.wheel.ticks_cycle = ticks_cycle;
	l.wheel.sprop = sprop;
	l.wheel.wheel_diameter = wheel_diameter;
	l.wheel.radial_distance = radial_distance;
}
void build_right_wheel(int port, long ticks_cycle, float sprop, float wheel_diameter, float radial_distance){
	r.wheel.port = port;
	r.wheel.ticks_cycle = ticks_cycle;
	r.wheel.sprop = sprop;
	r.wheel.wheel_diameter = wheel_diameter;
	r.wheel.radial_distance = radial_distance;
}
void build_left_tophat(int port, int white, int black, int error, long timeout){
	l.tophat.port = port;
	l.tophat.white = white;
	l.tophat.black = black;
	l.tophat.error = error;
	l.tophat.timeout = timeout;
}
void build_right_tophat(int port, int white, int black, int error, long timeout){
	r.tophat.port = port;
	r.tophat.white = white;
	r.tophat.black = black;
	r.tophat.error = error;
	r.tophat.timeout = timeout;
}
void build_left_touch(int port, long timeout){
	r.touch.port = port;
	r.touch.timeout = timeout;
}
void build_right_touch(int port, long timeout){
	r.touch.port = port;
	r.touch.timeout = timeout;
}
void cbc_wait(){
	bmd(l.wheel.port);
	bmd(r.wheel.port);
}
void cbc_halt(){
	off(l.wheel.port);
	off(r.wheel.port);
}
void cbc_stop(){
	cbc_wait();
	cbc_halt();
}

int cbc_direct(int lspeed, int rspeed){
	mav(l.wheel.port, lspeed);
	mav(r.wheel.port,rspeed);
	l.wheel.last_requested_speed = lspeed;
	r.wheel.last_requested_speed = rspeed;
	return 1;
}
void cbc_straight(int speed, float distance){
	if((float)speed / distance < 0)
	{
		speed *= -1;
	}
	float lticks = (distance * l.wheel.ticks_cycle) / (l.wheel.wheel_diameter * PI);
	float rticks = (distance * r.wheel.ticks_cycle) / (r.wheel.wheel_diameter * PI);
	float lspeed = (float)speed * l.wheel.sprop;
	float rspeed = (float)speed * r.wheel.sprop;
	if(rspeed > 1000 || lspeed > 1000 || rspeed < -1000 || lspeed < -1000){
		printf("\nWarning! Invalid Speed\n");
	}
	else{
		mrp(l.wheel.port, (int)(lspeed), (int)(lticks));
		mrp(r.wheel.port, (int)(rspeed), (int)(rticks));
		cbc_wait();
		l.wheel.last_requested_speed = (int)lspeed;
		r.wheel.last_requested_speed = (int)rspeed;
	}
}
int cbc_arc(int speed, float radius, float theta)
{
	if((float)speed * (radius / theta) < 0){
		speed *= -1;
	}
	float arc_length = radius * theta * DEG_TO_RAD;
	float ldistance = (radius - l.wheel.radial_distance) * theta * DEG_TO_RAD;
	float rdistance = (radius + r.wheel.radial_distance) * theta * DEG_TO_RAD;
	float lticks = (ldistance * l.wheel.ticks_cycle) / (l.wheel.wheel_diameter * PI);
	float rticks = (rdistance * r.wheel.ticks_cycle) / (r.wheel.wheel_diameter * PI);
	float lspeed = (float)speed * l.wheel.sprop * ldistance / arc_length;
	float rspeed = (float)speed * r.wheel.sprop * rdistance / arc_length;
	if(rspeed > 1000 || lspeed > 1000 || rspeed < -1000 || lspeed < -1000){
		printf("\nWarning! Invalid Speed, Please Lower\n");
	}
	if(radius < l.wheel.radial_distance || radius < r.wheel.radial_distance){
		printf("\nWarning! Invalid Radius, Please Raise\n");
	}
	else{
		mrp(l.wheel.port, (int)(lspeed), (int)(lticks));
		mrp(r.wheel.port, (int)(rspeed), (int)(rticks));
		cbc_wait();
		l.wheel.last_requested_speed = (int)lspeed;
		r.wheel.last_requested_speed = (int)rspeed;
	}
}
void cbc_spin(int speed, float theta){
	if((float)speed * theta < 0){
		speed *= -1;
	}
	float ldistance = -1.0 * l.wheel.radial_distance * theta * DEG_TO_RAD;
	float rdistance = r.wheel.radial_distance * theta * DEG_TO_RAD;
	float lticks = (ldistance * l.wheel.ticks_cycle) / (l.wheel.wheel_diameter * PI);
	float rticks = (rdistance * r.wheel.ticks_cycle) / (r.wheel.wheel_diameter * PI);
	float lspeed = (float)speed * l.wheel.sprop * l.wheel.radial_distance / (l.wheel.radial_distance + l.wheel.radial_distance);
	float rspeed = (float)speed * r.wheel.sprop * r.wheel.radial_distance / (r.wheel.radial_distance + r.wheel.radial_distance);
	if(rspeed > 1000 || lspeed > 1000 || rspeed < -1000 || lspeed < -1000){
		printf("\nWarning! Invalid Speed, Please Lower\n");
	}
	else{
		mrp(l.wheel.port, (int)(lspeed), (int)(lticks));
		mrp(r.wheel.port, (int)(rspeed), (int)(rticks));
		cbc_wait();
		l.wheel.last_requested_speed = (int)lspeed;
		r.wheel.last_requested_speed = (int)rspeed;
	}
}
int cbc_left_touch(){
	return(digital(l.touch.port));
}
int cbc_right_touch(){
	return(digital(r.touch.port));
}
void cbc_align_touch(){
	long ltimeout = l.touch.timeout;
	long rtimeout = r.touch.timeout;
	while(ltimeout > 0 && rtimeout > 0 && (!cbc_left_touch() || !cbc_right_touch())){
		ltimeout -= 10L;
		rtimeout -= 10L;
	}
	cbc_halt();
}
int cbc_left_tophat(){
	if(analog10(l.tophat.port) > l.tophat.black - l.tophat.error) return(1);
	else if(analog10(l.tophat.port) < l.tophat.white + l.tophat.error) return(0);
}
int cbc_right_tophat(){
	if(analog10(r.tophat.port) > r.tophat.black - r.tophat.error) return(1);
	else if(analog10(r.tophat.port) < r.tophat.white + r.tophat.error) return(0);
}
void cbc_align_black(){
	long ltimeout = l.tophat.timeout;
	long rtimeout = r.tophat.timeout;
	mav(l.wheel.port, l.wheel.last_requested_speed);
	mav(r.wheel.port, r.wheel.last_requested_speed);
	while(ltimeout > 0 && rtimeout > 0 && (!cbc_right_tophat() || !cbc_left_tophat())){
		if(analog10(l.tophat.port) > (l.tophat.black - l.tophat.error)){
			off(l.wheel.port);
		}
		if(analog10(r.tophat.port) > (r.tophat.black - r.tophat.error)){
			off(r.wheel.port);
		}
		msleep(10L);
		ltimeout -= 10L;
		rtimeout -= 10L;
	}
	cbc_halt();
}
void cbc_align_white(){
	long ltimeout = l.tophat.timeout;
	long rtimeout = r.tophat.timeout;
	set_analog_floats(0);
	mav(l.wheel.port, l.wheel.last_requested_speed);
	mav(r.wheel.port, r.wheel.last_requested_speed);
	while(ltimeout > 0 && rtimeout > 0 && (cbc_right_tophat() || cbc_left_tophat())){
		if(analog10(l.tophat.port) < (l.tophat.white + l.tophat.error)){
			off(l.wheel.port);
		}
		if(analog10(r.tophat.port) < (r.tophat.white + r.tophat.error)){
			off(r.wheel.port);
		}
		msleep(10L);
		ltimeout -= 10L;
		rtimeout -= 10L;
	}
	cbc_halt();
}
#endif
