//
// Created by joaoa on 05/04/2022.
//

#include "DNS.h"

/* CONSTRUCTOR */
net::DNS::DNS(char* request_string)
{
    char requested[strlen(request_string)];
    strcpy(requested, request_string);

    type = (int)* strtok(requested, "/");
    source = strtok(nullptr, "/");
    body = strtok(nullptr, "/");
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

