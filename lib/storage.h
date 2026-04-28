#ifndef STORAGE_H
#define STORAGE_H

#include "password_manager.h"
#include <stdbool.h>

bool storage_save(const PasswordStore *store, const char *password, const char *filename);
bool storage_load(PasswordStore *store, const char *password, const char *filename);

#endif
