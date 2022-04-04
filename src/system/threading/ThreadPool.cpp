//
// Created by Matos on 4/3/2022.
//

#include "ThreadPool.h"

/* CONSTRUCTOR */
ThreadPool::ThreadPool(int thread_count)
{
    num_threads = thread_count;
    active = true;

    // Initialize the pthread mutices
    lock = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
    signal = (pthread_cond_t)PTHREAD_COND_INITIALIZER;
    pthread_mutex_lock(&lock);

    for (int i = 0; i < num_threads; i++)
    {
        pthread_create(&pool[i], NULL, &ThreadPool::generic_thread_function, )
    }
}

/* MEMBER FUNCTIONS */
// The generic_thread_function() is required as the argument for creating each thread in the pool
// It allows each thread to await a ThreadJob and execute its contents
// This makes the pool dynamic - any function can be passed as a job
void *ThreadPool::generic_thread_function(void *arg)
{
    while(active)
    {
        // Lock the work queue
        pthread_mutex_lock(&lock);
        pthread_cond_wait(&signal, &lock);
        // Get the job from the queue
        Threading::ThreadJob job = work.pop();
        // Unlock the queue
        pthread_mutex_unlock(&lock);

        // Execute the job
        if (job.job)
        {
            job.job(job.arg);
        }
    }
    return nullptr;
}
