#include <cstring>
static unsigned char object[1100];
static int find_null;
extern "C" int amx_GetAddr(void*, int, int** physical) {
    static int cell;
    *physical = &cell;
    return 0;
}
extern "C" int amx_GetString(char* destination, int*) {
    std::strcpy(destination, "cart");
    return 0;
}
extern "C" unsigned char* objectFindObject__FPc(char* name) {
    if (std::strcmp(name, "cart") != 0) return 0;
    return find_null ? 0 : object;
}
extern "C" int amxControlVehicle__FP5__amxPi(void*, int*);
static unsigned load_u32(const unsigned char* address) {
    return static_cast<unsigned>(address[0]) | (static_cast<unsigned>(address[1]) << 8) |
           (static_cast<unsigned>(address[2]) << 16) | (static_cast<unsigned>(address[3]) << 24);
}
int main() {
    find_null = 1;
    int params[4] = {12, 1, 5, 3};
    object[1040] = 9;
    if (amxControlVehicle__FP5__amxPi(0, params) != 0 || object[1040] != 9) return 1;
    find_null = 0;
    if (amxControlVehicle__FP5__amxPi(0, params) != 0) return 2;
    if (load_u32(object + 1040) != 5u) return 3;
    unsigned bits = load_u32(object + 752);
    union {
        unsigned as_bits;
        float as_float;
    } value;
    value.as_bits = bits;
    if (value.as_float != 3.0f) return 4;
    params[3] = -2;
    if (amxControlVehicle__FP5__amxPi(0, params) != 0) return 5;
    value.as_bits = load_u32(object + 752);
    if (value.as_float != -2.0f) return 6;
    return 0;
}
