//
// Created by joaoa on 21/04/2022.
//

#ifndef DEV_DYNAMO_SHA256_H
#define DEV_DYNAMO_SHA256_H

#include <string>
#include <sstream>
#include "openssl/sha.h"

namespace crypto
{
    std::string to_hex(unsigned char s);

    std::string sha256(const std::string& str);
}

#endif //DEV_DYNAMO_SHA256_H
