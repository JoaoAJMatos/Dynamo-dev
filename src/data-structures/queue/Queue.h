//
// Created by joaoa on 24/03/2022.
//
//
// The Queue class serves as the blueprint for building a queue data structure.
//
// The datatype for the queue must be specified when creating an instance, like this: Queue<int> q;
//
// The Queue class allows you to push and pop values from an underlying Linked List, where pushing
// means adding a value to the end of the list, and popping means getting and removing the first value
// in the list; just like a queue (FIFO)

#ifndef DEV_DYNAMO_QUEUE_H
#define DEV_DYNAMO_QUEUE_H

#include "../linked-lists/LinkedList.h"

namespace DS
{
    template <typename T>

    class Queue: private LinkedList<T>
    {
    private:
        /* MEMBER VARIABLES */
        LinkedList<T> l;

    public:
        // Returns the first value in the queue and removes it
        T pop()
        {
            // Get the value at index 0
            T data = l.retrieve(0);
            // Remove the node at index 0
            l.deleteNode(0);
            // Return the data in that node
            return data;
        }

        // Places a designated value at the end of the queue
        void push(T data)
        {
            // Inserts a new node with the given data at the last index of the list
            // We don't need to worry about subtracting 1 from length because insert_node_at()
            // from LinkedList.h does that for us
            l.insert_node_at(data, l.len());
        }

        // Prints the contents of the queue to the screen
        void print_queue()
        {
            l.printList();
        }
    };
}

#endif //DEV_DYNAMO_QUEUE_H
