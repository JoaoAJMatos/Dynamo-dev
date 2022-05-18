#include <iostream>
#include <vector>
#include <unistd.h>

#include <algorithm>

#include "servers/DNS/DNS_Server.h"
#include "src/dynamo/network/node/Node.h"
#include "src/crypto/ECDSA.h"

int bind_param(std::string* str, const std::vector<std::string>& vector)
{
    int temp = -1; // This variable will hold the index of the placeholder occurences

    // Return if the amount of arguments is not the same as the amount of placeholders
    if (std::count(str->begin(), str->end(), '?') != vector.size()) return -1;

    // Loop through the parameters and replace the string
    for (auto& element : vector)
    {
        temp = str->find("?", temp + 1); // Find the index of the next placeholder
        str->replace(temp, element.length(), element); // Replace the string
    }

    return 0;
}

int main()
{
    ECDSA ec;

    /*servers::DNS_Server DNS(AF_INET, SOCK_STREAM, 6, 4542, INADDR_ANY, 100, 0);
    DNS.launch();*/

    /*Node n;
    n.start();*/
}
