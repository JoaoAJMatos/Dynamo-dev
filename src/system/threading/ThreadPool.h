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
        ~ThreadPool();

        ThreadPool(const ThreadPool&) = delete;
        ThreadPool(ThreadPool&&) = delete;
        ThreadPool& operator = (const ThreadPool&) = delete;
        ThreadPool& operator = (ThreadPool&&) = delete;

        /* CUSTOM TYPES */
        using work_item_t = std::function<void(void)>;

        /* PUBLIC FUNCTIONS */
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
        using threads_t = std::vector<std::thread>;
        threads_t m_threads;
    };
}



#endif //DEV_DYNAMO_THREADPOOL_H
