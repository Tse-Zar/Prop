// сделано Цезарем
// made by Tsezar


#include "../lib/crypto.h"
#include <string.h>

#define ROTR(x,n) (((x) >> (n)) | ((x) << (32 - (n))))

static const uint32_t k[64] = {
    0x428a2f98,0x71374491,0xb5c0fbcf,0xe9b5dba5,0x3956c25b,0x59f111f1,0x923f82a4,0xab1c5ed5,
    0xd807aa98,0x12835b01,0x243185be,0x550c7dc3,0x72be5d74,0x80deb1fe,0x9bdc06a7,0xc19bf174,
    0xe49b69c1,0xefbe4786,0x0fc19dc6,0x240ca1cc,0x2de92c6f,0x4a7484aa,0x5cb0a9dc,0x76f988da,
    0x983e5152,0xa831c66d,0xb00327c8,0xbf597fc7,0xc6e00bf3,0xd5a79147,0x06ca6351,0x14292967,
    0x27b70a85,0x2e1b2138,0x4d2c6dfc,0x53380d13,0x650a7354,0x766a0abb,0x81c2c92e,0x92722c85,
    0xa2bfe8a1,0xa81a664b,0xc24b8b70,0xc76c51a3,0xd192e819,0xd6990624,0xf40e3585,0x106aa070,
    0x19a4c116,0x1e376c08,0x2748774c,0x34b0bcb5,0x391c0cb3,0x4ed8aa4a,0x5b9cca4f,0x682e6ff3,
    0x748f82ee,0x78a5636f,0x84c87814,0x8cc70208,0x90befffa,0xa4506ceb,0xbef9a3f7,0xc67178f2
};

static uint32_t ch(uint32_t x, uint32_t y, uint32_t z)  { return (x & y) ^ (~x & z); }
static uint32_t maj(uint32_t x, uint32_t y, uint32_t z) { return (x & y) ^ (x & z) ^ (y & z); }
static uint32_t ep0(uint32_t x) { return ROTR(x,2) ^ ROTR(x,13) ^ ROTR(x,22); }
static uint32_t ep1(uint32_t x) { return ROTR(x,6) ^ ROTR(x,11) ^ ROTR(x,25); }
static uint32_t sig0(uint32_t x) { return ROTR(x,7) ^ ROTR(x,18) ^ (x >> 3); }
static uint32_t sig1(uint32_t x) { return ROTR(x,17) ^ ROTR(x,19) ^ (x >> 10); }

static void sha256_transform(uint32_t state[8], const uint8_t data[64]) {
    uint32_t a,b,c,d,e,f,g,h,i,j,t1,t2,m[64];
    for (i = 0, j = 0; i < 16; i++, j += 4)
        m[i] = ((uint32_t)data[j] << 24) | ((uint32_t)data[j+1] << 16) |
               ((uint32_t)data[j+2] << 8)  | ((uint32_t)data[j+3]);
    for (; i < 64; i++)
        m[i] = sig1(m[i-2]) + m[i-7] + sig0(m[i-15]) + m[i-16];

    a = state[0]; b = state[1]; c = state[2]; d = state[3];
    e = state[4]; f = state[5]; g = state[6]; h = state[7];

    for (i = 0; i < 64; i++) {
        t1 = h + ep1(e) + ch(e,f,g) + k[i] + m[i];
        t2 = ep0(a) + maj(a,b,c);
        h = g; g = f; f = e; e = d + t1; d = c; c = b; b = a; a = t1 + t2;
    }

    state[0] += a; state[1] += b; state[2] += c; state[3] += d;
    state[4] += e; state[5] += f; state[6] += g; state[7] += h;
}

void crypto_sha256(const uint8_t *data, size_t len, uint8_t hash[32]) {
    uint32_t state[8] = {
        0x6a09e667,0xbb67ae85,0x3c6ef372,0xa54ff53a,
        0x510e527f,0x9b05688c,0x1f83d9ab,0x5be0cd19
    };
    uint32_t datalen = 0;
    uint64_t bitlen = 0;
    uint8_t block[64];

    for (size_t i = 0; i < len; i++) {
        block[datalen++] = data[i];
        if (datalen == 64) {
            sha256_transform(state, block);
            bitlen += 512;
            datalen = 0;
        }
    }

    uint32_t i = datalen;
    block[i++] = 0x80;
    if (datalen < 56) {
        while (i < 56) block[i++] = 0x00;
    } else {
        while (i < 64) block[i++] = 0x00;
        sha256_transform(state, block);
        memset(block, 0, 56);
    }

    bitlen += datalen * 8;
    block[63] = (uint8_t)bitlen;
    block[62] = (uint8_t)(bitlen >> 8);
    block[61] = (uint8_t)(bitlen >> 16);
    block[60] = (uint8_t)(bitlen >> 24);
    block[59] = (uint8_t)(bitlen >> 32);
    block[58] = (uint8_t)(bitlen >> 40);
    block[57] = (uint8_t)(bitlen >> 48);
    block[56] = (uint8_t)(bitlen >> 56);
    sha256_transform(state, block);

    for (i = 0; i < 4; i++) {
        hash[i]      = (uint8_t)((state[0] >> (24 - i*8)) & 0xff);
        hash[i + 4]  = (uint8_t)((state[1] >> (24 - i*8)) & 0xff);
        hash[i + 8]  = (uint8_t)((state[2] >> (24 - i*8)) & 0xff);
        hash[i + 12] = (uint8_t)((state[3] >> (24 - i*8)) & 0xff);
        hash[i + 16] = (uint8_t)((state[4] >> (24 - i*8)) & 0xff);
        hash[i + 20] = (uint8_t)((state[5] >> (24 - i*8)) & 0xff);
        hash[i + 24] = (uint8_t)((state[6] >> (24 - i*8)) & 0xff);
        hash[i + 28] = (uint8_t)((state[7] >> (24 - i*8)) & 0xff);
    }
}

static void chacha_quarter_round(uint32_t *a, uint32_t *b, uint32_t *c, uint32_t *d) {
    *a += *b; *d ^= *a; *d = (*d << 16) | (*d >> 16);
    *c += *d; *b ^= *c; *b = (*b << 12) | (*b >> 20);
    *a += *b; *d ^= *a; *d = (*d << 8)  | (*d >> 24);
    *c += *d; *b ^= *c; *b = (*b << 7)  | (*b >> 25);
}

static void chacha_block(const uint8_t key[32], const uint8_t nonce[12],
                         uint32_t counter, uint8_t out[64]) {
    static const uint32_t constants[4] = {0x61707865, 0x3320646e, 0x79622d32, 0x6b206574};
    uint32_t state[16];
    uint32_t working[16];

    state[0] = constants[0];
    state[1] = constants[1];
    state[2] = constants[2];
    state[3] = constants[3];
    memcpy(&state[4], key, 32);
    state[12] = counter;
    memcpy(&state[13], nonce, 12);

    memcpy(working, state, sizeof(state));

    for (int i = 0; i < 10; i++) {
        chacha_quarter_round(&working[0], &working[4], &working[8],  &working[12]);
        chacha_quarter_round(&working[1], &working[5], &working[9],  &working[13]);
        chacha_quarter_round(&working[2], &working[6], &working[10], &working[14]);
        chacha_quarter_round(&working[3], &working[7], &working[11], &working[15]);
        chacha_quarter_round(&working[0], &working[5], &working[10], &working[15]);
        chacha_quarter_round(&working[1], &working[6], &working[11], &working[12]);
        chacha_quarter_round(&working[2], &working[7], &working[8],  &working[13]);
        chacha_quarter_round(&working[3], &working[4], &working[9],  &working[14]);
    }

    for (int i = 0; i < 16; i++) {
        working[i] += state[i];
        out[i*4+0] = (uint8_t)(working[i]        & 0xff);
        out[i*4+1] = (uint8_t)((working[i] >> 8)  & 0xff);
        out[i*4+2] = (uint8_t)((working[i] >> 16) & 0xff);
        out[i*4+3] = (uint8_t)((working[i] >> 24) & 0xff);
    }
}

void crypto_chacha20(const uint8_t key[32], const uint8_t nonce[12], uint32_t counter,
                     const uint8_t *input, uint8_t *output, size_t len) {
    uint8_t block[64];
    size_t i;
    while (len) {
        chacha_block(key, nonce, counter++, block);
        size_t chunk = len < 64 ? len : 64;
        for (i = 0; i < chunk; i++) {
            output[i] = input[i] ^ block[i];
        }
        input  += chunk;
        output += chunk;
        len    -= chunk;
    }
}

void crypto_derive_key(const char *password, uint8_t key[32]) {
    crypto_sha256((const uint8_t *)password, strlen(password), key);
}
