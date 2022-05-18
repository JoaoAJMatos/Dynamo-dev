//
// Created by Matos on 4/6/2022.
//

#ifndef DEV_DYNAMO_UUID_H
#define DEV_DYNAMO_UUID_H

#include <cstdio>
#include <cstdint>
#include <openssl/rand.h>

namespace Dynamo_UUID
{
    /**
     * @brief Generate a version 4 UUID according to RFC-4122
     * 
     * (The output buffer must be at least 38 bytes long)
     * 
     * @param output_buffer 
     * @return int 
     */
    
    int uuidv4(char* output_buffer);
}

#endif //DEV_DYNAMO_UUID_H
