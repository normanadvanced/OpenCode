#ifndef __CREATE_MUSIC_H__
#define __CREATE_MUSIC_H__
#include "create_config.h"

const int rest=30;
const int a=56;
const int aS=57;
const int b=58;
const int bS =59;
const int c=60;
const int cS=61;
const int d=62;
const int dS=63;
const int e=64;
const int f=65;
const int fS=66;
const int g=67;
const int gS=68;

//Convert ms to 64ths
int create_length(long msec)
{
	return((msec*64)/1000);
}
void create_load_onesong(int *gc_song)
{
	int i, length;
	
	CREATE_BUSY;
	serial_write_byte(140);
	serial_write_byte(*(gc_song++));
	length = *(gc_song);
	serial_write_byte(*(gc_song++));
	for( i = 0; i < length; i++)
	{
		serial_write_byte(*(gc_song++));
		serial_write_byte(*(gc_song++));
	}
	CREATE_FREE;
}

#endif
