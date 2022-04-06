//
// Created by Matos on 3/27/2022.
//

#ifndef DEV_DYNAMO_HTTP_H
#define DEV_DYNAMO_HTTP_H

#include <cstring>
#include <cstdlib>
#include <map>

namespace net {
    // Enum for HTTP methods
    enum HTTP_Methods
    {
        GET,
        HEAD,
        POST,
        PUT,
        PATCH,
        DELETE,
        CONNECT,
        OPTIONS,
        TRACE
    };

    class HTTP {
    private:
        /* MEMBER VARIABLES */
        std::map<char*, char*> request_line;
        std::map<char*, char*> header_fields;
        std::map<char*, char*> body;

        /* PRIVATE MEMBER FUNCTIONS */
        // These functions extract the constituent elements from a request string
        void extract_request_line_fields(char* input_request_line);
        void extract_header_fields(char* input_header_fields);
        void extract_body(char* input_body);

    public:
        /* CONSTRUCTOR */
        // The constructor for an HTTP request takes in a string containing the full request
        explicit HTTP(char* request_string);
    };
}


#endif //DEV_DYNAMO_HTTP_H
