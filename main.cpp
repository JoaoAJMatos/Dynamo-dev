#include <iostream>

#include "src/system/threading/ThreadPool.h"


void  hello()
{
    std::cout << "Hello" << std::endl;
}

int main()
{
    sys::ThreadPool tp(1);

    tp.do_work([&, hello = &hello]() {
        hello();
    });

}
