#include <cstdio>
#include <cstring>
extern "C" int amxPropAnimAtEnd__FP5__amxPi(void*, int*);
static unsigned char object[96];
extern "C" int amx_GetAddr(void*, int, void** physical) {
    static char packed[4];
    *physical = packed;
    return 0;
}
extern "C" int amx_GetString(char* destination, void*) {
    std::strcpy(destination, "door");
    return 0;
}
extern "C" void* objectFindObject__FPc(char*) { return object; }

int main() {
    std::memset(object, 0, sizeof object);
    int cells[] = {4, 1};
    object[90] = 0x80;
    if (amxPropAnimAtEnd__FP5__amxPi(nullptr, cells) != 1) return 1;
    object[90] = 0;
    object[91] = 0xff;
    if (amxPropAnimAtEnd__FP5__amxPi(nullptr, cells) != 0) return 2;
    std::puts("amxPropAnimAtEnd passed");
    return 0;
}
