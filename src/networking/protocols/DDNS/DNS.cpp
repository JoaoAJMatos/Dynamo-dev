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

/* HELPER FUNCTION */
std::string toString(int i) {
    std::stringstream ss;
    ss << i;

    return ss.str();
}

net::DNS::DNS(int type, char* body)
{
    this->type = type;
    this->body = body;

    this->query_string = toString(type) + "/" +  body;
}

/* GETTERS */
int net::DNS::get_type() const {
    return type;
}

const char *net::DNS::get_source() {
    return source;
}

char *net::DNS::get_body() {
    return body;
}

std::string net::DNS::get_query_string()
{
    return query_string;
}

