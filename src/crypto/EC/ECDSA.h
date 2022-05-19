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

#include "ecc.h"

class ECDSA
{
private:
    /* MEMBER VARIABLES */
    uint8_t privateKey[ECC_BYTES];
    uint8_t publicKey[ECC_BYTES+1];
    uint8_t signature[ECC_BYTES*2];

    // These pointers are used to store the private and public keys
    // in case the ECDSA object is constructed given two Hex arrays for the keys.
    // This has to do with the way the hex -> uint8_t conversion was implemented.
    // Prolly not the best idea, but it works for now.
    uint8_t* p_pubK;
    uint8_t* p_privK;

    char privateKeyHex[sizeof(privateKey)*2+1];
    char publicKeyHex[sizeof(publicKey)*2+1];
    char signatureHex[sizeof(signature)*2+1];

    // This flag indicates if the class was constructed from a hex string or not.
    // This is because the signHash() method has to know which key pairs to use (uint8_t[ECC_BYTES] or uint8_t*)
    // I know this is a mess I'm sorry
    int useKeysPointer;

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
     * @brief Signs a hash with the private key (SHA-2 family is recommended)
     * 
     * @param hash 
     * @return int
     * 
     * @details Returns 1 if the signature has been successful, 0 otherwise
     */
    int signHash(const char* hash);

    /**
     * @brief Verifies a digital signature given a public key and a hash
     * 
     * @param pubK 
     * @param hash 
     * @param signature 
     * @return int
     * 
     * @details Returns 1 if the public key matches the signature, 0 otherwise
     */
    int verifySignature(const char* pubK, const char* hash, const char* signature);

    /**
     * @brief Prints the keys to the standard out
     * 
     */
    void showKeys();

    /**
     * @brief Prints the signature to the standard out
     * 
     */
    void showSignature();
};

#endif