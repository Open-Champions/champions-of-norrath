#include <cstring>
static unsigned char object[32];
static float got_point[3];
static int got_zero = -1;
static float got_scale;
static int got_ten;
static int got_two;
extern "C" int amx_GetAddr(void*, int address, int** physical) {
    static int cell;
    if (address != 7) return 1;
    *physical = &cell;
    return 0;
}
extern "C" int amx_GetString(char* destination, int*) {
    std::strcpy(destination, "pool");
    return 0;
}
extern "C" unsigned char* objectFindObject__FPc(char* name) {
    return std::strcmp(name, "pool") == 0 ? object : 0;
}
extern "C" void floorAdd__FG6Point3sfii(float* point, int zero, float scale, int ten, int two) {
    got_point[0] = point[0];
    got_point[1] = point[1];
    got_point[2] = point[2];
    got_zero = zero;
    got_scale = scale;
    got_ten = ten;
    got_two = two;
}
extern "C" int amxAddExpandingPoolUnder__FP5__amxPi(void*, int*);
static void store_f32(unsigned char* address, float value) {
    union {
        float as_float;
        unsigned as_bits;
    } bits;
    bits.as_float = value;
    address[0] = static_cast<unsigned char>(bits.as_bits);
    address[1] = static_cast<unsigned char>(bits.as_bits >> 8);
    address[2] = static_cast<unsigned char>(bits.as_bits >> 16);
    address[3] = static_cast<unsigned char>(bits.as_bits >> 24);
}
int main() {
    store_f32(object + 16, 1.0f);
    store_f32(object + 20, 2.0f);
    store_f32(object + 24, 3.0f);
    int params[4] = {12, 7, 0, 6};
    if (amxAddExpandingPoolUnder__FP5__amxPi(0, params) != 0) return 1;
    if (got_point[0] != 1.0f || got_point[1] != 2.0f || got_point[2] != 3.0f) return 2;
    if (got_zero != 0 || got_scale != 6.0f || got_ten != 10 || got_two != 2) return 3;
    return 0;
}
