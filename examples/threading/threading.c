#include "threading.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

// Optional: use these functions to add debug or error prints to your application
//#define DEBUG_LOG(msg,...)
#define DEBUG_LOG(msg,...) printf("threading: " msg "\n" , ##__VA_ARGS__)
#define ERROR_LOG(msg,...) printf("threading ERROR: " msg "\n" , ##__VA_ARGS__)


void* threadfunc(void* thread_param)
{
    struct thread_data* thread_func_args = (struct thread_data *) thread_param;
    struct timespec wait_time={0,thread_func_args->wait_to_obtain*1000000};
    int ret;
    thread_func_args->thread_complete_success=true;

    ret = nanosleep(&wait_time,NULL);
    if (ret == -1) thread_func_args->thread_complete_success=false;
    pthread_mutex_lock(thread_func_args->mutex);
    wait_time.tv_nsec=thread_func_args->wait_to_release*1000000;
    ret = nanosleep(&wait_time,NULL);
    if (ret == -1) thread_func_args->thread_complete_success=false;
    pthread_mutex_unlock(thread_func_args->mutex);

    // TODO: wait, obtain mutex, wait, release mutex as described by thread_data structure
    // hint: use a cast like the one below to obtain thread arguments from your parameter
    return thread_param;
}


bool start_thread_obtaining_mutex(pthread_t *thread, pthread_mutex_t *mutex,int wait_to_obtain_ms, int wait_to_release_ms)
{
    //pthread_mutex_init(mutex,NULL);
    //allocate data for the struct
    struct thread_data *thread_d_ptr = (struct thread_data*) malloc(sizeof(struct thread_data));
    //assign values.
    if (thread_d_ptr == NULL)
    {
        ERROR_LOG("Memory allocation malloc failed to allocate %zu bytes",sizeof(struct thread_data));
        return false;
    }
    thread_d_ptr->mutex = mutex;
    thread_d_ptr->wait_to_obtain= wait_to_obtain_ms;
    thread_d_ptr->wait_to_release= wait_to_release_ms;

    int rc = pthread_create(thread,NULL,&threadfunc,thread_d_ptr);

    if (rc != 0)
    {
        ERROR_LOG("Thread creation failed");
        return false;
    }
    
    /**
     * TODO: allocate memory for thread_data, setup mutex and wait arguments, pass thread_data to created thread
     * using threadfunc() as entry point.
     *
     * return true if successful.
     *
     * See implementation details in threading.h file comment block
     */
    return true;
}

