#ifndef __DEPTH_H__
#define __DEPTH_H__

struct depth_attributes{
	double dist_x, dist_z, theta, error;
}depth_sensor;

typedef struct depth_attributes *depth_sensor_type;

depth_sensor_type build_depth(double dist_x, double dist_z, double theta, double error){
	depth_sensor.dist_x = dist_x;
	depth_sensor.dist_z = dist_z;
	depth_sensor.theta = theta;
	depth_sensor.error = error;
	return(&depth_sensor);
}

#endif
