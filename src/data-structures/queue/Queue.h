//
// Created by joaoa on 24/03/2022.
//

#ifndef DEV_DYNAMO_QUEUE_H
#define DEV_DYNAMO_QUEUE_H

#include "../linked-lists/LinkedList.h"

namespace DS
{
    template <typename T>

    // Tentar criar uma instancia da classe para conseguir usar o typename T dentro da classe
    class Queue: private LinkedList<int>
    {
    public:
        T pop()
        {
            T data = retrieve();
        }

    };
}

#endif //DEV_DYNAMO_QUEUE_H
