#ifndef TEMP_LIB_H_INCLUDED
#define TEMP_LIB_H_INCLUDED

#include <pthread.h>
pthread_mutex_t freeze_mem = PTHREAD_MUTEX_INITIALIZER;


struct motor_props
{
	int port, ID, is_frozen;
	pthread_t this_thread;
}motors[4];
void *freeze_motor_t(void *port_ptr)
{
    pthread_mutex_lock(freeze_mem);

    int temp=-1;
    port = (int)port_ptr;

    while(motors[port].is_frozen)
    {
        mav(port,temp);
        temp*=-1;
        msleep(10);
    }
    pthread_mutex_lock(freeze_mem);
}

void freeze_motor(int port)
{
    if(!motors[port].is_frozen)
    {
        if((motors[port] = pthread_create(&motors[port].this_thread, NULL, update_arm,(void*)port)))
        {
            printf("Threading Failure: %d\n", thread_num);
        }
        else
        {
            printf("Threading success\n");
        }
    }
    else
    {
        printf("Motor is already frozen");
    }
}

void unfreeze_motor(int port)
{
    motors[port].is_frozen=0;
}


#endif // TEMP_LIB_H_INCLUDED
