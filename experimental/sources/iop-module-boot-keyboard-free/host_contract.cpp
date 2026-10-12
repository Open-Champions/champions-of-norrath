#include "keyboard_free.cpp"

#include <cstdio>
#include <cstdlib>
#include <cstring>

static int prints;
static int closes;
static int closed_handle;

extern "C" int printf(const char* text, ...) {
    ++prints;
    if (std::strcmp(text, "======= FREE KEYBOARD =======\n") != 0) std::exit(2);
    return 1;
}
extern "C" void lgKbdClose(int handle) {
    ++closes;
    closed_handle = handle;
}

int main() {
    unsigned char object[16];
    std::memset(object, 0x5a, sizeof object);
    store_word(object + 4, 0xffffffffu);
    Free__10VIKeyboard(object);
    if (prints != 1 || closes != 0 || load_word(object + 4) != 0xffffffffu) return 3;
    if (object[0] != 0x5a || object[8] != 0x5a) return 4;

    store_word(object + 4, 5);
    Free__10VIKeyboard(object);
    if (prints != 2 || closes != 1 || closed_handle != 5) return 5;
    if (load_word(object + 4) != 0xffffffffu || object[0] != 0x5a) return 6;

    store_word(object + 4, 0);
    Free__10VIKeyboard(object);
    if (closes != 2 || closed_handle != 0 || load_word(object + 4) != 0xffffffffu) return 7;
    std::puts("Free keyboard passed");
    return 0;
}
