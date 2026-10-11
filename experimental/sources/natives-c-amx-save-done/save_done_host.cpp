#include <cstdio>

extern "C" int amxSaveDone__FP5__amxPi(void*, int*);

static int mode;

extern "C" int MC_GetAsyncMode__Fv() { return mode; }

int main() {
    int params[1] = {0};
    mode = -1;
    if (amxSaveDone__FP5__amxPi(nullptr, params) != 1) return 1;
    mode = 0;
    if (amxSaveDone__FP5__amxPi(nullptr, params) != 0) return 2;
    mode = 1;
    if (amxSaveDone__FP5__amxPi(nullptr, params) != 0) return 3;
    mode = 5;
    if (amxSaveDone__FP5__amxPi(nullptr, params) != 0) return 4;
    mode = -2;
    if (amxSaveDone__FP5__amxPi(nullptr, params) != 0) return 5;
    std::printf("save done passed\n");
    return 0;
}
