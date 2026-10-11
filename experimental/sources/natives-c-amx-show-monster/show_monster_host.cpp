#include <cstdio>
#include <cstring>

extern "C" int amxShowMonster__FP5__amxPi(void*, int*);

static unsigned char first[576];
static unsigned char second[576];
static int updates;
static int asked;

extern "C" int amx_GetAddr(void*, int, int** physical) {
    static int slot;
    *physical = &slot;
    return 0;
}

extern "C" int amx_GetString(char* dest, int*) {
    std::snprintf(dest, 160, "%s", "wolf");
    return 0;
}

extern "C" void objectFindObject__FPcPP10GameObjecti(char* name, void** objects, int count) {
    if (std::strcmp(name, "wolf") != 0 || count != 512) asked = -1;
    else asked = count;
    objects[0] = first;
    objects[1] = second;
    objects[2] = nullptr;
}

extern "C" void objectUpdateInGrid__FP10GameObject(void* object) {
    if (object == first || object == second) updates += 1;
}

int main() {
    std::memset(first, 0, sizeof first);
    std::memset(second, 0, sizeof second);
    int flagged = 0x20;
    int plain = 0;
    std::memcpy(first + 12, &flagged, 4);
    std::memcpy(second + 12, &plain, 4);
    first[570] = 0xff;
    second[570] = 0xff;
    int params[2] = {4, 1};
    if (amxShowMonster__FP5__amxPi(nullptr, params) != 0) return 1;
    if (asked != 512 || updates != 2) return 2;
    if (first[570] != 0xf7 || second[570] != 0xff) return 3;
    std::printf("show monster passed\n");
    return 0;
}
