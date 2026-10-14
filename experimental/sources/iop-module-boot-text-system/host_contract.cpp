#include "text_system.cpp"

#include <cstdio>

static int events[4];
static int count;
static void* seen;

extern "C" void VIStartKeyboardDriver__Fv() { events[count++] = 1; }
extern "C" void Init__10VIKeyboard(void* keyboard) {
    events[count++] = 2;
    seen = keyboard;
}

int main() {
    if (sizeof vik != 536) return 1;
    textSystemInit__Fv();
    textSystemInit__Fv();
    if (count != 4 || events[0] != 1 || events[1] != 2 || events[2] != 1 || events[3] != 2) return 3;
    if (seen != vik) return 4;
    std::puts("textSystemInit passed");
    return 0;
}
