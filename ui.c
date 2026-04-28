// сделано Цезарем
// made by Tsezar

#include "lib/ui.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "lib/raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "lib/raygui.h"
#include "raygui-4.0/styles/dark/style_dark.h"
#include "lib/storage.h"

/* To embed a font directly into the binary:
   1. Place any .ttf file (e.g. JetBrainsMono-Regular.ttf) in the project root
   2. Rename it to font.ttf
   3. Run: python tools/embed_font.py
   4. Rebuild.  If font_embedded.h exists, it will be used automatically. */
#if defined(__has_include)
    #if __has_include("font_embedded.h")
        #include "font_embedded.h"
        #define USE_EMBEDDED_FONT
    #endif
#endif

#define SCREEN_W 800
#define SCREEN_H 600

static Font g_customFont = {0};

void ui_init(void) {
    InitWindow(SCREEN_W, SCREEN_H, "Password Manager");
    SetTargetFPS(60);
    GuiLoadStyleDark();

    bool fontLoaded = false;

#ifdef USE_EMBEDDED_FONT
    g_customFont = LoadFontFromMemory(".ttf", font_ttf_data, FONT_TTF_SIZE, 20, NULL, 0);
    if (g_customFont.texture.id != 0) fontLoaded = true;
#endif

    if (!fontLoaded && FileExists("font.ttf")) {
        g_customFont = LoadFontEx("font.ttf", 20, NULL, 0);
        if (g_customFont.texture.id != 0) fontLoaded = true;
    }

    if (fontLoaded) {
        GuiSetFont(g_customFont);
        GuiSetStyle(DEFAULT, TEXT_SIZE, 20);
    }
}

void ui_run(PasswordStore *store) {
    int  length = 16;
    bool useLower   = true;
    bool useUpper   = true;
    bool useDigits  = true;
    bool useSymbols = true;
    char password[PASS_MAX] = {0};
    char service[SERV_MAX] = {0};
    char masterPassword[64] = {0};

    int scrollIndex = 0;
    int activeList = -1;

    bool showCopied = false;
    double copiedTime = 0.0;

    bool showVaultStatus = false;
    double vaultStatusTime = 0.0;
    char vaultStatus[64] = {0};

    bool editService = false;
    bool editMaster  = false;

    static char listText[16384];

    Rectangle recService = {130, 50, 240, 25};
    Rectangle recMaster  = {150, 425, 220, 25};

    while (!WindowShouldClose()) {
        /* --- handle text-box focus --- */
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            Vector2 mouse = GetMousePosition();
            if (CheckCollisionPointRec(mouse, recService)) {
                editService = true;
                editMaster  = false;
            } else if (CheckCollisionPointRec(mouse, recMaster)) {
                editMaster  = true;
                editService = false;
            } else {
                editService = false;
                editMaster  = false;
            }
        }

        BeginDrawing();
        ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));

        /* --- Generator Panel --- */
        GuiPanel((Rectangle){ 10, 10, 380, 580 }, "Generator");

        GuiLabel((Rectangle){ 30, 50, 100, 25 }, "Service:");
        GuiTextBox(recService, service, SERV_MAX, editService);

        GuiSpinner((Rectangle){ 130, 90, 120, 25 }, "Length: ", &length, 4, 64, false);

        GuiCheckBox((Rectangle){ 30, 130, 20, 20 }, "Lowercase", &useLower);
        GuiCheckBox((Rectangle){ 30, 160, 20, 20 }, "Uppercase", &useUpper);
        GuiCheckBox((Rectangle){ 30, 190, 20, 20 }, "Digits",    &useDigits);
        GuiCheckBox((Rectangle){ 30, 220, 20, 20 }, "Symbols",   &useSymbols);

        if (GuiButton((Rectangle){ 30, 260, 120, 30 }, "Generate")) {
            pm_generate_password(password, length, useLower, useUpper, useDigits, useSymbols);
        }

        GuiLabel((Rectangle){ 30, 300, 100, 25 }, "Password:");
        GuiTextBox((Rectangle){ 30, 330, 340, 30 }, password, PASS_MAX, false);

        if (GuiButton((Rectangle){ 30, 380, 80, 30 }, "Copy")) {
            SetClipboardText(password);
            showCopied = true;
            copiedTime = GetTime();
        }
        if (showCopied) {
            GuiLabel((Rectangle){ 120, 385, 100, 25 }, "Copied!");
            if (GetTime() - copiedTime > 1.2) showCopied = false;
        }

        if (GuiButton((Rectangle){ 220, 380, 150, 30 }, "Save to Manager")) {
            if (pm_add_entry(store, service, password)) {
                activeList = -1;
            }
        }

        GuiLabel((Rectangle){ 30, 425, 120, 25 }, "Master Pass:");
        GuiTextBox(recMaster, masterPassword, 64, editMaster);

        if (GuiButton((Rectangle){ 30, 465, 160, 30 }, "Save Vault")) {
            if (storage_save(store, masterPassword, NULL)) {
                strncpy(vaultStatus, "Vault saved!", sizeof(vaultStatus));
            } else {
                strncpy(vaultStatus, "Save failed!", sizeof(vaultStatus));
            }
            vaultStatus[sizeof(vaultStatus)-1] = '\0';
            showVaultStatus = true;
            vaultStatusTime = GetTime();
        }

        if (GuiButton((Rectangle){ 200, 465, 160, 30 }, "Load Vault")) {
            if (storage_load(store, masterPassword, NULL)) {
                strncpy(vaultStatus, "Vault loaded!", sizeof(vaultStatus));
                activeList = -1;
            } else {
                strncpy(vaultStatus, "Load failed!", sizeof(vaultStatus));
            }
            vaultStatus[sizeof(vaultStatus)-1] = '\0';
            showVaultStatus = true;
            vaultStatusTime = GetTime();
        }

        if (showVaultStatus) {
            GuiLabel((Rectangle){ 30, 510, 300, 25 }, vaultStatus);
            if (GetTime() - vaultStatusTime > 2.0) showVaultStatus = false;
        }

        /* --- Manager Panel --- */
        GuiPanel((Rectangle){ 410, 10, 380, 580 }, "Saved Passwords");

        int count = pm_get_count(store);
        if (count > 0) {
            listText[0] = '\0';
            for (int i = 0; i < count; i++) {
                const Entry *e = pm_get_entry(store, i);
                char line[384];
                snprintf(line, sizeof(line), "%s | %s", e->service, e->password);
                if (i > 0) strcat(listText, ";");
                strcat(listText, line);
            }

            GuiListView((Rectangle){ 430, 50, 340, 480 }, listText, &scrollIndex, &activeList);

            if (activeList >= 0 && activeList < count) {
                if (GuiButton((Rectangle){ 430, 540, 120, 30 }, "Copy Sel")) {
                    SetClipboardText(pm_get_entry(store, activeList)->password);
                }
                if (GuiButton((Rectangle){ 560, 540, 100, 30 }, "Delete")) {
                    pm_remove_entry(store, activeList);
                    activeList = -1;
                }
            }
        } else {
            GuiLabel((Rectangle){ 430, 50, 300, 25 }, "No saved passwords yet.");
        }

        EndDrawing();
    }
}

void ui_close(void) {
    if (g_customFont.texture.id != 0) {
        UnloadFont(g_customFont);
    }
    CloseWindow();
}
