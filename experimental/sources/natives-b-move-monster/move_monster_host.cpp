#include <cstdio>
#include <cstring>
extern "C" int amxMoveMonster__FP5__amxPi(void*, int*);
static unsigned char destination[58];
static unsigned char first[58];
static unsigned char second[58];
static int updates = 0;
static int multi_limit = 0;
static int missing_destination = 0;
extern "C" int amx_GetAddr(void*, int, void** physical) {
    static char packed[4];
    *physical = packed;
    return 0;
}
extern "C" int amx_GetString(char* destination_text, void*) {
    static int which = 0;
    std::strcpy(destination_text, (which & 1) == 0 ? "mob" : "mark");
    which += 1;
    return 0;
}
extern "C" void objectFindObject__FPcPP10GameObjecti(char* name, void** list, int limit) {
    multi_limit = limit;
    if (std::strcmp(name, "mob") != 0) multi_limit = -1;
    list[0] = first;
    list[1] = second;
    list[2] = nullptr;
}
extern "C" void* objectFindObject__FPc(char* name) {
    if (missing_destination || std::strcmp(name, "mark") != 0) return nullptr;
    return destination;
}
extern "C" void objectUpdateInGrid__FP10GameObject(void* object) {
    updates += 1;
    const unsigned char* bytes = static_cast<unsigned char*>(object);
    if (bytes[56] != 0x34 || bytes[57] != 0x12) updates = -100;
}

int main() {
    std::memset(destination, 0, sizeof destination);
    std::memset(first, 0, sizeof first);
    std::memset(second, 0, sizeof second);
    for (int index = 0; index < 12; ++index) destination[16 + index] = static_cast<unsigned char>(index + 1);
    destination[56] = 0x34;
    destination[57] = 0x12;
    int cells[] = {8, 1, 2};
    missing_destination = 1;
    if (amxMoveMonster__FP5__amxPi(nullptr, cells) != 0) return 1;
    if (updates != 0) return 2;
    missing_destination = 0;
    if (amxMoveMonster__FP5__amxPi(nullptr, cells) != 0) return 3;
    if (updates != 2 || multi_limit != 512) return 4;
    if (std::memcmp(first + 16, destination + 16, 12) != 0) return 5;
    if (std::memcmp(second + 16, destination + 16, 12) != 0) return 6;
    std::puts("amxMoveMonster passed");
    return 0;
}
