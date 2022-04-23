#include <iostream>
#include "servers/DNS/DNS_Server.h"
#include "src/networking/network/node/common/NodeClient.h"

int main()
{
    /*servers::DNS_Server DNS(AF_INET, SOCK_STREAM, 0, 80, INADDR_ANY, 10, 5);
    DNS.launch();*/

    NodeClient nc(AF_INET, SOCK_STREAM, 0, 80, inet_addr("192.168.1.109"));
    nc.request("127.0.0.1", "Hey");
}
