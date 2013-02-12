#ifndef ARM_H_INCLUDED
#define ARM_H_INCLUDED

#define ROT_POT 0
#define LEFT_MOTOR 1
#define RIGHT_MOTOR 3

#include "/usr/include/pthread.h"
#include <math.h>
#include <malloc.h>
#include "/usr/include/kovan/kovan.h"
#include "/usr/include/kovan/thread.h"

pthread_mutex_t arm_mem = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t freeze_mem = PTHREAD_MUTEX_INITIALIZER;
int thread_num;
int position;

void *update_arm(void *p)
{
    pthread_mutex_lock(&arm_mem);
    while(1)
    {
        while(analog10(ROT_POT)<position-15)
        {
            printf("1");
            mav(RIGHT_MOTOR,300);
            mav(LEFT_MOTOR,300);
            msleep(100);
        }
        freeze(RIGHT_MOTOR);
        freeze(LEFT_MOTOR);
        while(analog10(ROT_POT)>position+15)
        {
            printf("2");
            mav(RIGHT_MOTOR,-900);
            mav(LEFT_MOTOR,-900);
            msleep(100);
        }
        freeze(RIGHT_MOTOR);
        freeze(LEFT_MOTOR);
        msleep(100);
    }
    pthread_mutex_unlock(&arm_mem);
}

void set_arm_pos(int pos)
{
    position = pos;
}
int arm_position()
{
    return position;
}

void wait_for_arm(int pos)
{
    while(!(arm_position()<pos+15 && arm_position()>pos-15))
        msleep(10);
}
void start_arm()
{
    printf("Attempting thread");
    pthread_t t_thread;
    if((thread_num = pthread_create(&t_thread, NULL, update_arm,(void*)0)))
	{
		printf("Threading Failure: %d\n", thread_num);
	}
	else
	{
       printf("Threading success\n");
	}
}

#endif // MOTOR_H_INCLUDED
