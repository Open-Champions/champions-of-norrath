#include <cstdio>
#include <cstring>
extern "C" int amxNPCAnimAtEnd__FP5__amxPi(void*, int*);
static unsigned char object[300];
static int seen_address = -1;
static char packed[4];
extern "C" int amx_GetAddr(void*, int address, void** physical) {
    seen_address = address;
    *physical = packed;
    return 0;
}
extern "C" int amx_GetString(char* destination, void*) {
    std::strcpy(destination, "guard");
    return 0;
}
extern "C" void* objectFindObject__FPc(char* name) {
    if (std::strcmp(name, "guard") != 0) return nullptr;
    return object;
}

int main() {
    std::memset(object, 0, sizeof object);
    int cells[] = {4, 8};
    object[294] = 0x80;
    object[295] = 0;
    if (amxNPCAnimAtEnd__FP5__amxPi(nullptr, cells) != 1) return 1;
    object[294] = 0x7f;
    if (amxNPCAnimAtEnd__FP5__amxPi(nullptr, cells) != 0) return 2;
    object[294] = 0;
    object[295] = 0x80;
    if (amxNPCAnimAtEnd__FP5__amxPi(nullptr, cells) != 0) return 3;
    if (seen_address != 8) return 4;
    std::puts("amxNPCAnimAtEnd passed");
    return 0;
}
