//
// Created by Matos on 3/27/2022.
//

#ifndef DEV_DYNAMO_HTTP_H
#define DEV_DYNAMO_HTTP_H

#include <cstring>
#include <cstdlib>

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
        int method;
        char* URI;
        float HTTP_Version;

    public:
        /* CONSTRUCTOR */
        explicit HTTP(char* request_string);
    };

}


#endif //DEV_DYNAMO_HTTP_H
