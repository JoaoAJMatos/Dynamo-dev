#include "ECDSA.h"

/**
 * @brief Construct a new ECDSA::ECDSA object
 * 
 * Create a new key pair
 */
ECDSA::ECDSA()
{
    // Make the key pair
    ecc_make_key(publicKey, privateKey);

    // Set fromHex flag
    useKeysPointer = 0;

    // Convert uint8_t to hex and set the hex buffers
    toHex(privateKeyHex, sizeof(privateKeyHex), privateKey, sizeof(privateKey));
    toHex(publicKeyHex, sizeof(publicKeyHex), publicKey, sizeof(publicKey));

    /*uint8_t* hash;

    const char* str = "a665a45920422f9d417e4867efdc4fb8a04a1f3fff1fa07e998e86f7f7a27ae3";

    hash = hexToUint8(str);

    char buf[sizeof(privateKey)*2+1];
    char buf2[sizeof(publicKey)*2+1];

    toHex(buf, sizeof(buf), privateKey, sizeof(privateKey));
    toHex(buf2, sizeof(buf2), publicKey, sizeof(publicKey));

    std::cout << "Private key: " << buf << std::endl;
    std::cout << "Public key: " << buf2 << std::endl;

    uint8_t signature[ECC_BYTES*2];

    uint8_t* priva;
    priva = hexToUint8(buf);

    ecdsa_sign(priva, hash, signature);

    char buf3[sizeof(signature)*2+1];

    // Mess with the signature to test

    toHex(buf3, sizeof(buf3), signature, sizeof(signature));

    uint8_t* sig; 
    sig = hexToUint8(buf3);

    std::cout << "Signature: " << buf3 << std::endl;

    // Mess with the signature to test
    //sig[2] = 0; // This will output 0

    int res2 = ecdsa_verify(publicKey, hash, sig);

    std::cout << res2;
*/
    /*// Convert to hex and store the key pair
    toHex(publicKeyHex, sizeof(publicKeyHex), publicKey, sizeof(publicKey));
    toHex(privateKeyHex, sizeof(privateKeyHex), privateKey, sizeof(privateKey));*/  
}

/**
 * @brief Construct a new ECDSA::ECDSA object
 * 
 * @param publicKey 
 * @param privateKey 
 * 
 * @details Creates a new ECDSA object given a pre-defined key pair passed as a Hex string
 */
ECDSA::ECDSA(char* publicKey, char* privateKey)
{
    // Convert the char buffers into a uint8_t pointer and store the public/private keys
    p_privK = hexToUint8(privateKey);
    p_pubK = hexToUint8(publicKey);

    // Set fromHex flag
    useKeysPointer = 1;

    toHex(privateKeyHex, sizeof(privateKeyHex), p_privK, sizeof(p_privK));
    toHex(publicKeyHex, sizeof(publicKeyHex), p_pubK, sizeof(p_pubK));
}

/**
 * @brief Construct a new ECDSA::ECDSA object
 * 
 * @param publicKey 
 * @param privateKey 
 */
ECDSA::ECDSA(uint8_t* publicKey, uint8_t* privateKey)
{
    p_privK = privateKey;
    p_pubK = publicKey;

    useKeysPointer = 1;

    // Convert uint8_t to hex and set the hex buffers
    toHex(privateKeyHex, sizeof(privateKeyHex), privateKey, sizeof(privateKey));
    toHex(publicKeyHex, sizeof(publicKeyHex), publicKey, sizeof(publicKey));
}

/* PUBLIC FUNCTIONS */
int ECDSA::signHash(const char* hash)
{
    // Convert the hex string to a uint8_t pointer
    uint8_t* tmp;
    tmp = hexToUint8(hash);
    
    // If the flag is set to 1, use the uint8_t pointers. And use the uint8_t[ECC_BYTES] otherwise 
    if (useKeysPointer)
    {
        int res = ecdsa_sign(p_privK, tmp, this->signature);
        toHex(signatureHex, sizeof(signatureHex), this->signature, sizeof(this->signature));
        return res;
    } 

    int res = ecdsa_sign(this->privateKey, tmp, this->signature);
    toHex(signatureHex, sizeof(signatureHex), this->signature, sizeof(this->signature));
    return res;
}

/**
 * @brief 
 * 
 * @param pubK 
 * @param hash 
 * @param signature 
 * @return int 
 * 
 * @details Returns 1 if the public key matches the signature, 0 otherwise
 */
int ECDSA::verifySignature(const char* pubK, const char* hash, const char* signature)
{
    // Convert the incomming arguments to uint8_t pointer
    uint8_t* pk;
    pk = hexToUint8(pubK);

    uint8_t* tmpHash;
    tmpHash = hexToUint8(hash);

    uint8_t* sig;
    sig = hexToUint8(signature);

    // Verify and return
    return ecdsa_verify(pk, tmpHash, sig);
}

void ECDSA::showKeys()
{
    std::cout << "Public key: " << publicKeyHex << std::endl;
    std::cout << "Private key: " << privateKeyHex << std::endl;
}

void ECDSA::showSignature()
{
    std::cout << "Signature: " << signatureHex << std::endl;
}

/* HELPER FUNCTIONS */
int ECDSA::toHex(char* dest, size_t dest_len, const uint8_t* uint_buf, size_t uint_buf_len)
{
    // Check if the destination buffer is large enough
    if(dest_len < (uint_buf_len*2+1)) return 1;
    
    *dest = '\0'; // In case uint_buf_len == 0
    
    while(uint_buf_len--) 
    {
        // sprintf directly to where dest points
        sprintf(dest, "%02X", *uint_buf);
        dest += 2;
        ++uint_buf;
    }

    return 0;
}

uint8_t* ECDSA::hexToUint8(const char* buffer)
{
    if (buffer == nullptr) return nullptr;

    size_t slength = strlen(buffer);

    if ((slength % 2) != 0) return nullptr; // must be even

    size_t dlength = slength / 2;

    uint8_t* data = (uint8_t*)malloc(dlength);

    memset(data, 0, dlength);

    size_t index = 0;
    while (index < slength) 
    {
        char c = buffer[index];
        int value = 0;
        
        if (c >= '0' && c <= '9')
            value = (c - '0');
        else if (c >= 'A' && c <= 'F')
            value = (10 + (c - 'A'));
        else if (c >= 'a' && c <= 'f')
            value = (10 + (c - 'a'));
        else
            return nullptr;

        data[(index / 2)] += value << (((index + 1) % 2) * 4);

        index++;
    }   

    return data;
}