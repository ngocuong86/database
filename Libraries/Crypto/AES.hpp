//
// Created by tainguyen on 12/23/20.
//

#ifndef HCG1_AES_HPP
#define HCG1_AES_HPP

#include <openssl/evp.h>
#include <openssl/err.h>
#include "Libraries/Utils/Exception2.hpp>

namespace libX {
namespace crypto {
class AES {
public:
    static int Encrypt(unsigned char *input, int inputLength, unsigned char *key, unsigned char *iv, unsigned char *output);
    static int Decrypt(unsigned char *input, int inputLength, unsigned char *key, unsigned char *iv, unsigned char *output);

private:
    static void HandleErrors();
};

inline void AES::HandleErrors() {
    ThrowExceptionMessage(ErrorCode::ErrInternal, ERR_error_string(ERR_get_error(), nullptr));
}

inline int AES::Encrypt(
    unsigned char *input,
    int inputLength,
    unsigned char *key,
    unsigned char *iv,
    unsigned char *output
) {
    EVP_CIPHER_CTX *ctx;
    int length;
    int outputLength;
    if (!(ctx = EVP_CIPHER_CTX_new())) {
        HandleErrors();
    }

    if (EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, key, iv) != 1) {
        HandleErrors();
    }

    if (EVP_EncryptUpdate(ctx, output, &length, input, inputLength) != 1) {
        HandleErrors();
    } else {
        outputLength = length;
    }

    if (EVP_EncryptFinal_ex(ctx, output + length, &length)) {
        HandleErrors();
    } else {
        outputLength += length;
    }

    EVP_CIPHER_CTX_free(ctx);
    return outputLength;
}

inline int AES::Decrypt(
    unsigned char *input,
    int inputLength,
    unsigned char *key,
    unsigned char *iv,
    unsigned char *output
) {
    EVP_CIPHER_CTX *ctx;
    int length;
    int outputLength;
    if (!(ctx = EVP_CIPHER_CTX_new())) {
        HandleErrors();
    }

    if (EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, key, iv) != 1) {
        HandleErrors();
    }

    if (EVP_DecryptUpdate(ctx, output, &length, input, inputLength) != 1) {
        HandleErrors();
    } else {
        outputLength = length;
    }

    if (EVP_DecryptFinal_ex(ctx, output + length, &length) != 1) {
        HandleErrors();
    } else {
        outputLength += length;
    }

    EVP_CIPHER_CTX_free(ctx);
    return outputLength;
}
}
}

#endif //HCG1_AES_HPP
