//
// Created by joaoa on 21/04/2022.
//

#include "Block.h"

Block::Block(std::time_t timestamp, std::string hash, std::string prev_hash, size_t height, size_t nonce, int difficulty)
{
    this->timestamp = timestamp;
    this->hash = hash;
    this->prev_hash = prev_hash;
    this->height = height;
    this->nonce = nonce;
    this->difficulty = difficulty;
}

