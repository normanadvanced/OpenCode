#ifndef KOVAN_THREAD_H_INCLUDED
#define KOVAN_THREAD_H_INCLUDED

#include <pthread.h>
#include <malloc.h>
#include "/usr/include/kovan/kovan.h"

pthread_mutex_t thread_mem = PTHREAD_MUTEX_INITIALIZER;

struct thread_properties
{
    pthread_t this_thread;
    int ID;
    int is_running;
    int return_val;
    void* func;
};

typedef struct thread_properties *k_thread;

k_thread build_thread(void func)
{
    thread t_thread = malloc(sizeof(struct thread_properties));

    t_thread->ID=-1;
    t_thread->is_running=0;
    t_thread->func=(*void)func;
    return t_thread;

}


//starts a thread
void start_thread(k_thread t)
{
    pthread_mutex_lock(&thread_mem);
    if((t->ID = pthread_create(&t->this_thread, NULL, &t->func, (void *)0)))
	{
		printf("Threading Failure: %d\n", thread_num);
	}
	else
	{
	    t->is_running=1;
		t->ID = t.this_thread;
	}
    pthread_mutex_unlock(&thread_mem);
}

void wait_for_thread(k_thread t)
{
    pthread_join(t.thread,null);
}
void stop_thread(k_thread t)
{
    pthread_exit(t.this_thread.thread_return);
}
//returns 0 if not running
//1 if running
int status(thread t)
{
    return t->is_running;
}
#endif // KOVAN_THREAD_H_INCLUDED
