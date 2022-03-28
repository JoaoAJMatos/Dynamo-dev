//
// Created by joaoa on 24/03/2022.
//
// The Node class serves as the blueprint for all Nodes in a linked list.
// The Node class contains a data object which stores the data of the node, this can be of any type.
//
// The type is defined according to the type of the class on which the Node class is being instantiated upon.
//
// Meaning that, if the Node class is instantiated inside a LinkedList class of type int (LinkedList<int>), the
// data type will be int as well.
//
// The node class also contains a pointer to the next node in the list.

#ifndef DEV_DYNAMO_NODE_H
#define DEV_DYNAMO_NODE_H

#include <iostream>
#include <cstdio>

namespace DS
{
    template <typename T>

    class Node {
    private:
        /* MEMBER VARIABLES */
        T data; // Data variable that stores the node data
        Node* next; // Pointer to the next node
        Node* previous; // Pointer to the previous node

    public:
        /* CONSTRUCTORS */
        // Default constructor
        Node()
        {
            data = NULL;
            next = nullptr;
            previous = nullptr;
        }

        // Parameterized constructor
        explicit Node(T data)
        {
            this->data = data;
            this->next = nullptr;
            this->previous = nullptr;
        }

        /* GETTERS & SETTERS*/
        T get_data()
        {
            return data;
        }

        Node* get_next()
        {
            return next;
        }

        Node* get_previous()
        {
            return previous;
        }

        void set_next(Node* input_next)
        {
            next = input_next;
        }

        void set_previous(Node* input_prev)
        {
            previous = input_prev;
        }
    };
}

#endif //DEV_DYNAMO_NODE_H
