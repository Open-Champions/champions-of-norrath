#include <cstring>
unsigned char arena[1024];
unsigned char* norrath_native_host_base = arena;
static int which;
static unsigned char* saw_self;
static float saw_in[3];
static unsigned char* updated;
static void ride(unsigned char* self, float* in_point, float* out_point, unsigned short* out_half) {
    saw_self = self;
    saw_in[0] = in_point[0];
    saw_in[1] = in_point[1];
    saw_in[2] = in_point[2];
    out_point[0] = 4.0f;
    out_point[1] = 5.0f;
    out_point[2] = 6.0f;
    *out_half = 7;
}
void (*norrath_native_host_ride[2])(unsigned char*, float*, float*, unsigned short*) = {0, ride};
extern "C" int amx_GetAddr(void*, int, int** physical) {
    static int cell;
    *physical = &cell;
    return 0;
}
extern "C" int amx_GetString(char* destination, int*) {
    std::strcpy(destination, which == 0 ? "rider" : "cart");
    ++which;
    return 0;
}
extern "C" unsigned char* objectFindObject__FPc(char* name) {
    if (std::strcmp(name, "cart") == 0) return arena + 200;
    if (std::strcmp(name, "rider") == 0) return arena + 400;
    return 0;
}
extern "C" void objectUpdateInGrid__FP10GameObject(unsigned char* object) { updated = object; }
extern "C" int amxEntityRideVehicle__FP5__amxPi(void*, int*);
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
int main() {
    store_u32(arena + 200 + 72, 600u);
    arena[600 + 152] = 8;
    arena[600 + 153] = 0;
    store_u32(arena + 600 + 156, 1u);
    int params[7] = {24, 1, 2, 9, 8, 7, 0x00030004};
    if (amxEntityRideVehicle__FP5__amxPi(0, params) != 0) return 1;
    if (saw_self != arena + 208) return 2;
    if (saw_in[0] != 9.0f || saw_in[1] != 8.0f || saw_in[2] != 7.0f) return 3;
    union {
        float as_float;
        unsigned as_bits;
    } bits;
    bits.as_bits = load_u32(arena + 400 + 16);
    if (bits.as_float != 4.0f) return 4;
    bits.as_bits = load_u32(arena + 400 + 20);
    if (bits.as_float != 5.0f) return 5;
    bits.as_bits = load_u32(arena + 400 + 24);
    if (bits.as_float != 6.0f) return 6;
    unsigned half = static_cast<unsigned>(arena[400 + 56]) | (static_cast<unsigned>(arena[400 + 57]) << 8);
    if (half != 11u || updated != arena + 400) return 7;
    return 0;
}
