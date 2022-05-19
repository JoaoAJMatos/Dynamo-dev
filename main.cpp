#include <iostream>
#include <vector>
#include <unistd.h>

#include <algorithm>

#include "servers/DNS/DNS_Server.h"
#include "src/dynamo/network/node/Node.h"
#include "src/crypto/EC/ECDSA.h"

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

    ec.showKeys();

    //ec.signHash("a665a45920422f9d417e4867efdc4fb8a04a1f3fff1fa07e998e86f7f7a27ae3");
    //ec.showSignature();

    int result;
    result = ec.verifySignature("02A8EA59DD7D1E96FDFC8B836420E674CBEFC3107EE9AE74D9ACC6BBE5898F4675", "a665a45920422f9d417e4867efdc4fb8a04a1f3fff1fa07e998e86f7f7a27ae3", "0D1D6BEF27094791CE39B10EAA8110405F0BC91CF2A4626D91089468F686D8E7ED736706C18EE9E164ED76795E2B91BA18E4A948AD71CF23C3BE311FC2F74865");

    std::cout << result;

    /*servers::DNS_Server DNS(AF_INET, SOCK_STREAM, 6, 4542, INADDR_ANY, 100, 0);
    DNS.launch();*/

    /*Node n;
    n.start();*/
}
