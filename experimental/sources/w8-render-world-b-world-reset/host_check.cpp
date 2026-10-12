#include <cstring>

extern "C" {
float worldWaterWavyness, worldWaterBumpAmount, worldWaterBumpRadius, worldWaterWavyness2;
int worldWaterBumpTimer, worldWaterAlpha, worldWaterAlpha2;
unsigned worldWaterColor, worldWaterColor2;
alignas(8) unsigned char worldAnimation[64 * 48 + 8];
void worldReset__FP12_worldHeader(void* world);
}
alignas(8) static unsigned char arena[16384];
unsigned char* norrath_world_arena = arena;

static unsigned& W(int off) { return *reinterpret_cast<unsigned*>(arena + off); }
static unsigned short& H(int off) { return *reinterpret_cast<unsigned short*>(arena + off); }

int main() {
    std::memset(arena, 0, sizeof arena);
    std::memset(worldAnimation, 0x77, sizeof worldAnimation);
    // Header at arena+0.
    W(0) = 3; W(36) = 512; W(32) = 1536; W(48) = 2; W(52) = 3; W(56) = 128;
    W(80) = 2; W(84) = 1024;
    for (int i = 0; i < 6; ++i) W(128 + i * 4) = 0x12345600u + 0x10u * i + 0x0fu;
    W(128 + 6 * 4) = 0xdeadbeef;  // beyond rows*cols: untouched

    // Three 60-byte records at 512.
    for (int b = 0; b < 180; ++b) arena[512 + b] = 0x21;
    W(512) = 2048;          W(512 + 4) = 0x44444444;      W(512 + 36) = 0x80;
    W(512 + 60) = 2100;     W(512 + 60 + 4) = 0x55555555; W(512 + 60 + 36) = 0x880;
    W(512 + 120) = 2300;    W(512 + 120 + 4) = 0x66666666; W(512 + 120 + 36) = 0x700;
    W(2048 + 8) = 2200;     W(2200 + 4768) = 0x99999999;
    W(2100 + 8) = 0;
    W(2300 + 8) = 2400;     W(2400 + 4768) = 0x88888888;  // not reached: bit 0x80 clear

    // Two doors at 1024.
    H(1024 + 6) = 0xffff;   W(1024) = 1; H(1024 + 4) = 2;
    W(1024 + 8) = 0xa0; W(1024 + 12) = 0xb0; W(1024 + 16) = 0xc0;
    H(1024 + 20) = 0x1111; H(1024 + 22) = 0x2222; H(1024 + 24) = 1;
    H(1052 + 6) = 0x0001;   W(1052) = 0; H(1052 + 4) = 1;
    W(1052 + 8) = 0xd0; W(1052 + 12) = 0xe0; W(1052 + 16) = 0xf0;
    H(1052 + 20) = 0x3333; H(1052 + 22) = 0x4444; H(1052 + 24) = 0;
    H(1536 + 16) = 0xffff; H(1536 + 36 + 16) = 0xffff;

    worldWaterWavyness = worldWaterBumpAmount = worldWaterBumpRadius = 9.0f;
    worldWaterWavyness2 = 9.0f;
    worldWaterBumpTimer = worldWaterAlpha = worldWaterAlpha2 = 9;
    worldWaterColor = worldWaterColor2 = 9u;

    worldReset__FP12_worldHeader(arena);

    // Step 1.
    for (int i = 0; i < 6; ++i) if (W(128 + i * 4) != (0x0fu + 0x10u * i)) return 1;
    if (W(128 + 24) != 0xdeadbeef) return 2;
    // Step 2.
    if (W(2048 + 8) != 0 || W(2200 + 4768) != 0) return 3;
    if (W(512 + 4) != 0x44444444) return 4;           // record 0 lacks bit 0x800
    if (W(512 + 60 + 4) != 0) return 5;               // record 1 cleared
    if (W(2100 + 8) != 0) return 6;
    if (W(512 + 120 + 4) != 0x66666666) return 7;
    if (W(2300 + 8) != 2400 || W(2400 + 4768) != 0x88888888) return 8;
    // Step 3: door 0 writes records 1 and 2, door 1 writes record 0.
    if (H(1024 + 6) != 0xfffe) return 9;
    if (H(1052 + 6) != 0x0000) return 10;
    for (int r = 0; r < 3; ++r) {
        const int rec = 512 + r * 60;
        const bool first = r == 0;
        if (W(rec + 40) != (first ? 0xd0u : 0xa0u)) return 11;
        if (W(rec + 44) != (first ? 0xe0u : 0xb0u)) return 12;
        if (W(rec + 48) != (first ? 0xf0u : 0xc0u)) return 13;
        if (H(rec + 54) != (first ? 0x3333 : 0x1111)) return 14;
        if (H(rec + 56) != (first ? 0x4444 : 0x2222)) return 15;
    }
    if (H(1536 + 16) != 0xfffd || H(1536 + 36 + 16) != 0xfffd) return 16;
    // Step 4.
    for (int i = 0; i < 64; ++i)
        for (int b = 0; b < 48; ++b) {
            const bool cleared = b == 4 || (b >= 40 && b < 44);
            if (worldAnimation[i * 48 + b] != (cleared ? 0 : 0x77)) return 17;
        }
    if (worldAnimation[64 * 48] != 0x77) return 18;
    // Step 5.
    if (worldWaterWavyness != 0.0234375f || worldWaterBumpAmount != 41.0f || worldWaterBumpRadius != 4.0f) return 19;
    if (worldWaterBumpTimer != 6 || worldWaterAlpha != 98 || worldWaterColor != 0x00808080u) return 20;
    if (worldWaterWavyness2 != 0.0f || worldWaterAlpha2 != 0 || worldWaterColor2 != 0u) return 21;
    return 0;
}
