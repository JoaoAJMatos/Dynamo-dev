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
        void insertInDict(keyType key, valueType value)
        {
            auto* objectToInsert = new Dict::Entry<keyType, valueType>(key, value);

            bst.insert(objectToInsert);
        }

        valueType searchInDict(keyType key)
        {
            auto* result = bst.search(key);

            if (result)
            {
                return result->getValue();
            }

            return NULL;
        }

        int compare(Dict::Entry<keyType, valueType> entry_one, Dict::Entry<keyType, valueType> entry_two) override
        {
            if (entry_two.getKey() > entry_one.getKey())
            {
                return 1;
            }
            else if (entry_one.getKey() > entry_two.getKey())
            {
                return -1;
            }
            else
            {
                return 0;
            }
        }
    };
}

#endif //DEV_DYNAMO_DICTIONARY_H
