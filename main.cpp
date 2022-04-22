#include <iostream>
#include "servers/DNS/DNS_Server.h"
#include "src/crypto/SHA256.h"
#include "src/crypto/secp256k1/secp256k1.h"
#include "src/networking/network/node/Node.h"

int main()
{
   /* servers::DNS_Server DNS(AF_INET, SOCK_STREAM, 0, 80, INADDR_ANY, 10, 5);
    DNS.launch();*/

    Node* n = new Node();
}
