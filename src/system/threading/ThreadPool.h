//
// Created by joaoa on 05/04/2022.
//

#ifndef DEV_DYNAMO_THREADPOOL_H
#define DEV_DYNAMO_THREADPOOL_H

#include <iostream>
#include <thread>
#include <mutex>
#include <queue>
#include <memory>
#include <vector>
#include <utility>
#include <functional>
#include <condition_variable>
#include <stdexcept>
#include <cstdlib>

namespace sys
{
    class ThreadPool
    {
    public:
        /* CONSTRUCTOR/DESTRUCTOR */
        explicit ThreadPool(std::size_t thread_count = std::thread::hardware_concurrency());
        // The destructor will push a shutdown signal into the work queue.
        // The worker threads will interpret a nullptr as a shutdown signal and join with the main thread.
        ~ThreadPool();

        ThreadPool(const ThreadPool&) = delete;
        ThreadPool(ThreadPool&&) = delete;
        ThreadPool& operator = (const ThreadPool&) = delete;
        ThreadPool& operator = (ThreadPool&&) = delete;

        /* CUSTOM TYPES */
        using work_item_t = std::function<void(void)>;

        /* PUBLIC FUNCTIONS */
        // The do_work() function pushes a work_item onto the queue
        // The worker threads will wake up and pick up the signal to execute the tasks
        void do_work(work_item_t wi);

    private:
        /* MEMBER CUSTOM TYPES */
        using work_item_ptr_t = std::unique_ptr<work_item_t>;
        using work_queue_t = std::queue<work_item_ptr_t>;

        /* MEMBER VARIABLES */
        work_queue_t m_queue;
        std::mutex m_queue_lock;
        std::condition_variable m_condition;
        std::size_t number_of_threads;

        /* THREADS ARRAY */
        // This vector will store all the worker threads
        using threads_t = std::vector<std::thread>;
        threads_t m_threads;
    };
}



#endif //DEV_DYNAMO_THREADPOOL_H
