#include <iostream>
#include "servers/DNS/DNS_Server.h"
#include "src/crypto/SHA256.h"

int main()
{
   /* servers::DNS_Server DNS(AF_INET, SOCK_STREAM, 0, 80, INADDR_ANY, 10, 5);
    DNS.launch();*/

    std::cout << crypto::sha256("Hello There");
}
