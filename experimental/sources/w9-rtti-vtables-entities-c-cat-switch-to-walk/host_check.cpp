// Host check only. Not an EE build input.
#define NORRATH_RTTI_C_HOST 1
#include "switch_to_walk.cpp"
#include <cstdio>
#include <cstring>
static char log_text[16];
static unsigned log_n = 0;
static void* state_seen[5];
static void* header_seen[4];
static float floats[4][5];
static int flags_seen[4];
static unsigned char* object_under_test = nullptr;
static int hairy = 0;
static unsigned byte292_during[4];
static void note(char c) { log_text[log_n++] = c; log_text[log_n] = 0; }
extern "C" void animResetFramestates__FP14AnimationState(void* s) { note('z'); state_seen[4] = s; }
extern "C" void animAddHairy__FP14AnimationStateP15AnimationHeaderfffffi(void* s, void* h, float a, float b, float c, float d, float e, int f) {
    note('h'); state_seen[hairy] = s; header_seen[hairy] = h;
    floats[hairy][0] = a; floats[hairy][1] = b; floats[hairy][2] = c; floats[hairy][3] = d; floats[hairy][4] = e;
    flags_seen[hairy] = f; byte292_during[hairy] = object_under_test[292];
    std::memset(object_under_test + 296, 0xcc, 32);
    ++hairy;
}
int main() {
    int bad = 0;
    unsigned char object[512];
    unsigned char table[200];
    unsigned char headers[4][4];
    std::memset(table, 0, sizeof table);
    for (int i = 0; i < 4; ++i) rtti_c::host_note(headers[i]);
    rtti_c::host_note(table);
    const unsigned slots[4] = {116, 156, 164, 172};
    for (int i = 0; i < 4; ++i) rtti_c::write_word(rtti_c::address(table) + slots[i], rtti_c::address_bits(headers[i]));
    for (unsigned i = 0; i < sizeof object; ++i) object[i] = static_cast<unsigned char>(i * 5 + 7);
    rtti_c::write_word(rtti_c::address(object) + 36u, rtti_c::address_bits(table));
    object[292] = 0x77;
    unsigned char block[32], before[512];
    std::memcpy(block, object + 296, 32);
    std::memcpy(before, object, sizeof object);
    object_under_test = object;
    hairy = 0;
    switchToWalk__3Cat(object);
    if (std::strcmp(log_text, "zhhhh") != 0) { std::printf("order %s\n", log_text); ++bad; }
    for (int i = 0; i < 4; ++i) {
        if (state_seen[i] != object + 288 || header_seen[i] != headers[i] || flags_seen[i] != 513) { std::printf("call %d\n", i); ++bad; }
        if (byte292_during[i] != 0) { std::printf("byte 292 before call %d\n", i); ++bad; }
        const float want[2][5] = {{0, 1, 0, 1, 0}, {0, 0, 0, 1, 0}};
        for (int k = 0; k < 5; ++k) if (floats[i][k] != want[i == 0 ? 0 : 1][k]) { std::printf("float %d %d\n", i, k); ++bad; }
    }
    if (state_seen[4] != object + 288) { std::printf("reset state\n"); ++bad; }
    if (std::memcmp(object + 424, block, 8) != 0 || std::memcmp(object + 444, block + 20, 12) != 0) { std::printf("block copy\n"); ++bad; }
    if (std::memcmp(object + 424, block, 8) != 0) ++bad;
    if (rtti_c::read_word(rtti_c::address(object) + 428u) != rtti_c::read_word(rtti_c::address(block) + 4u)) { std::printf("block word 1\n"); ++bad; }
    if (rtti_c::read_word(rtti_c::address(object) + 432u) != 0x3f800000u || rtti_c::read_word(rtti_c::address(object) + 440u) != 0x3f800000u ||
        rtti_c::read_word(rtti_c::address(object) + 436u) != 0xbd088888u || object[292] != 5) { std::printf("tail fields\n"); ++bad; }
    for (unsigned i = 0; i < sizeof object; ++i) {
        const bool allowed = (i >= 296 && i < 328) || i == 292 || (i >= 424 && i < 456);
        if (!allowed && object[i] != before[i]) { std::printf("stray byte %u\n", i); ++bad; }
    }
    std::printf(bad ? "failed\n" : "switchToWalk__3Cat passed\n");
    return bad != 0;
}
