#include <cstring>
static unsigned char first[200];
static unsigned char second[200];
extern "C" int amx_GetAddr(void*, int, int** physical) {
    static int cell;
    *physical = &cell;
    return 0;
}
extern "C" int amx_GetString(char* destination, int*) {
    std::strcpy(destination, "npc");
    return 0;
}
extern "C" void objectFindObject__FPcPP10GameObjecti(char*, void** list, int limit) {
    if (limit != 512) list[0] = 0;
    else {
        list[0] = first;
        list[1] = second;
        list[2] = 0;
    }
}
extern "C" int amxFriendly__FP5__amxPi(void*, int*);
static void store_u32(unsigned char* address, unsigned value) {
    address[0] = static_cast<unsigned char>(value);
    address[1] = static_cast<unsigned char>(value >> 8);
    address[2] = static_cast<unsigned char>(value >> 16);
    address[3] = static_cast<unsigned char>(value >> 24);
}
static unsigned load_u32(const unsigned char* address) {
    return static_cast<unsigned>(address[0]) | (static_cast<unsigned>(address[1]) << 8) |
           (static_cast<unsigned>(address[2]) << 16) | (static_cast<unsigned>(address[3]) << 24);
}
static unsigned long long load_u64(const unsigned char* address) {
    unsigned long long value = 0ull;
    for (int index = 0; index < 8; ++index)
        value |= static_cast<unsigned long long>(address[index]) << (8 * index);
    return value;
}
int main() {
    store_u32(first + 12, 0x20u);
    store_u32(second + 12, 0x01u);
    first[160] = 3;
    second[160] = 4;
    int params[2] = {4, 1};
    if (amxFriendly__FP5__amxPi(0, params) != 0) return 1;
    if (load_u32(first + 12) != (0x20u | 0x01000000u)) return 2;
    if (load_u64(first + 160) != (3ull | 0x04000000ull)) return 3;
    if (load_u32(second + 12) != 0x01u || load_u64(second + 160) != 4ull) return 4;
    return 0;
}
