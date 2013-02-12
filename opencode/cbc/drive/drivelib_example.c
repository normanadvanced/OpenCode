#include <stdio.h>

#include "drivelib.h"

int main()
{
	build_lwheel(3, 1110, .989, 25.3, 38.0);
	//Build the left wheel, It has 1110 Ticks per rotation, its speed is 98.9 percent of the requested speed (Drift Compensation)
	//Its wheel diameter on the board is 25.3 mm, its distance from the center of the robot along the axleis 38.0 mm

	build_rwheel(1, 1070, 1.05, 25.3, 40.0);
	//Build the right wheel, It has 1070 Ticks per rotation, its speed is 100.5 percent of the requested speed (Drift Compensation)
	//Its wheel diameter on the board is 25.3 mm, its distance from the center of the robot along the axleis 40.0 mm

	cbc_straight(500, 1000.0);
	//Drive straight at requested speed 500 for +1000.0 mm (1.0 m Forward)

	wait_cbc();
	//Wait for the cbc to finish its movement

	cbc_arc(400, 300.0, 90.0);
	//Drive in an arc at requested speed 400 in an arc of +300.0 mm for +90.0 degrees (Forward, CCW)

	wait_cbc();
	//Wait for the cbc to finish its movement

	cbc_spin(350, 90.0);
	//Spin at requested speed 350 for +90.0 degrees (CCW)

	wait_cbc();
	//Wait for the cbc to finish its movement

	cbc_arc(400, -300.0, 90.0);
	//Drive in an arc at requested speed 400 in an arc of -300.0 mm for +90.0 degrees (Backwards, CCW)
}
