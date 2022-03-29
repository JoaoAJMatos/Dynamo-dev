//
// Created by joaoa on 29/03/2022.
//
// The dictionary will be a binary search tree containing

#ifndef DEV_DYNAMO_DICTIONARY_H
#define DEV_DYNAMO_DICTIONARY_H

#include "../trees/BinarySearchTree.h"
#include "./common/Entry.h"
#include "cstring"

namespace DS
{
    template<typename keyType, typename valueType>

    class Dictionary: public BinarySearchTree<Dict::Entry<keyType, valueType>>
    {
    private:
        BinarySearchTree<Dict::Entry<keyType, valueType>> bst;

    public:
        explicit Dictionary(int (*compare)(Dict::Entry<keyType, valueType>, Dict::Entry<keyType, valueType>))
        {
            bst.set_compare(compare);
        }

        void insertInDict(keyType key, valueType value)
        {
            Dict::Entry<keyType, valueType> objectToInsert = Dict::Entry<keyType, valueType>(key, value);

            bst.insert(objectToInsert);
        }

        valueType searchInDict(keyType key)
        {
            valueType dummy_value;
            Dict::Entry<keyType, valueType> searchable = Dict::Entry<keyType, valueType>(key, dummy_value);

            // TODO: Fix error happening here
            Dict::Entry<keyType, valueType> result = bst.search(searchable);

            return result.getValue();
        }
    };
}

#endif //DEV_DYNAMO_DICTIONARY_H
