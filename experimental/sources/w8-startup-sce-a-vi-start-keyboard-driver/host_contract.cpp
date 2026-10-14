#include "start_keyboard_driver.cpp"

#include <cstdio>

static int result;
static int seen[4];
static int calls;
extern "C" int lgKbMInit(int a, int b, int c, int d) {
    seen[0] = a; seen[1] = b; seen[2] = c; seen[3] = d;
    ++calls;
    return result;
}

int main() {
    result = 0;
    if (VIStartKeyboardDriver__Fv() != 0) return 1;
    if (calls != 1 || seen[0] != 1 || seen[1] != 0 || seen[2] != 15 || seen[3] != 0) return 2;
    result = -3;
    if (VIStartKeyboardDriver__Fv() != -1) return 3;
    result = 1;
    if (VIStartKeyboardDriver__Fv() != -1) return 4;
    std::puts("vi start keyboard driver contract passed");
    return 0;
}
