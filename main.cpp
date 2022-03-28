#include "src/data-structures/queue/Queue.h"
#include <iostream>
#include <vector>

int main() {
    DS::Queue<int> q;

    q.push(0);

    q.print_queue();

    return 0;
}
