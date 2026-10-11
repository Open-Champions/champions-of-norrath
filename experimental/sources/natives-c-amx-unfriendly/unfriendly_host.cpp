#include <cstdio>
#include <cstring>

extern "C" int amxUnFriendly__FP5__amxPi(void*, int*);

static unsigned char object[176];

extern "C" int amx_GetAddr(void*, int, int** physical) {
    static int slot;
    *physical = &slot;
    return 0;
}

extern "C" int amx_GetString(char* dest, int*) {
    std::snprintf(dest, 160, "%s", "mob");
    return 0;
}

extern "C" void objectFindObject__FPcPP10GameObjecti(char*, void** objects, int) {
    objects[0] = object;
    objects[1] = nullptr;
}

int main() {
    int flags = 0x01000020;
    unsigned long long bits = 0xffffffffffffffffull;
    std::memcpy(object + 12, &flags, 4);
    std::memcpy(object + 160, &bits, 8);
    int params[2] = {4, 1};
    if (amxUnFriendly__FP5__amxPi(nullptr, params) != 0) return 1;
    int stored = 0;
    unsigned long long wide = 0;
    std::memcpy(&stored, object + 12, 4);
    std::memcpy(&wide, object + 160, 8);
    if (stored != 0x20) return 2;
    if (wide != (bits & ~0x04000000ull)) return 3;
    std::printf("unfriendly passed\n");
    return 0;
}
