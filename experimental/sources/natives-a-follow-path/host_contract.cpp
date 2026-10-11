#include <cstring>
static unsigned char first[200];
static unsigned char second[200];
static int limit_seen;
extern "C" int amx_GetAddr(void*, int, int** physical) {
    static int cell;
    *physical = &cell;
    return 0;
}
extern "C" int amx_GetString(char* destination, int*) {
    std::strcpy(destination, "guard");
    return 0;
}
extern "C" void objectFindObject__FPcPP10GameObjecti(char* name, void** list, int limit) {
    limit_seen = std::strcmp(name, "guard") == 0 ? limit : -1;
    list[0] = first;
    list[1] = second;
    list[2] = 0;
}
extern "C" int amxFollowPath__FP5__amxPi(void*, int*);
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
    int params[2] = {4, 1};
    store_u32(first + 12, 0x20u);
    store_u32(second + 12, 0u);
    first[160] = 1;
    second[160] = 2;
    if (amxFollowPath__FP5__amxPi(0, params) != 0 || limit_seen != 512) return 1;
    if (load_u64(first + 160) != (1ull | 0x08000000ull)) return 2;
    if (load_u64(second + 160) != 2ull) return 3;
    return 0;
}
