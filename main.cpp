#include <iostream>

#include "src/system/threading/ThreadPool.h"

int main()
{
    using namespace std;
    mutex cout_guard;

    cout << "main thread ID: " << this_thread::get_id() << endl;

    sys::ThreadPool tp(10);

    for(auto i = 1; i <= 10; i++)
        tp.do_work([&, i = i]()
                   {
                       {
                           unique_lock<std::mutex> guard(cout_guard);
                           cout << "doing work " << i << "..." << endl;
                       }
                   });
}
