#define CREATE_BUSY
#define CREATE_FREE
#define HIGH_BYTE(x) ((x & 0xFFFF) >> 8)
#define LOW_BYTE(x) (x & 0xFF)
#define serial_write_byte create_write_byte
#define serial_read create_read_block

#define get_high_byte HIGH_BYTE
#define get_low_byte LOW_BYTE

#define PI 3.14159
#define CREATE_WAITFORBUFFER(buffer,count,error) { if(create_read_block(buffer, count) < count) { printf("Create connection failed.");stateOfCreate.createBusy=0; return error; }}

#define twopi 6.28318531
