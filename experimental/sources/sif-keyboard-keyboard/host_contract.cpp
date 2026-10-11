#include "keyboard.cpp"

#include <cstdio>

static int calls;
static void* seen;

extern "C" void Free__10VIKeyboard(void* object) {
    ++calls;
    seen = object;
}

int main() {
    if (sizeof vik != 536) return 1;
    keyboardFree__Fv();
    if (calls != 1 || seen != vik) return 2;
    keyboardFree__Fv();
    if (calls != 2 || seen != vik) return 3;
    std::puts("keyboard free passed");
    return 0;
}
