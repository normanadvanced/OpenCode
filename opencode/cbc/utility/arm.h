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
int thread_num;
pthread_t t_thread;
void *raise_arm_t()
{
    pthread_mutex_lock(&arm_mem);

    printf("Raising\n");
    mav(RIGHT_MOTOR,-1000);
    mav(LEFT_MOTOR,-1000);
    while(analog10(ROT_POT)>UP_POS)
    {
        msleep(3);
    }
    printf("Raised\n");
    motor(RIGHT_MOTOR,-10);
    motor(LEFT_MOTOR,-10);

    pthread_mutex_unlock(&arm_mem);
}

void *set_arm_t()
{
    pthread_mutex_lock(&arm_mem);
    printf("setting\n");
    if(analog10(ROT_POT)>SCORE_POS)
    {
        mav(RIGHT_MOTOR,-500);
        mav(LEFT_MOTOR,-500);
    }
    while(analog10(ROT_POT)>SCORE_POS)
    {
        msleep(5);
    }
    off(RIGHT_MOTOR);
    off(LEFT_MOTOR);
    if(analog10(ROT_POT)<SCORE_POS)
    {
        mav(RIGHT_MOTOR,200);
        mav(LEFT_MOTOR,200);
    }
    while(analog10(ROT_POT)<SCORE_POS)
    {
        msleep(5);
    }
    off(RIGHT_MOTOR);
    off(LEFT_MOTOR);
    printf("set\n");
    pthread_mutex_unlock(&arm_mem);
}
void *lower_arm_t()
{
    pthread_mutex_lock(&arm_mem);
    printf("Lowering\n");
    mav(RIGHT_MOTOR,400);
    mav(LEFT_MOTOR,400);
    while(analog10(ROT_POT)<DOWN_POS)
    {
        msleep(5);
    }
    printf("Lowered\n");
    off(RIGHT_MOTOR);
    off(LEFT_MOTOR);
    pthread_mutex_unlock(&arm_mem);
}
void raise_arm()
{
    printf("Attempting thread");
    if((thread_num = pthread_create(&t_thread, NULL, raise_arm_t,(void*)0)))
	{
		printf("Threading Failure: %d\n", thread_num);
	}
	else
	{
       printf("Threading success\n");
	}
}

void set_arm()
{
    printf("Attempting thread");
    if((thread_num = pthread_create(&t_thread, NULL, set_arm_t,(void*)0)))
	{
		printf("Threading Failure: %d\n", thread_num);
	}
	else
	{
       printf("Threading success\n");
	}
}
void wait_for_arm()
{
    pthread_join(t_thread, NULL);
}

void wait_for_arm_pos(int pos)
{
    while(analog10(ROT_POT)>pos)
        msleep(10);
}
void lower_arm()
{
    printf("Attempting thread");
    if((thread_num = pthread_create(&t_thread, NULL, lower_arm_t,(void*)0)))
	{
		printf("Threading Failure: %d\n", thread_num);
	}
	else
	{
       printf("Threading success\n");
	}
}

#endif // MOTOR_H_INCLUDED
