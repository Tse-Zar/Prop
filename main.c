// сделано Цезарем
// made by Tsezar

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "lib/password_manager.h"
#include "lib/ui.h"

int main(void) {
    srand((unsigned)time(NULL));

    PasswordStore store;
    pm_init(&store);

    ui_init();
    ui_run(&store);
    ui_close();

    return 0;
}