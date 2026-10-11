#include <cstring>
static unsigned char object[580];
static int missing;
extern "C" int amx_GetAddr(void*, int, int** physical) {
    static int cell;
    *physical = &cell;
    return 0;
}
extern "C" int amx_GetString(char* destination, int*) {
    std::strcpy(destination, "hero");
    return 0;
}
extern "C" unsigned char* objectFindObject__FPc(char* name) {
    if (std::strcmp(name, "hero") != 0 || missing) return 0;
    return object;
}
extern "C" int amxGetHitPoints__FP5__amxPi(void*, int*);
static void store_u32(unsigned char* address, unsigned value) {
    address[0] = static_cast<unsigned char>(value);
    address[1] = static_cast<unsigned char>(value >> 8);
    address[2] = static_cast<unsigned char>(value >> 16);
    address[3] = static_cast<unsigned char>(value >> 24);
}
static void store_f32(unsigned char* address, float value) {
    union {
        float as_float;
        unsigned as_bits;
    } bits;
    bits.as_float = value;
    store_u32(address, bits.as_bits);
}
int main() {
    int params[2] = {4, 1};
    missing = 1;
    if (amxGetHitPoints__FP5__amxPi(0, params) != 0) return 1;
    missing = 0;
    store_u32(object + 12, 0u);
    store_f32(object + 560, 1.5f);
    if (amxGetHitPoints__FP5__amxPi(0, params) != 0) return 2;
    store_u32(object + 12, 0x20u);
    if (amxGetHitPoints__FP5__amxPi(0, params) != 2) return 3;
    store_f32(object + 560, -1.5f);
    if (amxGetHitPoints__FP5__amxPi(0, params) != -2) return 4;
    store_f32(object + 560, 0.5f);
    if (amxGetHitPoints__FP5__amxPi(0, params) != 0) return 5;
    store_f32(object + 560, 2.5f);
    if (amxGetHitPoints__FP5__amxPi(0, params) != 2) return 6;
    return 0;
}
