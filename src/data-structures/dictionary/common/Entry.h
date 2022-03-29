//
// Created by joaoa on 28/03/2022.
//
//
// The Entry class is the basis for a dictionary element. Every element has a key/value pair.
// The key and value types should be specified at compile time when instantiating the class.

#ifndef DEV_DYNAMO_ENTRY_H
#define DEV_DYNAMO_ENTRY_H

namespace Dict
{
    template <typename keyType, typename valueType>

    class Entry
    {
    private:
        /* MEMBER VARIABLES */
        keyType key;
        valueType value;

    public:
        /* CONSTRUCTOR */
        Entry(keyType key, valueType value)
        {
            this->key = key;
            this->value = value;
        }

        /* GETTERS */
        keyType getKey() const
        {
            return key;
        }

        valueType getValue() const
        {
            return value;
        }
    };
}

#endif //DEV_DYNAMO_ENTRY_H
