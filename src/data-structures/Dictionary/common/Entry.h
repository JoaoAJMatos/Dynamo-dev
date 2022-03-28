//
// Created by joaoa on 28/03/2022.
//

#ifndef DEV_DYNAMO_ENTRY_H
#define DEV_DYNAMO_ENTRY_H

namespace Dict
{
    template <typename keyType, typename valueType>

    class Entry
    {
    private:
        keyType key;
        valueType value;

    public:
        Entry(keyType key, valueType value)
        {
            this->key = key;
            this->value = value;
        }
    };
}

#endif //DEV_DYNAMO_ENTRY_H
