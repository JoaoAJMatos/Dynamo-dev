//
// Created by Matos on 4/3/2022.
//

#ifndef DEV_DYNAMO_THREADPOOL_H
#define DEV_DYNAMO_THREADPOOL_H

#include <pthread.h>

#include "../../data-structures/queue/Queue.h"
#include "common/ThreadJob.h"

class ThreadPool
{
private:
    /* MEMBER VARIABLES */
    int num_threads;
    // A control switch for the thread pool
    bool active;
    // A queue to store work
    DS::Queue<Threading::ThreadJob> work;

    // Mutices for making the pool thread-safe
    pthread_t* pool;
    pthread_mutex_t lock;
    pthread_cond_t signal;

    static void* generic_thread_function(void* arg);

public:
    /* CONSTRUCTOR/DESTRUCTOR */
    // The ThreadPool class needs a parameterized constructor that takes in the number of threads as an argument
    // The constructor will create a thread pool with the desired number of threads
    ThreadPool(int thread_count);
    ~ThreadPool();


    // A function for safely adding work to the queue
    void add_work(Threading::ThreadJob);
};


#endif //DEV_DYNAMO_THREADPOOL_H
