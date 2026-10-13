// Host check only; not a build input. Native build: c++ -std=c++17 -ffp-contract=off host_check.cpp
// The drop array pointer is a 32-bit word mapped onto an arena (base 0x100000). The collision and
// construction calls are scripted stubs; the expected drop values are recomputed here with plain float
// arithmetic in the order given in the notes.
#define NORRATH_HOST_ARENA
#include <cstring>
#include <string>
static unsigned char arena[1 << 17];
static const unsigned arena_base = 0x100000u;
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + (address - arena_base); }
extern "C" unsigned char _vt$13WeatherFilter[24] = {0};
#include "process_weather.cpp"

static const unsigned SELF = 0x100, DROPS = 0x2000, CAM = 0x4000, SCENE = 0x5000;
static std::string trace;
static int ray_script[16], ray_n, env_script[16], env_n;
static float hit1_script[16][3], hit2_script[16][3];
static int hit1_n, hit2_n;
static unsigned rand_script[16];
static int rand_n;
static unsigned filter_vtable_word;
static std::string S(unsigned v) { return std::to_string(v); }
static std::string V(const unsigned char* p) {
    float f[3]; std::memcpy(f, p, 12);
    char b[96]; std::snprintf(b, sizeof b, "(%.9g,%.9g,%.9g)", f[0], f[1], f[2]); return b;
}

extern "C" {
void ProcessLavastorm__12VIAtmosphereR7VISceneR8VICamera(unsigned char* s, unsigned char* sc, unsigned char* c) {
    trace += "lava:" + S(static_cast<unsigned>(s - arena)) + "," + S(static_cast<unsigned>(sc - arena)) + "," + S(static_cast<unsigned>(c - arena)) + ";";
}
void AddSplash__12VIAtmosphereR7VIVect3(unsigned char*, unsigned char* p) { trace += "splash:" + V(p) + ";"; }
void __9VICollRay(unsigned char*) { trace += "ctor_ray;"; }
void __13VISceneFilter(unsigned char*) { trace += "ctor_filter;"; }
void __8VIEnvRay(unsigned char*) { trace += "ctor_env;"; }
void Init__9VICollRayRC7VIVect3T1(unsigned char*, unsigned char* a, unsigned char* b) { trace += "ray:" + V(a) + V(b) + ";"; }
void Init__8VIEnvRayRC7VIVect3T1Ui(unsigned char*, unsigned char* a, unsigned char* b, unsigned flags) { trace += "env:" + V(a) + V(b) + S(flags) + ";"; }
int Collide__7VISceneR9VICollRayR13VISceneFilter(unsigned char*, unsigned char*, unsigned char* filter) {
    std::memcpy(&filter_vtable_word, filter + 16, 4);
    trace += "cray;";
    return ray_script[ray_n++];
}
int Collide__7VISceneR8VIEnvRay(unsigned char*, unsigned char*) { trace += "cenv;"; return env_script[env_n++]; }
void GetHitPoint__9VICollRay(unsigned char* out, unsigned char*) { std::memcpy(out, hit1_script[hit1_n++], 12); }
void GetHitPoint__C8VIEnvRay(unsigned char* out, unsigned char*) { std::memcpy(out, hit2_script[hit2_n++], 12); }
int rand(void) { trace += "rand;"; return static_cast<int>(rand_script[rand_n++]); }
}

static void sw(unsigned off, unsigned v) { std::memcpy(arena + off, &v, 4); }
static unsigned rw(unsigned off) { unsigned v; std::memcpy(&v, arena + off, 4); return v; }
static void sf(unsigned off, float f) { std::memcpy(arena + off, &f, 4); }
static float rf(unsigned off) { float f; std::memcpy(&f, arena + off, 4); return f; }
static float wind_row(int w, int c) { return ((c == 0 ? 1.0f : c == 1 ? 0.5f : 0.25f) * (w + 1)) * 0.125f; }
static float table(int k) { return 0.125f * (k + 1); }
static const float K1[2][3] = {{1.0f, 2.0f, 4.0f}, {0.5f, 1.0f, 2.0f}};
static const float K2[2][3] = {{0.25f, 0.5f, 1.0f}, {0.0f, 0.0f, 0.125f}};

// Defaults: cell R = 64, two drops, intensity 1.0 (count1 = 2), rate 0.5 (count2 = 1), dt = 1/64
// (step60 = 0.9375), phase 10.0, camera at the last origin with a zero direction (so the delta is 0).
static void arm(float intensity, float lava, int n, float dt, float phase, float rate) {
    std::memset(arena, 0, sizeof arena);
    trace.clear(); ray_n = env_n = hit1_n = hit2_n = rand_n = 0;
    for (int i = 0; i < 16; ++i) { ray_script[i] = 0; env_script[i] = 0; rand_script[i] = 0; for (int c = 0; c < 3; ++c) hit1_script[i][c] = hit2_script[i][c] = 0.0f; }
    sf(SELF + 4, 64.0f); sw(SELF + 8, n); sw(SELF + 12, arena_base + DROPS);
    sf(SELF + 60, 1.0f); sf(SELF + 64, 2.0f); sf(SELF + 68, 4.0f);
    sf(SELF + 72, intensity); sf(SELF + 76, lava);
    for (int q = 0; q < 2; ++q) for (int c = 0; c < 3; ++c) { sf(SELF + 80 + 12 * q + 4 * c, K1[q][c]); sf(SELF + 104 + 12 * q + 4 * c, K2[q][c]); }
    sf(SELF + 128, rate); sf(SELF + 132, phase);
    for (int w = 0; w < 64; ++w) for (int c = 0; c < 3; ++c) sf(SELF + 168 + 12 * w + 4 * c, wind_row(w, c));
    for (int k = 0; k < 64; ++k) sf(SELF + 936 + 4 * k, table(k));
    sf(SELF + 1192, dt);
    // Camera: direction (0,0,0), position equal to the origin.
    sf(CAM + 28, 1.0f); sf(CAM + 32, 2.0f); sf(CAM + 36, 4.0f);
    for (int i = 0; i < 4; ++i) { sf(DROPS + 36 * i + 28, -1000.0f); }
}
static void go() { ProcessWeather__12VIAtmosphereR7VISceneR8VICamera(arena + SELF, arena + SCENE, arena + CAM); }
static const std::string CTORS = "ctor_ray;ctor_filter;ctor_env;";

static float bf(unsigned u) { float f; std::memcpy(&f, &u, 4); return f; }
static const float STEP60 = 0.015625f * 60.0f;  // 0.9375, exact
// Wind displacement of drop i (row index w, q = 1 for the first count2 drops).
static void wind_step(int i, int q, float& x, float& y, float& z) {
    const int w = (10 + 29 * i + i) & 63;
    x = x + ((wind_row(w, 0) * K1[q][0]) + K2[q][0]) * STEP60;
    y = y + ((wind_row(w, 1) * K1[q][1]) + (K2[q][1] * table(i & 63))) * STEP60;
    z = z + ((wind_row(w, 2) * K1[q][2]) + K2[q][2]) * STEP60;
}
static std::string vs(float a, float b, float c) { char t[96]; std::snprintf(t, sizeof t, "(%.9g,%.9g,%.9g)", a, b, c); return t; }
static void put_drop(int i, float x, float y, float z) { sf(DROPS + 36 * i + 0, x); sf(DROPS + 36 * i + 4, y); sf(DROPS + 36 * i + 8, z); }
static bool drop_is(int i, float x, float y, float z) { return rf(DROPS + 36 * i) == x && rf(DROPS + 36 * i + 4) == y && rf(DROPS + 36 * i + 8) == z; }

int main() {
    // 1. Zero intensity: only the lava storm call (when its intensity is positive) and nothing else.
    arm(0.0f, 0.5f, 2, 0.015625f, 10.0f, 0.5f); go();
    if (trace != "lava:" + S(SELF) + "," + S(SCENE) + "," + S(CAM) + ";") return 1;
    arm(0.0f, 0.0f, 2, 0.015625f, 10.0f, 0.5f); go();
    if (!trace.empty() || rf(SELF + 132) != 10.0f) return 2;
    // 2. Steady rain: no delta, drops inside the cell, no ground hit. Positions advance by the wind rows;
    //    count2 = (int)(2 * 0.5) = 1, so drop 0 uses row q = 1 and drop 1 uses q = 0.
    arm(1.0f, 0.0f, 2, 0.015625f, 10.0f, 0.5f);
    put_drop(0, 3.0f, 5.0f, -7.0f); put_drop(1, -2.0f, 1.0f, 9.0f);
    go();
    if (trace != CTORS) return 3;
    {
        float x0 = 3.0f, y0 = 5.0f, z0 = -7.0f, x1 = -2.0f, y1 = 1.0f, z1 = 9.0f;
        wind_step(0, 1, x0, y0, z0); wind_step(1, 0, x1, y1, z1);
        if (!drop_is(0, x0, y0, z0) || !drop_is(1, x1, y1, z1)) return 4;
        if (x0 != 3.64453125f) return 5;  // w = 10: wind.x 1.375 * K1[1].x 0.5 = 0.6875, * 0.9375 = 0.64453125, + 3
        if (rf(SELF + 132) != 10.0f + 0.015625f * 10.0f) return 6;
        if (rf(SELF + 60) != 1.0f || rf(SELF + 64) != 2.0f || rf(SELF + 68) != 4.0f) return 7;
    }
    // 3. Camera moved a little: the delta (new origin - old origin) is subtracted from every drop, y first,
    //    and the origin becomes position + 5 * direction. count2 = 2, so q = 1.
    arm(1.0f, 0.0f, 1, 0.015625f, 10.0f, 2.0f);
    sf(CAM + 8, 0.5f); sf(CAM + 12, 0.25f); sf(CAM + 16, 0.125f);
    put_drop(0, 0.0f, 0.0f, 0.0f);
    go();
    {
        const float nx = 0.5f * 5.0f + 1.0f, ny = 0.25f * 5.0f + 2.0f, nz = 0.125f * 5.0f + 4.0f;
        if (rf(SELF + 60) != nx || rf(SELF + 64) != ny || rf(SELF + 68) != nz) return 8;
        float x = 0.0f, y = 0.0f, z = 0.0f;
        wind_step(0, 1, x, y, z);
        if (!drop_is(0, x - (nx - 1.0f), y - (ny - 2.0f), z - (nz - 4.0f))) return 9;
    }
    // 4. Teleport: a delta above the cell reseeds every drop with rays from 5000 above to 5000 below,
    //    each offset by the new origin, and the delta is dropped. Drop 1 hits the ground at y = -20.
    arm(1.0f, 0.0f, 2, 0.015625f, 10.0f, 0.0f);
    sf(CAM + 28, 1.0f + 100.0f);
    put_drop(0, 2.0f, 3.0f, 4.0f); put_drop(1, 6.0f, 7.0f, 8.0f);
    ray_script[0] = 0; ray_script[1] = 1; env_script[0] = 0;
    hit1_script[0][0] = 11.0f; hit1_script[0][1] = -20.0f; hit1_script[0][2] = 13.0f;
    rand_script[0] = 128;
    go();
    {
        const std::string head = CTORS +
            "ray:" + vs(2.0f + 101.0f, 3.0f + 5000.0f + 2.0f, 4.0f + 4.0f) + vs(2.0f + 101.0f, 3.0f - 5000.0f + 2.0f, 4.0f + 4.0f) + ";" +
            "env:" + vs(2.0f + 101.0f, 3.0f + 5000.0f + 2.0f, 4.0f + 4.0f) + vs(2.0f + 101.0f, 3.0f - 5000.0f + 2.0f, 4.0f + 4.0f) + "15;cray;" +
            "ray:" + vs(6.0f + 101.0f, 7.0f + 5000.0f + 2.0f, 8.0f + 4.0f) + vs(6.0f + 101.0f, 7.0f - 5000.0f + 2.0f, 8.0f + 4.0f) + ";" +
            "env:" + vs(6.0f + 101.0f, 7.0f + 5000.0f + 2.0f, 8.0f + 4.0f) + vs(6.0f + 101.0f, 7.0f - 5000.0f + 2.0f, 8.0f + 4.0f) + "15;cray;cenv;rand;";
        if (trace != head) return 10;
        if (rf(SELF + 60) != 101.0f) return 11;
        // Drop 1: ground -20, relative height -22, then mixed with the random byte (128 of 255).
        const float half = 64.0f * 0.5f;
        const float t = (-20.0f - 2.0f) - half;
        const float u = static_cast<float>((128 * 131) & 0xff) * bf(0x3b808080);
        float x1 = 6.0f, y1 = half + t * u, z1 = 8.0f;
        float x0 = 2.0f, y0 = 3.0f, z0 = 4.0f;
        wind_step(0, 0, x0, y0, z0); wind_step(1, 0, x1, y1, z1);
        if (!drop_is(0, x0, y0, z0) || !drop_is(1, x1, y1, z1)) return 12;
        if (rf(DROPS + 36 + 28) != -20.0f || rw(DROPS + 36 + 32) != 0) return 13;
        if (filter_vtable_word != static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(_vt$13WeatherFilter))) return 14;
    }
    // 5. An x wrap (not a y wrap) re-casts the drop. x = 31.5 + 0.64453125 > 32 wraps by -64.
    arm(1.0f, 0.0f, 1, 0.015625f, 10.0f, 2.0f);
    put_drop(0, 31.5f, 0.0f, 0.0f);
    ray_script[0] = 1; env_script[0] = 1;
    hit1_script[0][0] = 1.0f; hit1_script[0][1] = -50.0f; hit1_script[0][2] = 3.0f;
    hit2_script[0][0] = 7.0f; hit2_script[0][1] = -10.0f; hit2_script[0][2] = 9.0f;
    rand_script[0] = 0;  // fraction 0: the mixed height is the half-cell 32 (exactly)
    go();
    {
        float x = 31.5f, y = 0.0f, z = 0.0f;
        wind_step(0, 1, x, y, z);
        if (!(x >= 32.0f)) return 28;
        const float wx = x - 64.0f;
        // The env hit is higher (-10 > -50): ground = -10 and the flag word becomes 1.
        if (rw(DROPS + 32) != 1 || rf(DROPS + 28) != -10.0f) return 15;
        if (trace.find("cray;cenv;rand;") == std::string::npos) return 16;
        if (rf(DROPS + 0) != wx) return 17;
        // y was set to the mixed height: half + (h - half) * 0 = 32, then ground check: 32 + 2.0 >= -10, no splash.
        if (rf(DROPS + 4) != 32.0f) return 18;
        if (trace.find("splash") != std::string::npos) return 19;
    }
    // 6. y wrap alone: no ray cast. y = 31.5 + wind -> above 32 -> minus 64.
    arm(1.0f, 0.0f, 1, 0.015625f, 10.0f, 2.0f);
    put_drop(0, 0.0f, 31.5f, 0.0f);
    go();
    {
        float x = 0.0f, y = 31.5f, z = 0.0f;
        wind_step(0, 1, x, y, z);
        if (!(y >= 32.0f) || !drop_is(0, x, y - 64.0f, z) || trace != CTORS) return 20;
    }
    // 7. Ground hit: drop 1 (q = 0, index < count1 = 2) spawns a splash at (x + O.x, ground + 0.08, z + O.z);
    //    drop 1's word is 1, so x and z are re-randomized from two rands; y = max(R/2, ground - O.y).
    arm(1.0f, 0.0f, 2, 0.015625f, 10.0f, 0.5f);
    put_drop(0, 0.0f, 0.0f, 0.0f); put_drop(1, 4.0f, -10.0f, 6.0f);
    sf(DROPS + 36 + 28, 0.0f); sw(DROPS + 36 + 32, 1);
    rand_script[0] = 255; rand_script[1] = 3;
    go();
    {
        float x1 = 4.0f, y1 = -10.0f, z1 = 6.0f;
        wind_step(1, 0, x1, y1, z1);
        if (!(y1 + 2.0f < 0.0f)) return 21;
        const std::string splash = "splash:" + vs(x1 + 1.0f, 0.0f + bf(0x3da3d70a), z1 + 4.0f) + ";";
        if (trace != CTORS + splash + "rand;rand;") return 22;
        const float rx = (static_cast<float>((255 * 131) & 0xff) * bf(0x3b808080)) * 64.0f - 64.0f * 0.5f;
        const float rz = (static_cast<float>((3 * 131) & 0xff) * bf(0x3b808080)) * 64.0f - 64.0f * 0.5f;
        if (rf(DROPS + 36) != rx || rf(DROPS + 44) != rz) return 23;
        // y = max(32, ground - O.y) = max(32, -2) = 32.
        if (rf(DROPS + 40) != 32.0f) return 24;
    }
    // 8. Same ground hit for a q = 1 drop (index 0 < count2): no splash, y re-set. Ground 100 gives y = 98.
    arm(1.0f, 0.0f, 1, 0.015625f, 10.0f, 2.0f);
    put_drop(0, 0.0f, 0.0f, 0.0f); sf(DROPS + 28, 100.0f);
    go();
    if (trace != CTORS || rf(DROPS + 4) != 98.0f) return 25;
    // 9. A splash needs i < count1: intensity 0.5 over two drops gives count1 = 1, so drop 1 does not splash.
    arm(0.5f, 0.0f, 2, 0.015625f, 10.0f, 0.0f);
    put_drop(1, 0.0f, -10.0f, 0.0f); sf(DROPS + 36 + 28, 0.0f);
    go();
    if (trace.find("splash") != std::string::npos) return 26;
    arm(0.5f, 0.0f, 2, 0.015625f, 10.0f, 0.0f);
    put_drop(0, 0.0f, -10.0f, 0.0f); sf(DROPS + 28, 0.0f);
    go();
    if (trace.find("splash") == std::string::npos) return 27;  // with count2 = 0 drop 0 has q = 0 and 0 < count1
    // 10. Wrap boundaries (all values exact): a coordinate landing exactly on +R/2 wraps, exactly on -R/2 does not.
    //     Drop 0 has q = 1 (step x 0.64453125, y 0.64453125, z 0.76171875).
    arm(1.0f, 0.0f, 1, 0.015625f, 10.0f, 2.0f);
    put_drop(0, 0.0f, 32.0f - 0.64453125f, 0.0f); go();
    if (!drop_is(0, 0.64453125f, -32.0f, 0.76171875f) || trace != CTORS) return 29;
    arm(1.0f, 0.0f, 1, 0.015625f, 10.0f, 2.0f);
    put_drop(0, 0.0f, -32.0f - 0.64453125f, 0.0f); go();
    if (!drop_is(0, 0.64453125f, -32.0f, 0.76171875f) || trace != CTORS) return 30;
    arm(1.0f, 0.0f, 1, 0.015625f, 10.0f, 2.0f);
    put_drop(0, 32.0f - 0.64453125f, 0.0f, 0.0f); go();
    if (rf(DROPS + 0) != -32.0f || trace.find("cray;") == std::string::npos) return 31;
    arm(1.0f, 0.0f, 1, 0.015625f, 10.0f, 2.0f);
    put_drop(0, -32.0f - 0.64453125f, 0.0f, 0.0f); go();
    if (rf(DROPS + 0) != -32.0f || trace != CTORS) return 32;
    arm(1.0f, 0.0f, 1, 0.015625f, 10.0f, 2.0f);
    put_drop(0, 0.0f, 0.0f, 32.0f - 0.76171875f); go();
    if (rf(DROPS + 8) != -32.0f || trace.find("cray;") == std::string::npos) return 33;
    arm(1.0f, 0.0f, 1, 0.015625f, 10.0f, 2.0f);
    put_drop(0, 0.0f, 0.0f, -32.0f - 0.76171875f); go();
    if (rf(DROPS + 8) != -32.0f || trace != CTORS) return 34;
    // 11. Teleport threshold (the test is one-sided): a y or z delta equal to the cell does not reseed (the
    //     drop only shifts, and a y wrap casts no ray); a larger one does; a large negative one does not.
    //     An x delta equal to the cell shifts the drop, which then wraps and casts from the wrapped position.
    arm(1.0f, 0.0f, 1, 0.015625f, 10.0f, 2.0f);
    sf(CAM + 32, 2.0f + 64.0f); put_drop(0, 0.0f, 0.0f, 0.0f); go();
    if (trace != CTORS) return 35;
    arm(1.0f, 0.0f, 1, 0.015625f, 10.0f, 2.0f);
    sf(CAM + 36, 4.0f + 64.0f); put_drop(0, 0.0f, 0.0f, 0.0f); go();
    if (trace.find("ray:" + vs(0.64453125f + 1.0f, 0.0f, 0.0f).substr(0, 8)) == std::string::npos) return 43;  // shifted, then wrapped, then cast
    arm(1.0f, 0.0f, 1, 0.015625f, 10.0f, 2.0f);
    sf(CAM + 28, 1.0f + 64.0f); put_drop(0, 0.0f, 0.0f, 0.0f); go();
    if (trace.find("ray:" + vs(0.64453125f + 65.0f, 0.0f, 0.0f).substr(0, 9)) == std::string::npos) return 44;
    arm(1.0f, 0.0f, 1, 0.015625f, 10.0f, 2.0f);
    sf(CAM + 32, 2.0f + 65.0f); put_drop(0, 0.0f, 0.0f, 0.0f); go();
    if (trace.find("ray:") == std::string::npos || !drop_is(0, 0.64453125f, 0.64453125f, 0.76171875f)) return 36;
    arm(1.0f, 0.0f, 1, 0.015625f, 10.0f, 2.0f);
    sf(CAM + 36, 4.0f + 65.0f); put_drop(0, 0.0f, 0.0f, 0.0f); go();
    if (trace.find("ray:") == std::string::npos || !drop_is(0, 0.64453125f, 0.64453125f, 0.76171875f)) return 37;
    arm(1.0f, 0.0f, 1, 0.015625f, 10.0f, 2.0f);
    sf(CAM + 32, 2.0f - 500.0f); put_drop(0, 0.0f, 0.0f, 0.0f); go();
    if (trace != CTORS) return 38;
    // 12. The phase wraps only when above 64: 63.84375 + 0.15625 = 64 stays, 63.9 wraps.
    arm(1.0f, 0.0f, 0, 0.015625f, 63.84375f, 2.0f); go();
    if (rf(SELF + 132) != 64.0f) return 39;
    arm(1.0f, 0.0f, 0, 0.015625f, 63.90625f, 2.0f); go();
    if (rf(SELF + 132) != (63.90625f + 0.15625f) - 64.0f) return 40;
    // 13. Hit points: an env hit lower than the ray hit does not replace it (ground stays the ray's).
    arm(1.0f, 0.0f, 1, 0.015625f, 10.0f, 2.0f);
    put_drop(0, 31.5f, 0.0f, 0.0f);
    ray_script[0] = 1; env_script[0] = 1;
    hit1_script[0][1] = -10.0f; hit2_script[0][1] = -50.0f;
    go();
    if (rf(DROPS + 28) != -10.0f || rw(DROPS + 32) != 0) return 41;
    // 14. The ray test with no ray hit leaves the drop's ground and flag words alone.
    arm(1.0f, 0.0f, 1, 0.015625f, 10.0f, 2.0f);
    put_drop(0, 31.5f, 0.0f, 0.0f); sf(DROPS + 28, -300.0f); sw(DROPS + 32, 5);
    ray_script[0] = 0;
    go();
    if (rf(DROPS + 28) != -300.0f || rw(DROPS + 32) != 5) return 42;
    // 15. Equal hit heights do not count as higher: the ray hit stays and the flag word stays 0.
    arm(1.0f, 0.0f, 1, 0.015625f, 10.0f, 2.0f);
    put_drop(0, 31.5f, 0.0f, 0.0f);
    ray_script[0] = 1; env_script[0] = 1;
    hit1_script[0][0] = 1.0f; hit1_script[0][1] = -10.0f; hit2_script[0][0] = 5.0f; hit2_script[0][1] = -10.0f;
    go();
    if (rf(DROPS + 28) != -10.0f || rw(DROPS + 32) != 0) return 45;
    // 16. The wind table of 64 floats is indexed by i & 63: drop 33 uses entry 33.
    arm(1.0f, 0.0f, 34, 0.015625f, 10.0f, 0.0f);
    for (int i = 0; i < 34; ++i) put_drop(i, 0.0f, 0.0f, 0.0f);
    go();
    {
        float x = 0.0f, y = 0.0f, z = 0.0f;
        wind_step(33, 0, x, y, z);
        if (!drop_is(33, x, y, z)) return 46;
        float x1 = 0.0f, y1 = 0.0f, z1 = 0.0f;
        wind_step(1, 0, x1, y1, z1);
        if (y == y1) return 47;  // the two table entries differ, so the check above is meaningful
    }
    // 17. A ray hit clears the flag word before the env test: previous value 7, env miss -> 0.
    arm(1.0f, 0.0f, 1, 0.015625f, 10.0f, 2.0f);
    put_drop(0, 31.5f, 0.0f, 0.0f); sw(DROPS + 32, 7);
    ray_script[0] = 1; env_script[0] = 0; hit1_script[0][1] = -10.0f;
    go();
    if (rw(DROPS + 32) != 0 || rf(DROPS + 28) != -10.0f) return 48;
    return 0;
}
