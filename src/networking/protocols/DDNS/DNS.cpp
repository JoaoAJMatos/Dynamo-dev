//
// Created by joaoa on 05/04/2022.
//

#include "DNS.h"

#include <iostream>

/* CONSTRUCTORS */
net::DNS::DNS(char* request_string)
{
    char requested[strlen(request_string)];
    strcpy(requested, request_string);

    type = atoi(strtok(requested, "/"));
    source_node = strtok(nullptr, "/");
    body = strtok(nullptr, "/");

    this->query_string = request_string;
}

net::DNS::DNS(int type, const std::string& source, const std::string& body)
{
    this->type = type;
    this->body = body;
    this->source_node = source;

    this->query_string = std::to_string(type) + "/" + source + "/" + body;
}

/* GETTERS */
int net::DNS::get_type() const
{
    return type;
}

std::string net::DNS::get_body() const
{
    return body;
}

char *net::DNS::get_string()
{
    return this->query_string.data();
}

std::string net::DNS::get_source() const {
    return this->source_node;
}


