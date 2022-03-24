#include <iostream>

#include "src/data-structures/linked-lists/LinkedList.h"

int main() {

    DS::LinkedList<int> list;

    list.insert_node_at(2, 0);
    list.insert_node_at(3, 1);

    list.printList();

    return 0;
}
