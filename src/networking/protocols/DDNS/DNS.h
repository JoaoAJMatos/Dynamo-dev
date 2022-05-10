//
// Created by joaoa on 05/04/2022.
//
// A DNS query in dynamo will have the following structure:
//
//      - Type (int)
//      - Source (char)
//      - Body (Char)
//
// The query type is indicated by an integer. Following are all the existing query types:
// [1] - SYNC-ME (Tells the server to send the node its known hosts list)
// [2] - SYNC (Sent from one name server to the others in order for them to sync their lists)
// [3] - IM-ALIVE (Sent by the node to the server to update the TTL on the server's connection list)
// [4] - REDIRECT (Sent by the server to a node in case the server's backlog is full. The body must contain the information needed to establish a connection with the other NS)
//
// Following are some examples of what a DNS query buffer should look like:
//
// ->    1/123e4567-e89b-12d3-a456-426614174000/""
// ->    2/(name server ID)/""
// ->    3/123e4567-e89b-12d3-a456-426614174000/""
// ->    4/(name server ID)/"192.168.0.1,7575" (Redirects a node to the name server at 192.168.0.1 port 7575)


#ifndef DEV_DYNAMO_DNS_H
#define DEV_DYNAMO_DNS_H

#include <map>
#include <cstring>
#include <cstdlib>
#include <sstream>
#include <string>

#define MAX_BUFFER_LENGTH 512

#define SYNC_ME 1
#define SYNC 2
#define IM_ALIVE 3
#define REDIRECT 4

namespace net
{
    class DNS
    {
    private:
        /* MEMBER VARIABLES */
        int type;
        std::string source_node;
        std::string body;

        std::string query_string;

    public:
        /* CONSTRUCTOR */
        // The constructor takes in a request string and parses it
        explicit DNS(char* request_string);
        DNS(int type, const std::string& source, const std::string& body);


        /* GETTERS */
        [[nodiscard]] int get_type() const;
        [[nodiscard]] std::string get_source() const;
        [[nodiscard]] std::string get_body() const;
        char* get_string();
    };
}


#endif //DEV_DYNAMO_DNS_H
