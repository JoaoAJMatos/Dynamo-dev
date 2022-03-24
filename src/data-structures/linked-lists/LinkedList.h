//
// Created by joaoa on 24/03/2022.
//

#ifndef DEV_DYNAMO_LINKEDLIST_H
#define DEV_DYNAMO_LINKEDLIST_H

#include "Node.h"
#include <stdexcept>

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
            head = NULL;
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
                cursor = cursor->next;
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
                newNode->next = head;
                // Update the head
                head = newNode;
            }
            else
            {
                // Get a pointer to the node before the specified index
                Node<T>* cursor = get_node_at_index(index - 1);
                // Set the new node's pointer to the previous node's pointer
                newNode->next = cursor->next;
                // Update the previous node's pointer
                cursor->next = newNode;
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
                throw std::invalid_argument("Unable to print empty list");
            }

            // Traverse the list
            for (int i = 0; i < length; i++)
            {
                std::cout << get_node_at_index(i)->data << " ";
            }
        }

        // Function to delete a node at a given position
        void deleteNode(int index)
        {
            // To delete the head of the list
            if (index == 0)
            {
                // To prevent memory leaks, we save where the original head was
                Node<T>* item_to_remove;
                // Update the head to the new head
                head = head->next;
                // Finally, destroy the original head
                destroy(item_to_remove);
            }
            else
            {
                // Get a pointer to the node previous to the node we want to remove
                Node<T>* cursor = get_node_at_index(index - 1);
                // Get a pointer to the node we want to remove
                Node<T>* item_to_remove = cursor->next;
                // Update the previous node's pointer to the node the item we want to remove was pointing to
                cursor->next = item_to_remove->next;
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
            return cursor->data;
        }
    };
}


#endif //DEV_DYNAMO_LINKEDLIST_H
