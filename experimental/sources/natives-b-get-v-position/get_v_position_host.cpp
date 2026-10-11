#include <cstdio>
#include <cstring>
extern "C" int amxGetVPosition__FP5__amxPi(void*, int*);
static unsigned char object[2320];
static int missing = 0;
extern "C" int amx_GetAddr(void*, int, void** physical) {
    static char packed[4];
    *physical = packed;
    return 0;
}
extern "C" int amx_GetString(char* destination, void*) {
    std::strcpy(destination, "crate");
    return 0;
}
extern "C" void* objectFindObject__FPc(char*) {
    return missing ? nullptr : object;
}

int main() {
    std::memset(object, 0, sizeof object);
    int cells[] = {4, 1};
    missing = 1;
    if (amxGetVPosition__FP5__amxPi(nullptr, cells) != 0) return 1;
    missing = 0;
    const unsigned exact = 0x3f800000u;
    object[2316] = static_cast<unsigned char>(exact);
    object[2317] = static_cast<unsigned char>(exact >> 8);
    object[2318] = static_cast<unsigned char>(exact >> 16);
    object[2319] = static_cast<unsigned char>(exact >> 24);
    if (amxGetVPosition__FP5__amxPi(nullptr, cells) != 128) return 2;
    const unsigned under = 0x3f7f3333u;
    object[2316] = static_cast<unsigned char>(under);
    object[2317] = static_cast<unsigned char>(under >> 8);
    object[2318] = static_cast<unsigned char>(under >> 16);
    object[2319] = static_cast<unsigned char>(under >> 24);
    if (amxGetVPosition__FP5__amxPi(nullptr, cells) != 128) return 3;
    std::puts("amxGetVPosition passed");
    return 0;
}
