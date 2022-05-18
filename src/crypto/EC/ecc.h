/**
 * @file ecc.h
 * @author Joao Matos (github/JoaoAJMatos.com)
 * @brief 
 * @version 0.1
 * @date 2022-05-18
 * 
 * @copyright Copyright (c) 2022
 * 
 * @details This library implements ECDH and ECDSA. It is twice as fast as OpenSSL at verifying signatures.
 */

#ifndef _EASY_ECC_H_
#define _EASY_ECC_H_

#include <stdint.h>

/* Curve selection options. */
#define secp128r1 16
#define secp192r1 24
#define secp256r1 32
#define secp384r1 48

#ifndef ECC_CURVE
    #define ECC_CURVE secp256r1
#endif

#if (ECC_CURVE != secp128r1 && ECC_CURVE != secp192r1 && ECC_CURVE != secp256r1 && ECC_CURVE != secp384r1)
    #error "Must define ECC_CURVE to one of the available curves"
#endif

#define ECC_BYTES ECC_CURVE

    /**
     * @brief Create a public/private key pair
     * 
     * @param p_publicKey  - Will be filled in with the public key
     * @param p_privateKey - Will be filled in with the private key
     * @return int 
     * 
     * Returns 1 if the key pair was generated successfully, 0 otherwise
     */
    int ecc_make_key(uint8_t p_publicKey[ECC_BYTES+1], uint8_t p_privateKey[ECC_BYTES]);

    /**
     * @brief Compute a shared secret given your secret key and someone else's public key.
     * 
     * @note It is recommended that you hash the result of ecdh_shared_secret(3) before using it for symetric encryption or HMAC.
     * 
     * Inputs:
     *  @param p_publicKey  - The public key of the remote party
     *  @param p_privateKey - Your private key.
     * 
     * Outputs:
     *  @param p_secret     - will be filled in with the shared secret value. 
     * 
     * @return int 
     * 
     * Returns 1 if the secret was successfully created, 0 otherwise.
     */
    int ecdh_shared_secret(const uint8_t p_publicKey[ECC_BYTES+1], const uint8_t p_privateKey[ECC_BYTES], uint8_t p_secret[ECC_BYTES]);

    /**
     * @brief Generate an ECDSA signature for a given hash (SHA-2 is recommended)
     * 
     * Inputs:
     *  @param p_privateKey - Your private key
     *  @param p_hash       - The message hash to sign
     * 
     * Outputs:
     *  @param p_signature  - Will be filled in with the signature value.
     * 
     * @return int 
     * 
     * Returns 1 if the signature was generated successfully, 0 otherwise.
     */
    int ecdsa_sign(const uint8_t p_privateKey[ECC_BYTES], const uint8_t p_hash[ECC_BYTES], uint8_t p_signature[ECC_BYTES*2]);

    /**
     * @brief Verify an ECDSA signature.
     * 
     * @param p_publicKey  - The signer's public key
     * @param p_hash       - The hash of the signed data
     * @param p_signature  - The signature value
     * 
     * @return int
     * 
     * Returns 1 if the signature is valid, 0 therwise. 
     */
    int ecdsa_verify(const uint8_t p_publicKey[ECC_BYTES+1], const uint8_t p_hash[ECC_BYTES], const uint8_t p_signature[ECC_BYTES*2]);

#endif /* _EASY_ECC_H_ */