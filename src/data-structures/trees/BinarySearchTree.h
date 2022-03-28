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

    class BinarySearchTree {
    private:
        /* MEMBER VARIABLES */
        Node<T>* head;

        /* MEMBER FUNCTIONS */
        Node<T>* iterate() {
            return nullptr;
        }

    public:
        BinarySearchTree();

        /* VIRTUAL FUNCTIONS */
        // The compare function should be implemented inside the child class
        // It must return either -1, 0, or 1.
        // Where -1 is going to represent a direction of -1 (left) and 1 is going to represent a direction of +1 (right)
        // The value 0 should be returned if both of the arguments are equal, informing the required node has been found.
        virtual int compare(void* data_one, void* data_two) = 0;

        /* PUBLIC FUNCTIONS */
        void insert(T data) {

        }

        void* search(T data) {
            return nullptr;
        }
    };
}


#endif //DEV_DYNAMO_BINARYSEARCHTREE_H
