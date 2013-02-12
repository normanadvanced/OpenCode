#include <stdio.h>

#include"servolib.h" //This is the Modular Servo Library we are using to control Servo Movement

void drive_to_botguy(){};
void drive_to_scoring_area(){};

int main() 
{
	servo arm = build_servo(1, 0, 2048);
	//Build a 'servo' named 'arm'
	//It is in Port #1, Its Minimum Position is 0, Its Maximum Position is 2048
	servo claw = build_servo(3, 300, 1500);
	//Build a 'servo' named 'claw'
	//It is in Port #3, Its Minimum Position is 300, Its Maximum Position is 1500
	
	servo_movement up = build_servo_movement(100, 15, 6);
	//Build a 'servo_movement' named 'up'
	//Its desired position is 100 Ticks, It moves 15 ticks each movement with a Latency of 6 ms
	servo_movement down = build_servo_movement(2000, 8, 10);
	//Build a 'servo_movement' named 'down'
	//Its desired position is 2000 Ticks, It moves 8 ticks each movement with a Latency of 10 ms
	servo_movement open = build_servo_movement(400, 20, 5);
	//Build a 'servo_movement' named 'open'
	//Its desired position is 400 Ticks, It moves 20 ticks each movement with a Latency of 5 ms
	servo_movement close = build_servo_movement(1400, 12, 6);
	//Build a 'servo_movement' named 'close'
	//Its desired position is 1400 Ticks, It moves 12 ticks each movement with a Latency of 6 ms
	
	move_servo(arm, up);
	//Move the 'arm' servo to the 'up' position
	move_servo(claw, open);
	//Move the 'claw' servo to the open position
	
	//NOTE: The 'claw' and 'arm' servos do not wait to finish
	//NOTE: This code would immediately execute the 'drive_to_botguy()' function
	//NOTE:	The 'claw' and 'arm'would move while the robot is still moving
	
	drive_to_botguy();
	//Drive the robot to botguy
	
	wait_servo(claw, close);
	//Wait until the 'claw' servo is at the 'close' position
	//NOTE: This function waits until the the servo is done moving to execute the next function
	//NOTE: This is the same as calling 'move_servo(claw, close);' and then 'bsd(claw);'
	
	drive_to_scoring_area();
	//Drive to the scoring area
	
	wait_servo(arm, down);
	//Wait until the 'arm' servo is at the 'down' position
	
	wait_servo(claw, open);
	//Wait until the 'claw' servo is at the 'open' position
	
	return 0;
	//Returns '0' to the system
}
