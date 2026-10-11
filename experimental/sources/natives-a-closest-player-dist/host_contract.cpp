#include <cstring>
unsigned char arena[4096];
unsigned char* norrath_native_host_base = arena;
extern "C" int nmPlayersAlive = 0;
extern "C" int playersAlive[4] = {};
extern "C" unsigned char player[95936] = {};
static unsigned char object[40];
extern "C" int amx_GetAddr(void*, int, int** physical) {
    static int cell;
    *physical = &cell;
    return 0;
}
extern "C" int amx_GetString(char* destination, int*) {
    std::strcpy(destination, "marker");
    return 0;
}
extern "C" unsigned char* objectFindObject__FPc(char*) { return object; }
extern "C" int amxClosestPlayerDist__FP5__amxPi(void*, int*);
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
    nmPlayersAlive = 0;
    if (amxClosestPlayerDist__FP5__amxPi(0, params) != 100000) return 1;
    nmPlayersAlive = 1;
    playersAlive[0] = 0;
    store_u32(player + 21872, 0u);
    if (amxClosestPlayerDist__FP5__amxPi(0, params) != 100000) return 2;
    store_u32(player + 21872, 200u);
    store_f32(object + 16, 0.0f);
    store_f32(object + 20, 0.0f);
    store_f32(object + 24, 0.0f);
    store_f32(arena + 200 + 16, 3.0f);
    store_f32(arena + 200 + 20, 4.0f);
    store_f32(arena + 200 + 24, 0.0f);
    if (amxClosestPlayerDist__FP5__amxPi(0, params) != 5) return 3;
    store_f32(arena + 200 + 16, 1.5f);
    store_f32(arena + 200 + 20, 0.0f);
    if (amxClosestPlayerDist__FP5__amxPi(0, params) != 2) return 4;
    nmPlayersAlive = 2;
    playersAlive[1] = 1;
    store_u32(player + 23984 + 21872, 400u);
    store_f32(arena + 400 + 16, 0.0f);
    store_f32(arena + 400 + 20, 0.0f);
    store_f32(arena + 400 + 24, 0.0f);
    store_f32(arena + 200 + 16, 6.0f);
    store_f32(arena + 200 + 20, 8.0f);
    if (amxClosestPlayerDist__FP5__amxPi(0, params) != 0) return 5;
    return 0;
}
