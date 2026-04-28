#ifndef UI_H
#define UI_H

#include "password_manager.h"

void ui_init(void);
void ui_run(PasswordStore *store);
void ui_close(void);

#endif
