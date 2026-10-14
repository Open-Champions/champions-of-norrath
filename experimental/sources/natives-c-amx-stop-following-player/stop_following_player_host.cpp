#include <cstdio>
#include <cstring>

extern "C" int amxStopFollowingPlayer__FP5__amxPi(void*, int*);

static unsigned char flagged[176];
static unsigned char plain[176];

extern "C" int amx_GetAddr(void*, int, int** physical) {
    static int slot;
    *physical = &slot;
    return 0;
}

extern "C" int amx_GetString(char* dest, int*) {
    std::snprintf(dest, 160, "%s", "guard");
    return 0;
}

extern "C" void objectFindObject__FPcPP10GameObjecti(char*, void** objects, int count) {
    if (count != 512) objects[0] = nullptr;
    else {
        objects[0] = flagged;
        objects[1] = plain;
        objects[2] = nullptr;
    }
}

static unsigned long long wide(unsigned char* object) {
    unsigned long long value = 0;
    std::memcpy(&value, object + 160, 8);
    return value;
}

int main() {
    int flags = 0x20;
    int none = 0;
    unsigned long long bits = 0x1000000080ffffffull;
    std::memcpy(flagged + 12, &flags, 4);
    std::memcpy(plain + 12, &none, 4);
    std::memcpy(flagged + 160, &bits, 8);
    std::memcpy(plain + 160, &bits, 8);
    int params[2] = {4, 1};
    if (amxStopFollowingPlayer__FP5__amxPi(nullptr, params) != 0) return 1;
    if (wide(flagged) != (bits & ~0x00800000ull)) return 2;
    if (wide(plain) != bits) return 3;
    std::printf("stop following player passed\n");
    return 0;
}
