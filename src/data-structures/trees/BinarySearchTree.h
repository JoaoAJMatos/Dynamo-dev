//
// Created by Matos on 3/27/2022.
//
//
// A binary tree refers to a tree with a single parent node, called a head,
// where each node will have no more than two children. Meaning you could have
// nodes with no children, nodes with one child, or nodes with two children.
//
// A binary search tree is one on which the node organization follows a specific
// pattern.
//
// Given the following tree:
//
//           8
//          / \
//         3   10
//        / \    \
//       1   6    \
//          / \   14
//         4   7  /
//              13
//
// - Starting from our head node, anything to the left of any given node is
//   going to have a value less than its parent, and anything to the right
//   is going to have a value greater than its parent.
//
// Let's say we want to find the number 7:
// - Starting from the head, which is 8, we compare the two values.
// - 7 < 8, so we move to the left. We arrive at node 3.
// - 7 > 3, so we move to the right. Arriving at node 6.
// - 7 > 6, again, we move to the right. Reaching the node 7, and finding what we were looking for
//
// This is an efficient way of storing items in terms of searchability.
// If I want to find a particular number, let's say 13. I can start with my head
// node and evaluate the comparison. 13 > 8. With that one comparison alone,
// we can eliminate half of tree. Assuming every node has 2 children, every
// comparison you do will reduce the number of remaining children in half.
// (In big O notation this is a log(n) function)
// For example, if there are a billion nodes in a tree, it will take ruffly,
// 30 iterations to find the value we are looking for (in the worst case scenario).


#ifndef DEV_DYNAMO_BINARYSEARCHTREE_H
#define DEV_DYNAMO_BINARYSEARCHTREE_H

#include "../common/Node.h"

namespace DS
{
    template <typename T>

    class BinarySearchTree
    {
    private:
        /* MEMBER VARIABLES */
        Node<T>* head;

        /* FUNCTION POINTERS */
        int (*compare)(T data_one, T data_two);

        /* MEMBER FUNCTIONS */
        // The iterate function is a recursive algorithm that traverses the branches of a tree.
        // It utilizes the compare function to determine if it should move left or right, and returns the cursor once there is nowhere left for the iterator to move.
        // The user must take care to ensure this function returns the node they are actually looking for.
        // The function takes a reference to the current position, desired data, and an int pointer as arguments.
        Node<T>* iterate(Node<T>* cursor, T data, int* direction)
        {
            std::cout << "Here";

            // Compare the cursor's data to the desired data
            if(compare(cursor->get_data(), data) == 1)
            {
                // Check if there is another node in the chain to be tested
                if (cursor->get_next())
                {
                    // Recursively test the next node (right)
                    return iterate(cursor->get_next(), data, direction);
                }
                else
                {
                    // Set the direction pointer and return the cursor
                    *direction = 1;
                    return cursor;
                }
            }
            // Alternative outcome of the compare function
            else if(compare(cursor->get_data(), data) == -1)
            {
                // Check if there is another node in the chain to be tested
                if (cursor->get_previous())
                {
                    // Recursively test the previous node (left)
                    return iterate(cursor->get_next(), data, direction);
                }
                else
                {
                    // Set the direction pointer and return the cursor
                    *direction = -1;
                    return cursor;
                }
            }

            // If the two values are equal
            *direction = 0;
            return cursor;
        }

    public:
        /* CONSTRUCTOR */
        explicit BinarySearchTree()
        {
            head = nullptr;
        }

        void set_compare(int (*incoming_compare)(T data_one, T data_two))
        {
            compare = incoming_compare;
        }

        /* PUBLIC FUNCTIONS */
        void insert(T data)
        {
            // Check if this is the first node in the tree
            if(!head)
            {
                head = new Node<T>(data);
            }
            else
            {
                // Set the direction pointer
                int direction = 0;
                // Find the desired position
                Node<T>* cursor = iterate(head, data, &direction);

                // Check if the new node should be inserted to the left or right
                if (direction == 1)
                {
                    cursor->set_next(new Node<T>(data));
                }
                else if (direction == -1)
                {
                    cursor->set_previous(new Node<T>(data));
                }
                // If the node is duplicate, we skip
            }
        }


        // SEARCH, searches for a node given the data and a compare function. Returns a pointer to a Node.
        // The compare function should be passed as a pointer to a function.
        // It must return either -1, 0, or 1.
        // Where -1 is going to represent a direction of -1 (left) and 1 is going to represent a direction of +1 (right)
        // The value 0 should be returned if both of the arguments are equal, informing the required node has been found.
        T search(T data)
        {
            // Set the direction pointer
            int direction = 0;
            // Use iterate to find de desired position

            Node<T>* cursor = iterate(head, data, &direction);

            std::cout << "Here";

            // Check if the node that was found by iterate is the desired one, or an adjacent one
            if (direction == 0)
            {
                return cursor->get_data();
            }
        }
    };
}


#endif //DEV_DYNAMO_BINARYSEARCHTREE_H
