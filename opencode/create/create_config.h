// Created on Wed Jul 4 2012

#ifndef __CREATE_CONFIG_H__ // Change FILE to your file's name
#define __CREATE_CONFIG_H__

#ifdef __arm__
#define serial_write_byte create_write_byte
#define serial_read create_read_block
extern struct _create_state stateOfCreate;
#else
#define CREATE_BUSY
#define CREATE_FREE
#define HIGH_BYTE(x) ((x & 0xFFFF) >> 8)
#define LOW_BYTE(x) (x & 0xFF)
#endif

#define get_high_byte HIGH_BYTE
#define get_low_byte LOW_BYTE

#define PI 3.14159


#endif
