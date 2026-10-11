#include <cstdio>
#include <cstring>

extern "C" int amxSetNoCollide__FP5__amxPi(void*, int*);

static unsigned char object[16];

extern "C" int amx_GetAddr(void*, int, int** physical) {
    static int cell;
    *physical = &cell;
    return 0;
}

extern "C" int amx_GetString(char* dest, int*) {
    std::snprintf(dest, 160, "%s", "crate");
    return 0;
}

extern "C" void* objectFindObject__FPc(char*) { return object; }

static int flags() {
    int value = 0;
    std::memcpy(&value, object + 12, 4);
    return value;
}

int main() {
    int initial = 0x10;
    std::memcpy(object + 12, &initial, 4);
    int params[3] = {8, 1, 1};
    if (amxSetNoCollide__FP5__amxPi(nullptr, params) != 0) return 1;
    if (flags() != 0x11) return 2;
    params[2] = 0;
    if (amxSetNoCollide__FP5__amxPi(nullptr, params) != 0) return 3;
    if (flags() != 0x10) return 4;
    params[2] = 4;
    if (amxSetNoCollide__FP5__amxPi(nullptr, params) != 0 || flags() != 0x11) return 5;
    std::printf("set no collide passed\n");
    return 0;
}
