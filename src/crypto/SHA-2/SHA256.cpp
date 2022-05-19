//
// Created by joaoa on 21/04/2022.
//

#include "SHA256.h"

std::string crypto::to_hex(unsigned char s)
{
    std::stringstream ss;
    ss << std::hex << (int) s;
    return ss.str();
}

std::string crypto::sha256(const std::string& str)
{
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;

    SHA256_Init(&sha256);
    SHA256_Update(&sha256, str.c_str(), str.length());
    SHA256_Final(hash, &sha256);

    std::string output;

    for(unsigned char i : hash) {
        output += to_hex(i);
    }

    return output;
}