#include <cstdio>

extern "C" int DLG_StartWithSwoopDone = 0;
extern "C" int amxStartWithSwoopDone__FP5__amxPi(void*, int*);

int main() {
    int params[1] = {0};
    DLG_StartWithSwoopDone = 0;
    if (amxStartWithSwoopDone__FP5__amxPi(nullptr, params) != 0) return 1;
    if (DLG_StartWithSwoopDone != 1) return 2;
    DLG_StartWithSwoopDone = 4;
    if (amxStartWithSwoopDone__FP5__amxPi(nullptr, params) != 0) return 3;
    if (DLG_StartWithSwoopDone != 1) return 4;
    std::printf("start with swoop done passed\n");
    return 0;
}
