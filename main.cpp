#include "src/data-structures/queue/Queue.h"
#include "src/data-structures/dictionary/Dictionary.h"
#include "src/data-structures/trees/default-comparisons/string-keys-compare.h"
#include <iostream>
#include <vector>

int int_compare(int entry_one, int entry_two)
{
    if (entry_two > entry_one)
    {
        return 1;
    }
    else if (entry_one > entry_two)
    {
        return -1;
    }
    else
    {
        return 0;
    }
}

int main() {
    //DS::Dictionary2<int, int> dict(int_compare);
    DS::Dictionary<int, int> dict;


    return 0;
}
