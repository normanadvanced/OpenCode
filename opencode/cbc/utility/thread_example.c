#ifndef THREAD_EXAMPLE_C_INCLUDED
#define THREAD_EXAMPLE_C_INCLUDED

#include <stdio.h>
#include "kovan_thread.h>
void threaded_function()
{
    int i;
    for(i=0;i<500;i++)
    {
        printf("This is a thread\n");
        msleep(10);
    }
}
int main()
{
    thread print_thread = build_thread(threaded_function);

    start_thread(print_thread);

    msleep(1000);

    int i;
    for(i=0;i<100;i++)
    {
        printf("This is not a thread");
        msleep(10);
    }

    wait_for_thread(print_thread);
    printf("Done with the thread.");


}

#endif // THREAD_EXAMPLE_C_INCLUDED
