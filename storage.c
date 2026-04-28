// сделано Цезарем
// made by Tsezar

#include "lib/storage.h"
#include "lib/crypto.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STORAGE_FILE "vault.dat"

bool storage_save(const PasswordStore *store, const char *password, const char *filename) {
    if (!filename) filename = STORAGE_FILE;
    if (!password || !password[0]) return false;

    uint8_t key[32];
    crypto_derive_key(password, key);

    size_t entry_size = sizeof(Entry);
    size_t total_size = sizeof(int) + store->count * entry_size;
    uint8_t *plaintext = (uint8_t *)malloc(total_size);
    if (!plaintext) return false;

    memcpy(plaintext, &store->count, sizeof(int));
    memcpy(plaintext + sizeof(int), store->entries, store->count * entry_size);

    uint8_t nonce[12];
    for (int i = 0; i < 12; i++) nonce[i] = (uint8_t)(rand() & 0xFF);

    uint8_t *ciphertext = (uint8_t *)malloc(total_size);
    if (!ciphertext) { free(plaintext); return false; }

    crypto_chacha20(key, nonce, 1, plaintext, ciphertext, total_size);
    free(plaintext);

    FILE *f = fopen(filename, "wb");
    if (!f) { free(ciphertext); return false; }

    fwrite(nonce, 1, 12, f);
    fwrite(ciphertext, 1, total_size, f);
    fclose(f);
    free(ciphertext);
    return true;
}

bool storage_load(PasswordStore *store, const char *password, const char *filename) {
    if (!filename) filename = STORAGE_FILE;
    if (!password || !password[0]) return false;

    FILE *f = fopen(filename, "rb");
    if (!f) return false;

    fseek(f, 0, SEEK_END);
    long file_size = ftell(f);
    fseek(f, 0, SEEK_SET);

    if (file_size < 12) { fclose(f); return false; }

    uint8_t nonce[12];
    if (fread(nonce, 1, 12, f) != 12) { fclose(f); return false; }

    size_t cipher_size = (size_t)(file_size - 12);
    uint8_t *ciphertext = (uint8_t *)malloc(cipher_size);
    if (!ciphertext) { fclose(f); return false; }

    if (fread(ciphertext, 1, cipher_size, f) != cipher_size) {
        free(ciphertext); fclose(f); return false;
    }
    fclose(f);

    uint8_t key[32];
    crypto_derive_key(password, key);

    uint8_t *plaintext = (uint8_t *)malloc(cipher_size);
    if (!plaintext) { free(ciphertext); return false; }

    crypto_chacha20(key, nonce, 1, ciphertext, plaintext, cipher_size);
    free(ciphertext);

    int count;
    memcpy(&count, plaintext, sizeof(int));
    if (count < 0 || count > MAX_SAVED ||
        (sizeof(int) + (size_t)count * sizeof(Entry)) > cipher_size) {
        free(plaintext);
        return false;
    }

    store->count = count;
    memcpy(store->entries, plaintext + sizeof(int), count * sizeof(Entry));
    free(plaintext);
    return true;
}
