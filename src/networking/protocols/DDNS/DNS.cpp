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

    type = atoi(strtok(requested, "/"));
    std::cout << type << std::endl;
    source = strtok(nullptr, "/");
    std::cout << source << std::endl;
    body = strtok(nullptr, "/");
    std::cout << body << std::endl;
}

/* GETTERS */
int net::DNS::get_type() const {
    return type;
}

char *net::DNS::get_source() {
    return source;
}

char *net::DNS::get_body() {
    return body;
}

