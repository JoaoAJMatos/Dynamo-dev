//
// Created by Matos on 3/28/2022.
//

#ifndef DEV_DYNAMO_INT_COMPARE_H
#define DEV_DYNAMO_INT_COMPARE_H

int int_compare(int data_one, int data_two)
{
    if (data_two > data_one)
    {
        return 1;
    }
    else if (data_one > data_two)
    {
        return -1;
    }
    else
    {
        return 0;
    }
}

#endif //DEV_DYNAMO_INT_COMPARE_H
