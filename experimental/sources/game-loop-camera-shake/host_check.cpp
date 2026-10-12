// Host contract for gameSetCameraShake. Not an EE plan input.
#include "camera_shake.cpp"

#include <cstring>

extern "C" {
float gameCameraShake = 0.0f;
int nmPlayers = 0;
unsigned char player[23984 * 3];
}

static int rumble_calls = 0;
static int rumble_index[4];
static unsigned rumble_bits[4];
static unsigned rumble_second[4];
static int nm_at_rumble[4];

static void store_u32(unsigned char* address, unsigned value) {
    address[0] = static_cast<unsigned char>(value);
    address[1] = static_cast<unsigned char>(value >> 8);
    address[2] = static_cast<unsigned char>(value >> 16);
    address[3] = static_cast<unsigned char>(value >> 24);
}

static void store_f32(unsigned char* address, float value) {
    unsigned bits;
    std::memcpy(&bits, &value, 4);
    store_u32(address, bits);
}

static unsigned bits_of(float value) {
    unsigned bits;
    std::memcpy(&bits, &value, 4);
    return bits;
}

extern "C" void padBigRumble__Fiff(int index, float amount, float second) {
    if (rumble_calls < 4) {
        rumble_index[rumble_calls] = index;
        rumble_bits[rumble_calls] = bits_of(amount);
        rumble_second[rumble_calls] = bits_of(second);
        nm_at_rumble[rumble_calls] = nmPlayers;
    }
    ++rumble_calls;
    if (rumble_calls == 1) nmPlayers = 1;
}

static float from_bits(unsigned bits) {
    float value;
    std::memcpy(&value, &bits, 4);
    return value;
}

int main() {
    gameCameraShake = 2.0f;
    gameSetCameraShake__Ffb(2.0f, 1);
    if (bits_of(gameCameraShake) != bits_of(2.0f) || rumble_calls != 0) return 1;
    gameSetCameraShake__Ffb(1.5f, 0);
    if (bits_of(gameCameraShake) != bits_of(2.0f) || rumble_calls != 0) return 2;
    unsigned nan_bits = 0x7fc00000u;
    gameSetCameraShake__Ffb(from_bits(nan_bits), 0);
    if (bits_of(gameCameraShake) != bits_of(2.0f)) return 3;

    gameSetCameraShake__Ffb(4.0f, 2);
    if (bits_of(gameCameraShake) != bits_of(4.0f) || rumble_calls != 0) return 4;

    gameCameraShake = 9.0f;
    nmPlayers = 0;
    gameSetCameraShake__Ffb(1.0f, 0);
    if (rumble_calls != 0 || bits_of(gameCameraShake) != bits_of(9.0f)) return 5;

    nmPlayers = 3;
    store_u32(player + 21872, 0u);
    store_f32(player + 172, 5.0f);
    store_u32(player + 23984 + 21872, 1u);
    store_f32(player + 23984 + 172, 0.0f);
    store_u32(player + 23984 * 2 + 21872, 1u);
    store_f32(player + 23984 * 2 + 172, 1.0f);
    gameCameraShake = 8.0f;
    gameSetCameraShake__Ffb(3.0f, 0);
    if (rumble_calls != 1) return 6;
    if (rumble_index[0] != 2) return 7;
    float expected = 3.0f * from_bits(0x40400000u) * from_bits(0x3c888888u);
    if (rumble_bits[0] != bits_of(expected)) return 8;
    if (rumble_second[0] != 0x3f800000u) return 9;
    if (bits_of(gameCameraShake) != bits_of(8.0f)) return 10;

    rumble_calls = 0;
    nmPlayers = 2;
    store_u32(player + 21872, 4u);
    store_f32(player + 172, 1.0f);
    store_u32(player + 23984 + 21872, 4u);
    store_f32(player + 23984 + 172, 1.0f);
    gameSetCameraShake__Ffb(1.0f, 0);
    if (rumble_calls != 1 || rumble_index[0] != 0 || nm_at_rumble[0] != 2) return 11;
    return 0;
}
