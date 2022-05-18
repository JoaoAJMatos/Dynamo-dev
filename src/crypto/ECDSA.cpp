#include "ECDSA.h"

/**
 * @brief Construct a new ECDSA::ECDSA object
 * 
 * Create a new key pair
 */
ECDSA::ECDSA()
{
    //publicKey = nullptr;
    //privateKey = nullptr;

    int res = ecc_make_key(publicKey, privateKey);

    uint8_t* hash;

    const char* str = "a665a45920422f9d417e4867efdc4fb8a04a1f3fff1fa07e998e86f7f7a27ae3";

    hash = hexToUint8(str);

    char buf[sizeof(privateKey)*2+1];
    char buf2[sizeof(publicKey)*2+1];

    toHex(buf, sizeof(buf), privateKey, sizeof(privateKey));
    toHex(buf2, sizeof(buf2), publicKey, sizeof(publicKey));

    std::cout << "Private key: " << buf << std::endl;
    std::cout << "Public key: " << buf2 << std::endl;

    uint8_t signature[ECC_BYTES*2];

    ecdsa_sign(privateKey, hash, signature);

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