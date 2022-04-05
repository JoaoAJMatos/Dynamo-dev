//
// Created by joaoa on 05/04/2022.
//

#include "ThreadPool.h"


/* CONSTRUCTOR/DESTRUCTOR */
sys::ThreadPool::ThreadPool(std::size_t thread_count)
{
    // Throw an exception if the user tries to create a pool with less than 0 worker threads
    if(thread_count < 0) throw std::invalid_argument("Thread count must not be less than 0");
    // If the thread count is value, update the `number_of_threads`. In case the value is 0, set the number of threads to the default value.
    else if(thread_count == 0) number_of_threads = std::thread::hardware_concurrency();
    else number_of_threads = thread_count;

    m_threads.reserve(number_of_threads);

    for(auto i = 0; i < number_of_threads; ++i)
    {
        m_threads.push_back(std::thread(
                [this](){
                    while(true)
                    {
                        work_item_ptr_t work{nullptr};
                        {
                            std::unique_lock<std::mutex> guard(m_queue_lock);
                            m_condition.wait(guard, [&]() { return !m_queue.empty(); });
                            work = std::move(m_queue.front());
                            m_queue.pop();
                        }
                        if(!work)
                        {
                            break;
                        }

                        (*work)();
                    }
                }));
    }
}

// The destructor will push a shutdown signal into the work queue.
// The worker threads will interpret a nullptr as a shutdown signal.
sys::ThreadPool::~ThreadPool()
{
    {
        // Lock the queue
        std::unique_lock<std::mutex> guard(m_queue_lock);
        // Push x shutdown signals to the queue, where x is the number of threads
        for(auto& t : m_threads)
        {
            m_queue.push(work_item_ptr_t{nullptr});
        }
    }

    // Join the threads
    for(auto& t : m_threads)
    {
        t.join();
    }
}


/* PUBLIC FUNCTIONS */
void sys::ThreadPool::do_work(work_item_t wi)
{
    auto work_item = std::make_unique<work_item_t>(std::move(wi));
    {
        std::unique_lock<std::mutex> guard(m_queue_lock);
        m_queue.push(std::move(work_item));
    }

    m_condition.notify_one();
}

