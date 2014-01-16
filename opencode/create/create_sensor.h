#ifndef __CREATE_SENSOR_H__
#define __CREATE_SENSOR_H__

#include "./create_config.h"

// structures used to track and cache Create data
struct _sensor_packet{
    float lastUpdate; //cpu clock time
    unsigned int data; // 4 bytes or less of data
};

struct create_side{
	struct create_wheel_properties{
		int last_requested_speed;
		float sprop;
	}wheel;
	struct cliffs_of_dover{
		int port, black, white, error;
		long timeout;
	}cliff;
	struct bump_me{
		long timeout;
	}bump;
}create_left, create_right;

struct _create_state{
    int createConnected;
    int createBusy;
    int normalizedAngle;
    int totalAngle;
    int accumulatedDistance;//sum of the increment distances
    int driveDirect;//1 if active command of each wheel
    int drive;//1 if speed and radius command is active
    int lspeed;//command last sent
    int rspeed;
    int radius;
    int speed;
    int leds[3];//The state of the create LEDs
    struct _sensor_packet bumpsWheelDrops;//packet 7, b4 caster, b3 lw, b2 rw, b1 lb, b0 rb
    struct _sensor_packet wall;//packet 8, 0 no wall, 1 wall
    struct _sensor_packet lcliff;//packet 9 left cliff sensor 0 no cliff, 1 cliff
    struct _sensor_packet lfcliff;//packet 10 left front cliff sensor 0 no cliff, 1 cliff
    struct _sensor_packet rfcliff;//packet 11 right front cliff sensor 0 no cliff, 1 cliff
    struct _sensor_packet rcliff;//packet 12 right cliff sensor 0 no cliff, 1 cliff
    struct _sensor_packet vWall;//packet 13 0 no wall seen, 1 there is a wall
    struct _sensor_packet LSDandWheelOvercurrents ;//packet 14;b4 LW, b3 RW, B2 LD2, b1 LD0, b0 LD1
    // packets 15-16 are unused
    struct _sensor_packet infrared;//packet 17; byte received from IR remote. 255 means no byte seen
    struct _sensor_packet buttons;//packet 18; b2 advance button is pressed, b0 play button
    struct _sensor_packet distance; //packet 19; 2 byte signed number in mm since last request
    struct _sensor_packet angle;//packet 20; in degrees since last request - 2 byte signed num
    struct _sensor_packet chargingState;//packet 21; 0=not charging; 1-reconditioning chargeing; 2 full charging; 3 trickle charging; 4 waiting; 5 charge fault
    struct _sensor_packet voltage;//packet 22; unsigned 2 byte in mV
    struct _sensor_packet current;// packet 23; signed 2 byte in mA
    struct _sensor_packet batteryTemp;//packet 24; temp in degrees Celcius (1 byte signed)
    struct _sensor_packet batteryCharge;//packet 25; 2byte no sign in mAh
    struct _sensor_packet batteryCapacity;//packet 26; estimated charge capacity of battery in mAh
    struct _sensor_packet wallSignal;//packet 27; analog value of wall sensor 0-4095
    struct _sensor_packet lcliffSignal;//packet 28 left cliff sensor 0-4095
    struct _sensor_packet lfcliffSignal;//packet 29 left front cliff sensor 0-4095
    struct _sensor_packet rfcliffSignal;//packet 30 right front cliff sensor 0-4095
    struct _sensor_packet rcliffSignal;//packet 31 right cliff sensor 0-4095
    struct _sensor_packet cargoBayDI;//packet 32; b4 baud rate change pin 15, b3 DI3 pin 6, b2 DI2-pin 18, b1 DI1 pin 5, b0 DI0 pin 17
    struct _sensor_packet cargoBayAI;//packet 33; 0-1024
    struct _sensor_packet chargingSource;//packet 34; b1 home base, b0 internal charger
    struct _sensor_packet OIMode;//packet 35;0=off,1=passive,2=safe,3=full
    struct _sensor_packet songNumber;//packet 36; currently selected song number
    struct _sensor_packet songPlaying;//packet 37;1=song is playing, 0 song not playing
    struct _sensor_packet numStreamPackets;//packet-38, number of streamed packets, 0-43, is returned
    struct _sensor_packet requestedVelocity;//packet 39; -500 to 500
    struct _sensor_packet requestedRadius;//packet 40; -32768 to 32767
    struct _sensor_packet requestedRVelocity;//packet 41; -500 to 500
    struct _sensor_packet requestedLVelocity;//packet 42; left wheel v -500 to 500
} ;

// This global is used by the user to store songs to be played on the Create
// The functions create_load_song and create_play_song are used to get the data
// from this global to the Create.  The user fills the global on their own.
extern int gc_song_array[16][33];

#define EVENT_WHEEL_DROP 1
#define EVENT_FRONT_WHEEL_DROP 2
#define EVENT_LEFT_WHEEL_DROP 3
#define EVENT_RIGHT_WHEEL_DROP 4
#define EVENT_BUMP 5
#define EVENT_LEFT_BUMP 6
#define EVENT_RIGHT_BUMP 7
#define EVENT_VIRTUAL_WALL 8
#define EVENT_WALL 9
#define EVENT_CLIFF 10
#define EVENT_LEFT_CLIFF 11
#define EVENT_FRONT_LEFT_CLIFF 12
#define EVENT_FRONT_RIGHT_CLIFF 13
#define EVENT_RIGHT_CLIFF 14
#define EVENT_HOME_BASE 15
#define EVENT_ADVANCE_BUTTON 16
#define EVENT_PLAY_BUTTON 17
#define EVENT_DIGITAL_INPUT(port) (18+port)
#define EVENT_OI_MODE_PASSIVE 22

//Create Wait for Light Sensor, plug into create bay ports 20 and 17
void create_wait_for_light()
{
	CREATE_BUSY;
	serial_write_byte(147);
	serial_write_byte(4);
	serial_write_byte(158);
	serial_write_byte(18);
	serial_write_byte(147);
	serial_write_byte(0);
	CREATE_FREE;
}
void create_wait_theta(int angle)
{
	CREATE_BUSY;
	serial_write_byte(157);
	serial_write_byte(get_high_byte(angle));
	serial_write_byte(get_low_byte(angle));
	CREATE_FREE;
}
void create_wait_length(int distance)
{
	CREATE_BUSY;
	serial_write_byte(156);
	serial_write_byte(get_high_byte(distance));
	serial_write_byte(get_low_byte(distance));
	CREATE_FREE;
}
void create_wait_duration(int dseconds)
{
	CREATE_BUSY;
	serial_write_byte(155);
	serial_write_byte(dseconds);
	CREATE_FREE;
}
void create_wait_sensor(int packet_id)
{
	CREATE_BUSY;
	serial_write_byte(158);
	serial_write_byte(packet_id);
	CREATE_FREE;
}
//move create at given wheel speeds until front bump sensor is hit
void create_drive_bump(int rvel,int lvel)
{
	CREATE_BUSY;
	serial_write_byte(145);
	serial_write_byte(get_high_byte(rvel));
	serial_write_byte(get_low_byte(rvel));
	serial_write_byte(get_high_byte(lvel));
	serial_write_byte(get_low_byte(lvel));
	serial_write_byte(158);
	serial_write_byte(EVENT_BUMP);
	CREATE_FREE;
}
void create_drive_touch(int rspeed, int lspeed, int rport, int lport)
{
	create_drive_direct(rspeed, lspeed);
	while(!digital(rport) || !digital(lport))
	{
		if(digital(rport))
		{
			create_drive_direct(rspeed / 10, lspeed);
			msleep(10);
		}
		if(digital(lport))
		{
			create_drive_direct(rspeed, lspeed / 10);
			msleep(10);
		}
	}
}
//sync the create movements with the controller
//when we write create bytes, all the bytes are sent at once
//the create then executes them sequentially
//to force the controller to wait for the create to finish
//(e.g. waiting for the create to get to an item before lifting the arm servo)
//call this fucntion to block the program
void create_sync()
{
	char buffer[1];
    char *bptr = buffer;

	int read_count = 0;
    int count = 1;

    //CREATE_BUSY;
    serial_write_byte(142);
    serial_write_byte(35);

    while(read_count == 0 || read_count == -1)
	{
		#ifdef __arm__
        read_count = create_read_block(buffer+0, count-read_count);
        msleep(2);
		#endif
	}

	//CREATE_FREE;
}
#endif
