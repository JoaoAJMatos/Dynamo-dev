//
// Created by Matos on 3/28/2022.
//

#ifndef DEV_DYNAMO_STRING_COMPARE_H
#define DEV_DYNAMO_STRING_COMPARE_H

#include <cstring>

int string_compare(char* data_one, char* data_two)
{
    int comparison = strcmp(data_one, data_two);

    if (comparison > 0)
    {
        return 1;
    }
    else if (comparison < 0)
    {
        return -1;
    }
    else
    {
        return 0;
    }
}

#endif //DEV_DYNAMO_STRING_COMPARE_H