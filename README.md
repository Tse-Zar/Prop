# Password Manager

Cross-platform password generator and manager with graphical UI built in C using **raylib** + **raygui**.

---

## Features

- **Password Generator** — customizable length and character sets (lowercase, uppercase, digits, symbols)
- **Password Manager** — save generated passwords with service names, copy to clipboard, delete entries
- **Encrypted Vault** — save/load all passwords to a file encrypted with **ChaCha20** + **SHA-256** key derivation
- **Cross-platform** — works on Windows, Linux, macOS
- **Styled UI** — built-in dark theme with custom pixel font

---

## Project Structure

```
.
├── src/
│   ├── main.c                 # Entry point
│   ├── ui.c                   # raygui-based GUI implementation
│   ├── password_manager.c     # Generator and in-memory storage logic
│   ├── crypto.c               # Encryption primitives (pure C, no deps)
│   └── storage.c              # Vault file I/O with ChaCha20 encryption
├── lib/
│   ├── ui.h                   # UI module interface
│   ├── password_manager.h     # Data structures and API
│   ├── crypto.h               # SHA-256 + ChaCha20 declarations
│   ├── storage.h              # Encrypted vault save/load declarations
│   ├── raylib.h               # raylib header
│   └── raygui.h               # raygui header
├── font_embedded.h            # Generated embedded font (optional)
├── styles/                    # raygui themes (dark, terminal, cyber, etc.)
└── libraylib.a / raylib.lib   # Compiled raylib library
```

---

## Build

### Windows (MinGW / MSYS2 UCRT64)

```bash
gcc src/main.c src/password_manager.c src/ui.c src/crypto.c src/storage.c -Ilib -o passmgr.exe -mwindows -L. -lraylib -lopengl32 -lgdi32 -lwinmm
```

`-mwindows` hides the console window.

### Linux

```bash
gcc src/main.c src/password_manager.c src/ui.c src/crypto.c src/storage.c -Ilib -o passmgr -lraylib -lm -lpthread -ldl -lrt -lX11
```

### macOS

```bash
gcc src/main.c src/password_manager.c src/ui.c src/crypto.c src/storage.c -Ilib -o passmgr -lraylib -framework OpenGL -framework Cocoa -framework IOKit
```

---

## Dependencies

- [raylib](https://github.com/raysan5/raylib) — graphics/window/input library
- [raygui](https://github.com/raysan5/raygui) — immediate-mode GUI for raylib

Place `raylib.h`, `raygui.h` and the compiled library (`libraylib.a` / `raylib.lib`) in the `lib/` folder.

---

## How to Use

1. Enter a **service name** (e.g., `github`, `email`)
2. Set **password length** (4–64)
3. Check desired **character types**
4. Click **Generate**
5. Click **Copy** to copy to clipboard, or **Save to Manager** to store it in memory
6. In the **Saved Passwords** panel: select an entry, then **Copy Sel** or **Delete**
7. Enter a **Master Password** and click **Save Vault** to write an encrypted `vault.dat` file
8. Click **Load Vault** to restore saved passwords from the encrypted file

> **Note:** The master password is hashed with SHA-256 to produce a 256-bit key for ChaCha20 stream encryption. The vault file format is: `nonce (12 bytes) || ciphertext`.

---

## Changing Theme

Edit `src/ui.c`:

```c
#include "styles/dark/style_dark.h"
// ...
GuiLoadStyleDark();
```

Available themes in `styles/`:
- `dark` — dark modern
- `terminal` — green hacker-style
- `cyber` — cyberpunk
- `cherry` — red accent
- `jungle` — green accent
- `lavanda` — purple accent

---

---

---

# Менеджер паролей

Кроссплатформенный генератор и менеджер паролей с графическим интерфейсом на C с использованием **raylib** + **raygui**.

---

## Возможности

- **Генератор паролей** — настраиваемая длина и наборы символов (строчные, заглавные, цифры, спецсимволы)
- **Менеджер паролей** — сохранение паролей с названием сервиса, копирование в буфер обмена, удаление записей
- **Зашифрованное хранилище** — сохранение и загрузка всех паролей в файл, зашифрованный алгоритмом **ChaCha20** с ключом, полученным через **SHA-256**
- **Кроссплатформенность** — работает на Windows, Linux, macOS
- **Стилизованный интерфейс** — встроенная тёмная тема с пиксельным шрифтом

---

## Структура проекта

```
.
├── src/
│   ├── main.c                 # Точка входа
│   ├── ui.c                   # GUI на raygui
│   ├── password_manager.c     # Логика генерации и хранения в памяти
│   ├── crypto.c               # Криптографические примитивы (чистый C, без зависимостей)
│   └── storage.c              # Файловый ввод-вывод с шифрованием ChaCha20
├── lib/
│   ├── ui.h                   # Интерфейс модуля UI
│   ├── password_manager.h     # Структуры данных и API
│   ├── crypto.h               # Объявления SHA-256 + ChaCha20
│   ├── storage.h              # Объявления сохранения/загрузки хранилища
│   ├── raylib.h               # Заголовок raylib
│   └── raygui.h               # Заголовок raygui
├── font_embedded.h            # Сгенерированный встроенный шрифт (опционально)
├── styles/                    # Темы оформления raygui (dark, terminal, cyber и др.)
└── libraylib.a / raylib.lib   # Скомпилированная библиотека raylib
```

---

## Сборка

### Windows (MinGW / MSYS2 UCRT64)

```bash
gcc src/main.c src/password_manager.c src/ui.c src/crypto.c src/storage.c -Ilib -o passmgr.exe -mwindows -L. -lraylib -lopengl32 -lgdi32 -lwinmm
```

`-mwindows` скрывает консольное окно.

### Linux

```bash
gcc src/main.c src/password_manager.c src/ui.c src/crypto.c src/storage.c -Ilib -o passmgr -lraylib -lm -lpthread -ldl -lrt -lX11
```

### macOS

```bash
gcc src/main.c src/password_manager.c src/ui.c src/crypto.c src/storage.c -Ilib -o passmgr -lraylib -framework OpenGL -framework Cocoa -framework IOKit
```

---

## Зависимости

- [raylib](https://github.com/raysan5/raylib) — библиотека графики, окон и ввода
- [raygui](https://github.com/raysan5/raygui) — GUI для raylib

Положите `raylib.h`, `raygui.h` и скомпилированную библиотеку (`libraylib.a` / `raylib.lib`) в папку `lib/`.

---

## Использование

1. Введите **название сервиса** (например, `github`, `email`)
2. Задайте **длину пароля** (4–64)
3. Отметьте нужные **типы символов**
4. Нажмите **Generate**
5. Нажмите **Copy**, чтобы скопировать в буфер, или **Save to Manager**, чтобы сохранить в памяти
6. В панели **Saved Passwords** выберите запись, затем **Copy Sel** или **Delete**
7. Введите **мастер-пароль** и нажмите **Save Vault**, чтобы записать зашифрованный файл `vault.dat`
8. Нажмите **Load Vault**, чтобы восстановить сохранённые пароли из зашифрованного файла

> **Примечание:** мастер-пароль хешируется алгоритмом SHA-256 для получения 256-битного ключа шифрования ChaCha20. Формат файла хранилища: `nonce (12 байт) || шифротекст`.

---

## Смена темы

Отредактируйте `src/ui.c`:

```c
#include "styles/dark/style_dark.h"
// ...
GuiLoadStyleDark();
```

Доступные темы в `styles/`:
- `dark` — тёмная современная
- `terminal` — зелёный хакерский стиль
- `cyber` — киберпанк
- `cherry` — красные акценты
- `jungle` — зелёные акценты
- `lavanda` — фиолетовые акценты

---

