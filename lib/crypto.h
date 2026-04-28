#ifndef CRYPTO_H
#define CRYPTO_H

#include <stdint.h>
#include <stddef.h>

void crypto_sha256(const uint8_t *data, size_t len, uint8_t hash[32]);
void crypto_chacha20(const uint8_t key[32], const uint8_t nonce[12], uint32_t counter,
                     const uint8_t *input, uint8_t *output, size_t len);
void crypto_derive_key(const char *password, uint8_t key[32]);

#endif
