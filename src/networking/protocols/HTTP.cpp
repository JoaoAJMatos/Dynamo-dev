//
// Created by Matos on 3/27/2022.
//

#include "HTTP.h"

/* CONSTRUCTOR */
// The constructor takes in a pointer to a string array and parses it to create an instance of
// HTTP request defined in HTTP.h.
//
// The HTTP request string looks something like this:
//
//      POST / HTTP/1.1
//      Host: 127.0.0.1
//      User-Agent: curl...
//      Accept-Encoding: gzip, deflate
//      Accept: */*
//      Connection: keep-alive
//      Content-Length: 16
//      Content-Type: application/x-www-form-urlencoded
//
//      one=2&three=four
//
// First, the constructor loops through the string array and checks for the body of the request.
// Once found, it separates the body from the headers with a '|' character.
// After that, it separates the request line, containing the Method and HTTP version;
// Separates the headers from the request string, and separates the body of the request as well.
// Finally, it fills in the members of the class, to create a new instance of an HTTP request.

/* HELPER FUNCTION */
// This function selects the right specified method given a method string
int what_method(char* method_string)
{
    // I wish I could use a fucking switch statement for this
    if (strcmp(method_string, "GET") == 0) return net::HTTP_Methods::GET;
    else if (strcmp(method_string, "HEAD") == 0) return  net::HTTP_Methods::HEAD;
    else if (strcmp(method_string, "POST") == 0) return  net::HTTP_Methods::POST;
    else if (strcmp(method_string, "PUT") == 0) return  net::HTTP_Methods::PUT;
    else if (strcmp(method_string, "PATCH") == 0) return  net::HTTP_Methods::PATCH;
    else if (strcmp(method_string, "DELETE") == 0) return  net::HTTP_Methods::DELETE;
    else if (strcmp(method_string, "CONNECT") == 0) return  net::HTTP_Methods::CONNECT;
    else if (strcmp(method_string, "OPTIONS") == 0) return  net::HTTP_Methods::OPTIONS;

    return net::HTTP_Methods::TRACE;
}

net::HTTP::HTTP(char *request_string)
{
    char requested[strlen(request_string)];
    strcpy(requested, request_string);

    // Loop through the string array and replace the new-line character
    // separating the request body by a different separator token
    for (int i = 0; i < strlen(requested) - 1; i++)
    {
        if (requested[i] == '\n' && requested[i + 1] == '\n')
        {
            requested[1 + 1] = '|';
        }
    }

    // Separate the request string into sections
    char* request_line = strtok(requested, "\n");
    char* header_fields = strtok(nullptr, "|");
    char* body = strtok(nullptr, "|");

    // Fill in the member variables
    // (sorry for the mess, there's prolly a more elegant way of doing this, but I need to take a shit, so...)
    char* _method = strtok(request_line, " ");
    method = what_method(_method);
    URI = strtok(nullptr, " ");
    char* HTTP_Version_temp = strtok(nullptr, " ");
    HTTP_Version_temp = strtok(HTTP_Version_temp, "/");
    HTTP_Version_temp = strtok(nullptr, "/");
    HTTP_Version = (float)atof(HTTP_Version_temp);
}
