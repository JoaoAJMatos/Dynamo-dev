//
// Created by joaoa on 05/04/2022.
//

#include "DNS.h"

#include <iostream>

/* CONSTRUCTOR */
net::DNS::DNS(char* request_string)
{
    char requested[strlen(request_string)];
    strcpy(requested, request_string);

    int type = atoi(strtok(requested, "/"));
    char* body = strtok(nullptr, "/");


}

/* HELPER FUNCTION */
std::string toString(int i) {
    std::stringstream ss;
    ss << i;

    return ss.str();
}

net::DNS::DNS(int type, const char* body)
{
    //this->type = type;
    //this->body = body;

    //this->query_string = toString(type) + "/" +  body;
}


