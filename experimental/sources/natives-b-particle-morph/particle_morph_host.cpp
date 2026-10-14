#include <cstdio>
#include <cstring>
extern "C" int amxParticleMorph__FP5__amxPi(void*, int*);
static unsigned char first[16];
static unsigned char second[16];
static int block_marker = 1;
static int constructs = 0;
static int allocs = 0;
extern "C" int amx_GetAddr(void*, int, void** physical) {
    static char packed[4];
    *physical = packed;
    return 0;
}
extern "C" int amx_GetString(char* destination, void*) {
    static int which = 0;
    std::strcpy(destination, (which & 1) == 0 ? "a" : "b");
    which += 1;
    return 0;
}
extern "C" void objectFindObject__FPcPP10GameObjecti(char* name, void** list, int) {
    list[0] = std::strcmp(name, "a") == 0 ? first : second;
    list[1] = &block_marker;
    list[2] = nullptr;
}
extern "C" void* blockAlloc__Fi(int bytes) {
    allocs += 1;
    return bytes == 92 ? &block_marker : nullptr;
}
extern "C" void __19ParticleMorphEffectP8CreatureT1(void* block, void* left, void* right) {
    constructs += 1;
    if (block != &block_marker || left != first || right != second) constructs = -100;
}

int main() {
    std::memset(first, 0, sizeof first);
    std::memset(second, 0, sizeof second);
    int cells[] = {8, 1, 2};
    if (amxParticleMorph__FP5__amxPi(nullptr, cells) != 0) return 1;
    if (allocs != 0 || constructs != 0) return 2;
    first[12] = 0x20;
    second[12] = 0x20;
    if (amxParticleMorph__FP5__amxPi(nullptr, cells) != 0) return 3;
    if (allocs != 1 || constructs != 1) return 4;
    std::puts("amxParticleMorph passed");
    return 0;
}
