unsigned char arena[8192];
unsigned char* norrath_native_host_base = arena;
extern "C" int nmPlayers = 0;
extern "C" unsigned char player[95936] = {};
extern "C" int amxGetPlayerVPosition__FP5__amxPi(void*, int*);
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
    nmPlayers = 0;
    if (amxGetPlayerVPosition__FP5__amxPi(0, 0) != 0) return 1;
    nmPlayers = 2;
    store_u32(player + 21872, 100u);
    store_u32(player + 23984 + 21872, 300u);
    store_u32(arena + 100 + 12, 0u);
    store_u32(arena + 100 + 1328, 1000u);
    store_f32(arena + 1000 + 2316, 10.0f);
    store_f32(arena + 1000 + 844, 1.5f);
    store_u32(arena + 300 + 12, 8u);
    store_u32(arena + 300 + 1328, 2000u);
    store_f32(arena + 2000 + 2316, 50.0f);
    store_f32(arena + 2000 + 844, 9.0f);
    if (amxGetPlayerVPosition__FP5__amxPi(0, 0) != 192) return 2;
    store_u32(arena + 300 + 12, 0u);
    store_f32(arena + 2000 + 2316, 0.5f);
    if (amxGetPlayerVPosition__FP5__amxPi(0, 0) != 192) return 3;
    store_f32(arena + 2000 + 2316, 4.0f);
    store_f32(arena + 2000 + 844, 2.0f);
    if (amxGetPlayerVPosition__FP5__amxPi(0, 0) != 256) return 4;
    return 0;
}
