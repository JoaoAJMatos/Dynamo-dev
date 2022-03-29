//
// Created by joaoa on 29/03/2022.
//

#ifndef DEV_DYNAMO_STRING_KEYS_COMPARE_H
#define DEV_DYNAMO_STRING_KEYS_COMPARE_H

#include "../../dictionary/common/Entry.h"
#include "cstring"

int compare_string_keys(Dict::Entry<char*, char*> entry_one, Dict::Entry<char*, char*> entry_two)
{
    if (strcmp(entry_one.getKey(), entry_two.getKey()) > 0)
    {
        return 1;
    }
    else if (strcmp((char *)(entry_one.getKey()), (char *)(entry_two.getKey())) < 0)
    {
        return -1;
    }
    return 0;
}

#endif //DEV_DYNAMO_STRING_KEYS_COMPARE_H
