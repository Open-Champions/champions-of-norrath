#include <cmath>
#include <cstring>

extern "C" void worldMoveDoor__FP12_worldHeaderiG6Point3s(void* world, int door_index, const float* point, short angle);

alignas(8) static unsigned char arena[4096];
unsigned char* norrath_world_arena = arena;
static float g_sin, g_cos;
static int g_angle_sin, g_angle_cos;
extern "C" float isin__Fi(int angle) { g_angle_sin = angle; return g_sin; }
extern "C" float icos__Fi(int angle) { g_angle_cos = angle; return g_cos; }

static unsigned& W(int off) { return *reinterpret_cast<unsigned*>(arena + off); }
static int& I(int off) { return *reinterpret_cast<int*>(arena + off); }
static short& H(int off) { return *reinterpret_cast<short*>(arena + off); }

static void setup(int cx, int cy) {
    std::memset(arena, 0, sizeof arena);
    W(84) = 256; W(36) = 1024;
    // door 0 at 256 (unused), door 1 at 284.
    const int d = 256 + 28;
    W(d + 0) = 2; H(d + 4) = 2;
    I(d + 8) = 100; I(d + 12) = -50; I(d + 16) = 7;
    H(d + 20) = static_cast<short>(cx); H(d + 22) = static_cast<short>(cy);
    // records 2 and 3 at 1024 + 60*n; record 3 has the 0x40 flag.
    for (int b = 0; b < 4 * 60; ++b) arena[1024 + b] = 0x5c;
    W(1024 + 60 * 2 + 36) = 0;
    W(1024 + 60 * 3 + 36) = 0x40;
}

int main() {
    const float point[3] = {1.0f, 2.0f, 3.0f};
    const int r2 = 1024 + 120, r3 = 1024 + 180;

    setup(1000, 2000);
    g_sin = 0.5f; g_cos = 0.25f;
    worldMoveDoor__FP12_worldHeaderiG6Point3s(arena, 1, point, static_cast<short>(0x12345678));
    if (g_angle_sin != 0x5678 || g_angle_cos != 0x5678) return 1;  // 16-bit angle, sign extended
    if (I(r2 + 40) != 5 || I(r2 + 44) != -62 || I(r2 + 48) != 19) return 2;
    if (H(r2 + 54) != -750 || H(r2 + 56) != 1000) return 3;
    if (I(r3 + 40) != 47 || I(r3 + 44) != 41 || I(r3 + 48) != 19) return 4;
    if (H(r3 + 54) != -750 || H(r3 + 56) != 1000) return 5;
    // Bytes outside the written fields stay put.
    if (arena[r2 + 35] != 0x5c || arena[r2 + 52] != 0x5c || arena[r2 + 58] != 0x5c) return 6;
    if (arena[1024 + 60 * 4] != 0) return 7;

    // Negative door index touches nothing, not even the trig callbacks.
    setup(1000, 2000);
    g_angle_sin = g_angle_cos = 99;
    worldMoveDoor__FP12_worldHeaderiG6Point3s(arena, -1, point, 5);
    if (g_angle_sin != 99 || I(r2 + 40) != 0x5c5c5c5c) return 8;

    // Clamping of the rotated centre.
    setup(32000, 32000);
    g_sin = 1.0f; g_cos = 1.0f;
    worldMoveDoor__FP12_worldHeaderiG6Point3s(arena, 1, point, -3);
    if (g_angle_sin != -3) return 9;
    if (H(r2 + 54) != 0 || H(r2 + 56) != 32767) return 10;
    setup(32000, 32000);
    g_sin = 2.0f; g_cos = 0.0f;
    worldMoveDoor__FP12_worldHeaderiG6Point3s(arena, 1, point, 0);
    if (H(r2 + 54) != -32768 || H(r2 + 56) != 32767) return 11;
    // Count of zero writes nothing.
    setup(10, 10);
    H(256 + 28 + 4) = 0;
    worldMoveDoor__FP12_worldHeaderiG6Point3s(arena, 1, point, 0);
    if (I(r2 + 40) != 0x5c5c5c5c) return 12;
    return 0;
}
