#include <iostream>
#include "servers/DNS/DNS_Server.h"
#include "src/networking/network/node/common/NodeClient.h"
#include "src/networking/network/node/Node.h"
#include "src/networking/sockets/common/Socket.h"
#include <unistd.h>

int main()
{
    /*servers::DNS_Server DNS(AF_INET, SOCK_STREAM, 0, 80, INADDR_ANY, 10, 5);
    DNS.launch();*/

    /*WSADATA wsa;
    WORD wVersionRequired = MAKEWORD(2, 2);
    int iResult = WSAStartup(wVersionRequired, &wsa);

    int sockfd;
    struct sockaddr_in serv_addr{};

    char buffer[256];

    sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(80);


    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    int conn = connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    if (conn < 0) {
        printf("\nConnection Failed \n");
        printf("Hey");
        std::cout << WSAGetLastError();
        return -1;
    }

    send(sockfd, "hello", strlen("hello"), 0);
    send(sockfd, "hehe", strlen("hello"), 0);

    recv(sockfd, buffer, 256, 0);

    closesocket(sockfd);*/

    /*NodeClient nc(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    nc.request("192.168.1.109", 80, "1/123e4567-e89b-12d3-a456-426614174000/\"\"");
    nc.request("192.168.1.109", 80, "222222222");*/

    Node n;
}
