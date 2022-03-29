#include "src/data-structures/queue/Queue.h"
#include "src/data-structures/dictionary/Dictionary.h"
#include <iostream>
#include <vector>


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

int main() {
    DS::Dictionary<char*, char*> dict(&compare_string_keys);

    char* key = "Hello";
    char* val = "Bye";

    char* got = dict.searchInDict("Hello");

    std::string printable(dict.searchInDict("Hello"));

    std::cout << printable;

    //dict.insertInDict(key, val);

    return 0;
}
