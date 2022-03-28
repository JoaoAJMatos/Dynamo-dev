//
// Created by joaoa on 24/03/2022.
//

#ifndef DEV_DYNAMO_LINKEDLIST_H
#define DEV_DYNAMO_LINKEDLIST_H

#include "../common/Node.h"
#include <stdexcept>
#include <string>

namespace DS
{
    template <typename T>

    class LinkedList
    {
    private:
        int length{};
        Node<T>* head;

    public:
        /* CONSTRUCTOR */
        LinkedList()
        {
            head = nullptr;
            length = 0;
        }

        /* PUBLIC FUNCTIONS */
        // This function returns a pointer to a node at a specified index in the Linked List
        Node<T>* get_node_at_index(int index)
        {
            // Check if the passed index is valid
            if (index < 0 || index >= length)
            {
                throw std::out_of_range("Index out of range");
            }

            // Cursor is a pointer to a node in the linked list
            // Once the node at the specified index is found, we can return the cursor pointer
            Node<T>* cursor = head;

            // Since we know how deep in the list we want to go, we can use a for loop to iterate
            for (int i = 0; i < index; i++)
            {
                cursor = cursor->get_next();
            }

            return cursor;
        }

        // Insert a node at a specified index
        void insert_node_at(T data, int index)
        {
            // Create the new node
            auto* newNode = new Node<T>(data);

            // If the user wishes to insert a node at the beginning we can change the head and return
            if (index == 0)
            {
                // Set the new node pointer to the previous head
                newNode->set_next(head);
                // Update the head
                head = newNode;
            }
            else
            {
                // Get a pointer to the node before the specified index
                Node<T>* cursor = get_node_at_index(index - 1);
                // Set the new node's pointer to the previous node's pointer
                newNode->set_next(cursor->get_next());
                // Update the previous node's pointer
                cursor->set_next(newNode);
            }

            // Update the linked list length
            length++;
        }

        // Prints the linked list
        void printList()
        {
            // Check if the list is not empty
            if (length == 0)
            {
                std::cout << "[]\n";
                return;
            }

            std::cout << "[";

            // Traverse the list
            for (int i = 0; i < length; i++)
            {
                if (i < length - 1)
                {
                    std::cout << get_node_at_index(i)->get_data() << ", ";
                }
                else
                {
                    std::cout << get_node_at_index(i)->get_data();
                }
            }

            std::cout << "]\n";
        }

        // Function to delete a node at a given position
        void deleteNode(int index)
        {
            // To delete the head of the list
            if (index == 0)
            {
                // Update the head to the new head
                head = head->get_next();
            }
            else
            {
                // Get a pointer to the node previous to the node we want to remove
                Node<T>* cursor = get_node_at_index(index - 1);
                // Get a pointer to the node we want to remove
                Node<T>* item_to_remove = cursor->get_next();
                // Update the previous node's pointer to the node the item we want to remove was pointing to
                cursor->set_next(item_to_remove->get_next());
                // Destroy the item we want to remove to prevent memory leaks
                free(item_to_remove);
            }

            // Update the linked list length
            length--;
        }

        // retrieve() returns the data stored within a node
        T retrieve(int index)
        {
            Node<T> * cursor = get_node_at_index(index);
            return cursor->get_data();
        }

        /* GETTERS */
        int len()
        {
            return length;
        }
    };
}


#endif //DEV_DYNAMO_LINKEDLIST_H
