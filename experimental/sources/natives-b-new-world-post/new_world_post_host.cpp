#include <cstdio>
#include <cstring>
extern "C" int amxNewWorldPost__FP5__amxPi(void*, int*);
extern "C" char DLG_NewWorld[64] = {};
extern "C" int amx_GetAddr(void*, int address, void** physical) {
    static char packed[4];
    if (address != 15) return 1;
    *physical = packed;
    return 0;
}
extern "C" int amx_GetString(char* destination, void*) {
    std::strcpy(destination, "zone");
    return 0;
}

int main() {
    std::memset(DLG_NewWorld, 0x5a, sizeof DLG_NewWorld);
    int cells[] = {4, 15};
    if (amxNewWorldPost__FP5__amxPi(nullptr, cells) != 0) return 1;
    if (std::strcmp(DLG_NewWorld, "zone") != 0) return 2;
    std::puts("amxNewWorldPost passed");
    return 0;
}
