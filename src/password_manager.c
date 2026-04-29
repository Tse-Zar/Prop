// сделано Цезарем
// made by Tsezar

#include "../lib/password_manager.h"
#include <string.h>
#include <stdlib.h>
#include <time.h>

void pm_init(PasswordStore *store) {
    store->count = 0;
}

void pm_generate_password(char *out, int len, bool lower, bool upper, bool digits, bool symbols) {
    const char *low  = "abcdefghijklmnopqrstuvwxyz";
    const char *up   = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const char *dig  = "0123456789";
    const char *sym  = "!@#$^&*()_+=-";

    char pool[128] = {0};
    if (lower)  strcat(pool, low);
    if (upper)  strcat(pool, up);
    if (digits) strcat(pool, dig);
    if (symbols)strcat(pool, sym);

    int pool_len = (int)strlen(pool);
    if (pool_len == 0) {
        out[0] = '\0';
        return;
    }

    for (int i = 0; i < len; i++) {
        out[i] = pool[rand() % pool_len];
    }
    out[len] = '\0';
}

bool pm_add_entry(PasswordStore *store, const char *service, const char *password) {
    if (store->count >= MAX_SAVED || !password || password[0] == '\0')
        return false;

    Entry *e = &store->entries[store->count];
    strncpy(e->service, service, SERV_MAX - 1);
    e->service[SERV_MAX - 1] = '\0';
    strncpy(e->password, password, PASS_MAX - 1);
    e->password[PASS_MAX - 1] = '\0';
    store->count++;
    return true;
}

void pm_remove_entry(PasswordStore *store, int index) {
    if (index < 0 || index >= store->count)
        return;
    for (int i = index; i < store->count - 1; i++) {
        store->entries[i] = store->entries[i + 1];
    }
    store->count--;
}

const Entry* pm_get_entry(const PasswordStore *store, int index) {
    if (index < 0 || index >= store->count)
        return NULL;
    return &store->entries[index];
}

int pm_get_count(const PasswordStore *store) {
    return store->count;
}
