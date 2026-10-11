#include <cstdio>
#include <cstring>
extern "C" int amxPreloadResource__FP5__amxPi(void*, int*);
static int seen_address = -1;
static char loaded[160];
static int loads = 0;
static char packed[4];

extern "C" int amx_GetAddr(void*, int address, void** physical) {
    seen_address = address;
    *physical = packed;
    return 0;
}
extern "C" int amx_GetString(char* destination, void* cells) {
    if (cells != packed) std::strcpy(destination, "bad");
    else std::strcpy(destination, "door.lmp");
    return 0;
}
extern "C" int lumpLoad__FPc(char* name) {
    loads += 1;
    std::strcpy(loaded, name);
    return 4;
}

int main() {
    int cells[] = {4, 18};
    if (amxPreloadResource__FP5__amxPi(nullptr, cells) != 0) return 1;
    if (seen_address != 18 || loads != 1) return 2;
    if (std::strcmp(loaded, "door.lmp") != 0) return 3;
    std::puts("amxPreloadResource passed");
    return 0;
}
