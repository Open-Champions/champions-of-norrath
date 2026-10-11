#include "keyboard.cpp"

#include <cstdio>

static int free_calls;
static int init_calls;
static void* seen_free;
static void* seen_init;

extern "C" void Free__10VIKeyboard(void* object) {
    ++free_calls;
    seen_free = object;
}
extern "C" void Init__10VIKeyboard(void* object) {
    ++init_calls;
    seen_init = object;
}

int main() {
    if (sizeof vik != 536) return 1;
    keyboardFree__Fv();
    if (free_calls != 1 || init_calls != 0 || seen_free != vik) return 2;
    keyboardRestart__Fv();
    if (init_calls != 1 || free_calls != 1 || seen_init != vik) return 3;
    keyboardFree__Fv();
    keyboardRestart__Fv();
    if (free_calls != 2 || init_calls != 2 || seen_free != vik || seen_init != vik) return 4;
    std::puts("keyboard free and restart passed");
    return 0;
}
