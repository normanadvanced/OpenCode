#include "create_script.h"
#include "create_sensor.h"

void create_align_back_CW(unsigned int speed)
{
	script_write_byte(145);
	script_write_byte(get_high_byte(-speed));
	script_write_byte(get_low_byte(-speed));
	script_write_byte(get_high_byte(-speed));
	script_write_byte(get_low_byte(-speed));
	
	serial_write_byte(158);
	serial_write_byte(EVENT_LEFT_CLIFF);
	
	script_write_byte(145);
	script_write_byte(get_high_byte(-speed));
	script_write_byte(get_low_byte(-speed));
	script_write_byte(get_high_byte(0));
	script_write_byte(get_low_byte(0));
	
	serial_write_byte(158);
	serial_write_byte(EVENT_RIGHT_CLIFF);
}