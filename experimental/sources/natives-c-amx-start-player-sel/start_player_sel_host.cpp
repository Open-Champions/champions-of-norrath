#include <cstdio>

extern "C" int amxStartPlayerSel__FP5__amxPi(void*, int*);

static int starts;

extern "C" void playerSelectionStart__Fv() { starts += 1; }

int main() {
    int params[1] = {0};
    if (amxStartPlayerSel__FP5__amxPi(nullptr, params) != 0) return 1;
    if (starts != 1) return 2;
    std::printf("start player sel passed\n");
    return 0;
}
