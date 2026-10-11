// Host contract for gameCheckPlayerAlive. Not an EE plan input.
#include "check_player_alive.cpp"

#include <cstring>

extern "C" {
int nmPlayers = 0;
unsigned char player[23984 * 4];
}

static void store_f32(unsigned char* address, float value) {
    unsigned bits;
    std::memcpy(&bits, &value, 4);
    address[0] = static_cast<unsigned char>(bits);
    address[1] = static_cast<unsigned char>(bits >> 8);
    address[2] = static_cast<unsigned char>(bits >> 16);
    address[3] = static_cast<unsigned char>(bits >> 24);
}

static int fail(int code) { return code; }

int main() {
    if (gameCheckPlayerAlive__Fv() != 0) return fail(1);
    nmPlayers = -3;
    store_f32(player + 172, 4.0f);
    if (gameCheckPlayerAlive__Fv() != 0) return fail(2);

    nmPlayers = 1;
    store_f32(player + 172, 0.0f);
    store_f32(player + 172 + 23984, 9.0f);
    if (gameCheckPlayerAlive__Fv() != 0) return fail(3);

    store_f32(player + 172, 0.25f);
    if (gameCheckPlayerAlive__Fv() != 1) return fail(4);

    nmPlayers = 4;
    store_f32(player + 172, 1.0f);
    store_f32(player + 172 + 23984, 0.0f);
    store_f32(player + 172 + 23984 * 2, -2.0f);
    store_f32(player + 172 + 23984 * 3, 3.0f);
    store_f32(player + 200, 8.0f);
    if (gameCheckPlayerAlive__Fv() != 2) return fail(5);

    unsigned nan_bits = 0x7fc00000u;
    float nan_value;
    std::memcpy(&nan_value, &nan_bits, 4);
    store_f32(player + 172, nan_value);
    store_f32(player + 172 + 23984 * 3, -0.0f);
    if (gameCheckPlayerAlive__Fv() != 0) return fail(6);
    return 0;
}
