#ifndef PASSWORD_MANAGER_H
#define PASSWORD_MANAGER_H

#include <stdbool.h>

#define MAX_SAVED 64
#define PASS_MAX  128
#define SERV_MAX  64

typedef struct {
    char service[SERV_MAX];
    char password[PASS_MAX];
} Entry;

typedef struct {
    Entry entries[MAX_SAVED];
    int count;
} PasswordStore;

void pm_init(PasswordStore *store);
void pm_generate_password(char *out, int len, bool lower, bool upper, bool digits, bool symbols);
bool pm_add_entry(PasswordStore *store, const char *service, const char *password);
void pm_remove_entry(PasswordStore *store, int index);
const Entry* pm_get_entry(const PasswordStore *store, int index);
int pm_get_count(const PasswordStore *store);

#endif
