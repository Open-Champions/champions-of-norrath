#include <cstring>
static unsigned char first[200];
static unsigned char second[200];
static int charms;
static unsigned char* charmed;
static int charm_value;
extern "C" int amx_GetAddr(void*, int, int** physical) {
    static int cell;
    *physical = &cell;
    return 0;
}
extern "C" int amx_GetString(char* destination, int*) {
    std::strcpy(destination, "wolf");
    return 0;
}
extern "C" void objectFindObject__FPcPP10GameObjecti(char*, void** list, int) {
    list[0] = first;
    list[1] = second;
    list[2] = 0;
}
extern "C" void charm__8Creaturei(unsigned char* object, int value) {
    ++charms;
    charmed = object;
    charm_value = value;
}
extern "C" int amxFollowPlayer__FP5__amxPi(void*, int*);
static void store_u32(unsigned char* address, unsigned value) {
    address[0] = static_cast<unsigned char>(value);
    address[1] = static_cast<unsigned char>(value >> 8);
    address[2] = static_cast<unsigned char>(value >> 16);
    address[3] = static_cast<unsigned char>(value >> 24);
}
static unsigned long long load_u64(const unsigned char* address) {
    unsigned long long value = 0ull;
    for (int index = 0; index < 8; ++index)
        value |= static_cast<unsigned long long>(address[index]) << (8 * index);
    return value;
}
int main() {
    store_u32(first + 12, 0x20u);
    store_u32(second + 12, 0x10u);
    first[160] = 4;
    second[160] = 5;
    int params[2] = {4, 1};
    if (amxFollowPlayer__FP5__amxPi(0, params) != 0) return 1;
    if (charms != 1 || charmed != first || charm_value != -1) return 2;
    if (load_u64(first + 160) != (4ull | 0x00800000ull)) return 3;
    if (load_u64(second + 160) != 5ull) return 4;
    return 0;
}
