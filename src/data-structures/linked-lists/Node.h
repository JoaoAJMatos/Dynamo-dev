//
// Created by joaoa on 24/03/2022.
//
// The Node class serves as the blueprint for all Nodes in a linked list
//


#ifndef DEV_DYNAMO_NODE_H
#define DEV_DYNAMO_NODE_H

#include <iostream>

namespace DS
{
    template <typename T>

    class Node {
    private:


    public:
        T data;     // Data variable that stores the node data
        Node* next; // Pointer to the next node

        /* CONSTRUCTOR */
        // Default constructor
        Node()
        {
            data = NULL;
            next = nullptr;
        }
        // Parameterized constructor
        explicit Node(T data)
        {
            this->data = data;
            this->next = nullptr;
        }

    };
}

#endif //DEV_DYNAMO_NODE_H
