#include <cstdio>
#include <cstring>
extern "C" int amxGetv__FP5__amxPi(void*, int*);
static int seen_address = -1;
static char packed[4];
extern "C" int amx_GetAddr(void*, int address, void** physical) {
    seen_address = address;
    *physical = packed;
    return 5;
}
extern "C" int amx_GetString(char* destination, void*) {
    std::strcpy(destination, "quest");
    return 0;
}
extern "C" int scriptGetGameState__FPc(char* name) {
    return std::strcmp(name, "quest") == 0 ? 14 : -1;
}

int main() {
    int cells[] = {4, 21, 0};
    if (amxGetv__FP5__amxPi(nullptr, cells) != 14) return 1;
    if (seen_address != 21) return 2;
    std::puts("amxGetv passed");
    return 0;
}
