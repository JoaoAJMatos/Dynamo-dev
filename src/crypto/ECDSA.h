/**
 * @file ECDSA.h
 * @author Joao Matos (github.com/JoaoAJMatos)
 * @brief 
 * @version 0.1
 * @date 2022-05-18
 * 
 * @copyright Copyright (c) 2022
 * 
 * The following code aims to wrap the ECC library into an easy-to-use API which can serve 
 * as the basis for creating Dynamo Wallets.
 */

#ifndef DEV_DYNAMO_ECDSA_H
#define DEV_DYNAMO_ECDSA_H

#include <iostream>
#include <cstring>

#include "EC/ecc.h"

class ECDSA
{
private:
    /* MEMBER VARIABLES */
    uint8_t privateKey[ECC_BYTES];
    uint8_t publicKey[ECC_BYTES+1];
    uint8_t signature[ECC_BYTES*2];

    char privateKeyHex[sizeof(privateKey)*2+1];
    char publicKeyHex[sizeof(publicKey)*2+1];
    char signatureHex[sizeof(signature)*2+1];

    /* HELPER FUNCTIONS */
    /**
     * @brief Converts a uint8_t buffer into a hexadecimal buffer
     * 
     * Output:
     *  @param dest 
     * 
     * Input:
     *  @param des_len 
     *  @param uint_buf 
     *  @param uint_buf_len 
     * 
     * @return int (0 if success, 1 otherwise) 
     */
    int toHex(char* dest, size_t dest_len, const uint8_t* uint_buf, size_t uint_buf_len);

    /**
     * @brief Converts a hex buffer into a uint8_t buffer
     * 
     * Output:
     *  @param dest   - Pointer to the destination buffer
     * 
     * Input:
     *  @param buffer - Hex buffer to convert
     * 
     * @return int (0 if success, 1 otherwise)
     */
    uint8_t* hexToUint8(const char* buffer);

public:
    /* CONSTRUCTORS */
    /**
     * @brief Construct a new ECDSA object
     *
     * @details Generates a new keypair from scratch
     */
    ECDSA();

    /**
     * @brief Construct a new ECDSA object
     * 
     * @param publicKey  (hex string)
     * @param privateKey (hex string)
     */
    ECDSA(char* publicKey, char* privateKey);

    /**
     * @brief Construct a new ECDSA object
     * 
     * @param publicKey  (uint8 buffer)
     * @param privateKey (uint8 buffer)
     */
    ECDSA(uint8_t* publicKey, uint8_t* privateKey);

    /* PUBLIC FUNCTIONS */
    /**
     * @brief Creates a new key pair
     * 
     * @return int 
     */
    int genKeyPair();
    
    int sign();

    int verifySignature();
};

#endif