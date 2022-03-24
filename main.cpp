#include <iostream>

#include "src/data-structures/queue/Queue.h"

int main() {

    DS::Queue<int> q;

    q.push(2);
    q.push(3);

    q.print_queue();

    int value = q.pop();

    q.print_queue();


    int value2 = q.pop();

    q.print_queue();


    std::cout << "Value: " << value << std::endl;
    std::cout << "Value2: " << value2 << std::endl;


    return 0;
}
