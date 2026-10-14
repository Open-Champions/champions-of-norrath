#include <cstdio>
#include <cstring>

extern "C" int amxStopFollowingPath__FP5__amxPi(void*, int*);

static unsigned char object[800];

extern "C" int amx_GetAddr(void*, int, int** physical) {
    static int slot;
    *physical = &slot;
    return 0;
}

extern "C" int amx_GetString(char* dest, int*) {
    std::snprintf(dest, 160, "%s", "path");
    return 0;
}

extern "C" void objectFindObject__FPcPP10GameObjecti(char*, void** objects, int) {
    objects[0] = object;
    objects[1] = nullptr;
}

int main() {
    std::memset(object, 0x11, sizeof object);
    int flags = 0x20;
    unsigned long long bits = 0xffffffffffffffffull;
    unsigned char point[12] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    std::memcpy(object + 12, &flags, 4);
    std::memcpy(object + 160, &bits, 8);
    std::memcpy(object + 16, point, sizeof point);
    int params[2] = {4, 1};
    if (amxStopFollowingPath__FP5__amxPi(nullptr, params) != 0) return 1;
    unsigned long long stored = 0;
    std::memcpy(&stored, object + 160, 8);
    if (stored != (bits & ~0x08000000ull)) return 2;
    if (std::memcmp(object + 776, point, sizeof point) != 0) return 3;
    std::printf("stop following path passed\n");
    return 0;
}
