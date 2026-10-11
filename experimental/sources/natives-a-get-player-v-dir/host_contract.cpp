unsigned char arena[8192];
unsigned char* norrath_native_host_base = arena;
extern "C" int nmPlayersAlive = 0;
extern "C" int playersAlive[4] = {};
extern "C" unsigned char player[95936] = {};
extern "C" int amxGetPlayerVDir__FP5__amxPi(void*, int*);
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
static void plant(float along, float gate) {
    store_u32(player + 21872, 100u);
    store_u32(arena + 100 + 12, 0u);
    store_u32(arena + 100 + 1328, 1000u);
    store_f32(arena + 1000 + 840, gate);
    store_f32(arena + 1000 + 1128, along);
    store_f32(arena + 1000 + 1132, 0.0f);
    store_f32(arena + 1000 + 1136, 0.0f);
    store_u32(arena + 1000 + 2320, 2000u);
    store_f32(arena + 2000 + 12, 1.0f);
    store_f32(arena + 2000 + 16, 0.0f);
    store_f32(arena + 2000 + 20, 0.0f);
}
int main() {
    nmPlayersAlive = 0;
    if (amxGetPlayerVDir__FP5__amxPi(0, 0) != 0) return 1;
    nmPlayersAlive = 1;
    playersAlive[0] = 0;
    store_u32(player + 21872, 0u);
    if (amxGetPlayerVDir__FP5__amxPi(0, 0) != 0) return 2;
    plant(-1.0f, 3.0f);
    if (amxGetPlayerVDir__FP5__amxPi(0, 0) != 1) return 3;
    plant(1.0f, 3.0f);
    if (amxGetPlayerVDir__FP5__amxPi(0, 0) != -1) return 4;
    plant(-1.0f, 1.0f);
    if (amxGetPlayerVDir__FP5__amxPi(0, 0) != 0) return 5;
    plant(-1.0f, 3.0f);
    store_u32(arena + 100 + 12, 8u);
    if (amxGetPlayerVDir__FP5__amxPi(0, 0) != 0) return 6;
    return 0;
}
