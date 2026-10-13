// Host check only; not a build input. Native build: c++ -std=c++17 -ffp-contract=off host_check.cpp
// All pointer words are 32-bit addresses mapped onto an arena (base 0x100000). rand, sinf and cosf are
// scripted stubs so the arithmetic of the shooting star can be recomputed here independently.
#define NORRATH_HOST_ARENA
#include <cstring>
#include <string>
static unsigned char arena[1 << 18];
static const unsigned arena_base = 0x100000u;
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + (address - arena_base); }
#include "render_stars.cpp"

// atmosphere 0x100 (4200 bytes), raster 0x2000, billboard batches at 0x4000 + 0x2000 * batch, scene 0x20000.
static const unsigned ATM = 0x100, RAS = 0x2000, BB = 0x4000, SCENE = 0x20000;
static std::string trace;
static int begin_n, begin_calls;
static unsigned rand_script[16];
static int rand_index;
static unsigned addr(const void* p) { return static_cast<unsigned>(static_cast<const unsigned char*>(p) - arena); }
static std::string S(unsigned v) { return std::to_string(v); }
static unsigned bits(float f) { unsigned u; std::memcpy(&u, &f, 4); return u; }

extern "C" {
void SetFog__8VIRasteriffRC10VIColor32F(unsigned char* r, int mode, float a, float b, const float* c) {
    trace += "fog:" + S(addr(r)) + "," + std::to_string(mode) + "," + S(bits(a)) + "," + S(bits(b)) + "," + S(bits(c[0]) | bits(c[1]) | bits(c[2]) | bits(c[3])) + ";";
}
void SetMaterial__8VIRasteri(unsigned char* r, int m) { trace += "mat:" + S(addr(r)) + "," + std::to_string(m) + ";"; }
int BeginBillboards__8VIRasterPP11VIBillboardi(unsigned char*, unsigned* cursor, int count) {
    trace += "begin:" + std::to_string(count) + ";";
    *cursor = arena_base + BB + 0x2000 * begin_calls++;
    return begin_n;
}
void EndBillboards__8VIRaster(unsigned char* r) { trace += "end;"; (void)r; }
int rand(void) { trace += "rand;"; return static_cast<int>(rand_script[rand_index++]); }
float sinf(float x) { trace += "sin;"; return x * 0.001f; }
float cosf(float x) { trace += "cos;"; return x * 0.002f + 1.0f; }
}

static void sw(unsigned off, unsigned v) { std::memcpy(arena + off, &v, 4); }
static unsigned rw(unsigned off) { unsigned v; std::memcpy(&v, arena + off, 4); return v; }
static void sf(unsigned off, float f) { std::memcpy(arena + off, &f, 4); }
static float rf(unsigned off) { float f; std::memcpy(&f, arena + off, 4); return f; }
static void go() { RenderStars__12VIAtmosphereR7VISceneR8VICamera(arena + ATM, arena + SCENE, nullptr); }
static float star_x(int i) { return 1.0f + i; }
static float star_w(int i) { return 0.5f + 0.01f * i; }
static signed char star_off(int i) { return static_cast<signed char>(i * 7 - 40); }
static float table(int i) { return 0.03125f * (i + 1); }
static void arm(float intensity, float timer, float progress, float dt, float b3912, int n, float phase) {
    std::memset(arena, 0x2e, sizeof arena);
    trace.clear(); begin_calls = 0; begin_n = n; rand_index = 0;
    sw(SCENE + 19356, arena_base + RAS);
    sf(ATM + 1564, intensity); sw(ATM + 1244, 3); sf(ATM + 1192, dt); sf(ATM + 3876, phase);
    sf(ATM + 3880, timer); sf(ATM + 3884, progress);
    sf(ATM + 3888, 10.0f); sf(ATM + 3892, 20.0f); sf(ATM + 3896, 30.0f);
    sf(ATM + 3900, 50.0f); sf(ATM + 3904, 80.0f); sf(ATM + 3908, 130.0f);
    sf(ATM + 3912, b3912); sw(ATM + 3920, 9);
    for (int i = 0; i < 128; ++i) {
        sf(ATM + 1568 + 16 * i, star_x(i)); sf(ATM + 1572 + 16 * i, -star_x(i)); sf(ATM + 1576 + 16 * i, 2.0f * i); sf(ATM + 1580 + 16 * i, star_w(i));
        arena[ATM + 3616 + i] = static_cast<unsigned char>(star_off(i));
    }
    for (int i = 0; i < 32; ++i) sf(ATM + 3744 + 4 * i, table(i));
    for (int i = 0; i < 14; ++i) { sf(ATM + 3936 + 16 * i, 100.0f + i); sf(ATM + 3940 + 16 * i, 200.0f + i); sf(ATM + 3944 + 16 * i, 300.0f + i); sf(ATM + 3948 + 16 * i, 0.25f * (i + 1)); }
}
static bool billboard_matches(unsigned b, float x, float y, float z, float w, float c) {
    return rf(b + 0) == x && rf(b + 4) == y && rf(b + 8) == z && rw(b + 12) == 0 && rf(b + 16) == w && rf(b + 20) == w &&
           rf(b + 32) == c && rf(b + 36) == c && rf(b + 40) == c && rf(b + 44) == 1.0f;
}
// The billboard i of the 128-star run: batches of n, each batch at BB + 0x2000 * batch.
static unsigned star_billboard(int i, int n) { return BB + 0x2000 * (i / n) + 48 * (i % n); }

static float bf(unsigned u) { float f; std::memcpy(&f, &u, 4); return f; }
static const std::string FOG = "fog:" + std::to_string(RAS) + ",0,0,0,0;";

int main() {
    const std::string stars_head = FOG + "mat:" + S(RAS) + ",3;";
    // Both parts off: nothing at all.
    arm(0.0f, 5.0f, 0.0f, 0.1f, 0.0f, 128, 3.0f); go();
    if (!trace.empty() || rf(ATM + 3880) != 5.0f) return 1;
    // Stars only, timer running: timer drops by dt, phase advances, 128 star billboards in batches of 50.
    arm(0.5f, 5.0f, 0.0f, 0.25f, 0.0f, 50, 3.75f); go();
    if (trace != stars_head + "begin:128;end;begin:78;end;begin:28;end;") return 2;
    if (rf(ATM + 3880) != 4.75f) return 3;
    if (rf(ATM + 3876) != 3.75f + 0.25f * 15.0f) return 4;
    for (int i = 0; i < 128; ++i) {
        const int k = 3;  // (signed char)(int)3.75
        const float c = table((k + star_off(i)) & 31) * 0.5f;
        if (!billboard_matches(star_billboard(i, 50), star_x(i), -star_x(i), 2.0f * i, star_w(i), c)) return 5;
    }
    if (arena[BB + 0x2000 * 2 + 48 * 28] != 0x2e) return 6;
    // Phase wrap: above 32 subtracts 32; a negative phase truncates toward zero.
    arm(0.5f, 5.0f, 0.0f, 1.0f, 0.0f, 200, 30.0f); go();
    if (rf(ATM + 3876) != (30.0f + 15.0f) - 32.0f || trace != stars_head + "begin:128;end;") return 7;
    arm(0.5f, 5.0f, 0.0f, 1.0f, 0.0f, 200, -2.5f); go();
    for (int i = 0; i < 128; ++i)
        if (rf(BB + 0x2000 * 0 + 48 * i + 32) != table((-2 + star_off(i)) & 31) * 0.5f) return 8;
    if (rf(ATM + 3876) != -2.5f + 15.0f) return 9;
    // Spawn: angle rands, then P = 1e-5 advances by dt * 0.2 in the same call; two jitter rands follow.
    rand_script[0] = 300; rand_script[1] = 41; rand_script[2] = 1000000; rand_script[3] = 300000;
    arm(0.5f, 0.0f, 0.0f, 0.5f, 0.0f, 200, 3.0f); go();
    if (trace != stars_head + "rand;sin;cos;rand;sin;cos;rand;rand;begin:1;end;begin:128;end;") return 10;
    const float a0 = static_cast<float>((300 * 131) & 0xff) * bf(0x40799999) * bf(0x3c49d9b2);
    const float a1 = static_cast<float>((41 * 131) & 0xff) * bf(0x40799999) * bf(0x3c49d9b2);
    const float sx = a0 * 0.001f * 100.0f, sz = (a0 * 0.002f + 1.0f) * 100.0f;
    const float ex = a1 * 0.001f * 100.0f, ez = (a1 * 0.002f + 1.0f) * 100.0f;
    if (rf(ATM + 3888) != sx || rf(ATM + 3892) != 100.0f || rf(ATM + 3896) != sz) return 11;
    if (rf(ATM + 3900) != ex || rf(ATM + 3904) != 0.0f || rf(ATM + 3908) != ez) return 12;
    const float p = bf(0x3727c5ac) + 0.5f * bf(0x3e4ccccc);
    if (rf(ATM + 3884) != p) return 13;
    float b = (static_cast<float>(1000000) - static_cast<float>(300000)) * bf(0x30000000) * bf(0x3e999999) + bf(0x3f333333);
    b = b * (p * 5.0f);
    float x = ex - sx; float y = 0.0f - 100.0f; float z = ez - sz;
    x = x * p; y = y * p; z = z * p;
    x = x + sx; y = y + 100.0f; z = z + sz;
    if (!billboard_matches(BB, x, y, z, 0.5f, b)) return 14;
    if (rf(ATM + 3880) != 0.0f) return 15;
    // Late in flight: the fade-out branch, clamped at zero when it goes negative; completion resets.
    rand_script[0] = 5; rand_script[1] = 9; rand_script[2] = 0x1237;
    arm(0.5f, 0.0f, 0.7f, 1.0f, 0.0f, 200, 3.0f); go();
    if (trace != stars_head + "rand;rand;begin:1;end;begin:128;end;") return 16;
    {
        const float pp = 0.7f + 1.0f * bf(0x3e4ccccc);
        float bb = (static_cast<float>(5) - static_cast<float>(9)) * bf(0x30000000) * bf(0x3e999999) + bf(0x3f333333);
        bb = bb * (1.0f - (pp - bf(0x3e4ccccc)) * 1.25f);
        if (bb < 0.0f) bb = 0.0f;
        if (bb > 1.0f) bb = 1.0f;
        float x2 = 50.0f - 10.0f, y2 = 80.0f - 20.0f, z2 = 130.0f - 30.0f;
        x2 = x2 * pp; y2 = y2 * pp; z2 = z2 * pp;
        x2 = x2 + 10.0f; y2 = y2 + 20.0f; z2 = z2 + 30.0f;
        if (rf(ATM + 3884) != pp || !billboard_matches(BB, x2, y2, z2, 0.5f, bb)) return 17;
    }
    arm(0.5f, 0.0f, 0.95f, 1.0f, 0.0f, 200, 3.0f); rand_script[0] = 5; rand_script[1] = 9; rand_script[2] = 0x1237; go();
    if (trace != stars_head + "rand;rand;begin:1;end;rand;begin:128;end;") return 18;
    if (rf(ATM + 3884) != 0.0f || rf(ATM + 3880) != static_cast<float>((0x1237 & 15) + 10)) return 19;
    // p = 1.15 makes the fade factor negative: the colour clamps to exactly 0.
    if (rf(BB + 32) != 0.0f || rf(BB + 36) != 0.0f || rf(BB + 40) != 0.0f || rf(BB + 44) != 1.0f) return 28;
    // Boundary: P exactly 0.2 (dt 0) takes the fade branch, whose factor is exactly 1.0.
    rand_script[0] = 1000000; rand_script[1] = 0;
    arm(0.5f, 0.0f, bf(0x3e4ccccc), 0.0f, 0.0f, 200, 3.0f); go();
    {
        const float bb = (static_cast<float>(1000000) - static_cast<float>(0)) * bf(0x30000000) * bf(0x3e999999) + bf(0x3f333333);
        if (rf(BB + 32) != bb * (1.0f - (bf(0x3e4ccccc) - bf(0x3e4ccccc)) * 1.25f)) return 29;
    }
    // Boundary: a phase of exactly 32.0 after the step is not wrapped.
    arm(0.5f, 5.0f, 0.0f, 1.0f, 0.0f, 200, 17.0f); go();
    if (rf(ATM + 3876) != 32.0f) return 30;
    // Brightness clamps to 1.0 and the colour is (b, b, b, 1.0): P just above 0.2 with a large jitter.
    rand_script[0] = 2000000000; rand_script[1] = 0;
    arm(0.5f, 0.0f, 0.3f, 0.0f, 0.0f, 200, 3.0f); go();
    {
        float bb = (static_cast<float>(2000000000) - static_cast<float>(0)) * bf(0x30000000) * bf(0x3e999999) + bf(0x3f333333);
        const float pp = 0.3f + 0.0f * bf(0x3e4ccccc);
        bb = bb * (1.0f - (pp - bf(0x3e4ccccc)) * 1.25f);
        if (bb > 1.0f) bb = 1.0f;
        if (rf(BB + 32) != bb || rf(BB + 44) != 1.0f) return 20;
    }
    // Timer running with zero progress: no rand, no shooting star.
    arm(0.5f, 0.001f, 0.0f, 0.5f, 0.0f, 200, 3.0f); go();
    if (trace.find("rand") != std::string::npos || trace.find("begin:1;") != std::string::npos) return 21;
    // Timer expired but progress is not zero: no spawn, only the advance.
    rand_script[0] = 7; rand_script[1] = 7;
    arm(0.5f, 0.0f, 0.4f, 0.5f, 0.0f, 200, 3.0f); go();
    if (trace != stars_head + "rand;rand;begin:1;end;begin:128;end;") return 22;
    // Second layer: intensity at +3912 only. Fourteen billboards, colour min(F * 0.45, 0.15).
    arm(0.0f, 0.0f, 0.0f, 0.1f, 0.8f, 5, 3.0f); go();
    if (trace != FOG + "mat:" + S(RAS) + ",9;begin:14;end;begin:9;end;begin:4;end;") return 23;
    for (int i = 0; i < 14; ++i) {
        const unsigned bb = BB + 0x2000 * (i / 5) + 48 * (i % 5);
        if (!billboard_matches(bb, 100.0f + i, 200.0f + i, 300.0f + i, 0.25f * (i + 1), bf(0x3e199999))) return 24;
    }
    arm(0.0f, 0.0f, 0.0f, 0.1f, 0.2f, 20, 3.0f); go();
    if (trace != FOG + "mat:" + S(RAS) + ",9;begin:14;end;") return 25;
    if (rf(BB + 32) != 0.2f * bf(0x3ee66666) || rf(BB + 48 * 13 + 40) != 0.2f * bf(0x3ee66666)) return 26;
    // Both layers: stars first, then the second layer with its own fog and material.
    arm(0.5f, 5.0f, 0.0f, 0.25f, 0.8f, 200, 3.0f); go();
    if (trace != stars_head + "begin:128;end;" + FOG + "mat:" + S(RAS) + ",9;begin:14;end;") return 27;
    return 0;
}
