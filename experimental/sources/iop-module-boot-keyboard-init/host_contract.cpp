#include "keyboard_init.cpp"

#include <cstdio>
#include <cstring>

static int prints;
static int attached_at_print;

extern "C" int printf(const char* text, ...) {
    ++prints;
    attached_at_print = gKeyboardAttached;
    if (std::strcmp(text, "======= INIT KEYBOARD =======\n") != 0) return 9;
    return 4;
}

static unsigned load_word(const unsigned char* slot) {
    return slot[0] | (slot[1] << 8) | (slot[2] << 16) | (slot[3] << 24);
}

int main() {
    if (sizeof sQueue != 264) return 1;
    gKeyboardAttached = 9;
    alignas(4) unsigned char object[536];
    std::memset(object, 0xa5, sizeof object);
    if (Init__10VIKeyboard(object) != 0) return 2;
    if (prints != 1 || attached_at_print != 0 || gKeyboardAttached != 0) return 3;
    if (load_word(object) != 0) return 4;
    if (load_word(object + 4) != 0xffffffffu) return 5;
    const unsigned queue = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(sQueue));
    if (load_word(object + 8) != queue) return 6;
    if (load_word(object + 12) != 0 || load_word(object + 528) != 0) return 7;
    for (int index = 16; index < 528; ++index)
        if (object[index] != 0xa5) return 8;
    if (object[532] != 0xa5 || object[535] != 0xa5) return 10;
    std::puts("Init keyboard passed");
    return 0;
}
