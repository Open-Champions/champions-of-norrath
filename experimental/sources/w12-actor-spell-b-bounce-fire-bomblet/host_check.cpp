// Host contract for bounce__11FireBomblet; not an EE input. Includes the source with pointer words
// kept as offsets from a base. Callees are stubs that log their arguments.
#define W12_BOUNCE_HOST
#include "bounce.cpp"
#include <cstdio>
#include <cstring>
#include <string>
#include <cmath>

unsigned char* w12_bounce_host_base;
int frameNm;
signed char cutMode;
unsigned syncRandState;
static std::string log_;
static float lastFloorZ;
static short lastFloorA;
static Point3 lastFire;
static int lastFireLife;
static bool allocFails;
static unsigned char block[1200];
void* lumpFindResource__FPcT0(const char* a, const char* b) { log_ += std::string("lump:") + a + "," + b + ";"; return nullptr; }
int soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void*, void*, float v) { log_ += "snd" + std::to_string(v) + ";"; return 0; }
void floorAdd__FG6Point3sfii(Point3 w, short a, float b, int c, int d) {
    log_ += "floor:" + std::to_string(b) + "," + std::to_string(c) + "," + std::to_string(d) + ";"; lastFloorZ = w.z; lastFloorA = a;
}
void* blockAlloc__Fi(int n) { log_ += "alloc" + std::to_string(n) + ";"; return allocFails ? nullptr : block; }
void StaticFire_construct(void*, Point3 w, float a, int b, float c) {
    log_ += "fire:" + std::to_string(a) + "," + std::to_string(c) + ";"; lastFire = w; lastFireLife = b;
}

static unsigned step(unsigned s) { return s * 0x19660du + 0x3c6ef35fu; }
alignas(16) static unsigned char obj[0x100];
static unsigned char counterStore[16];
static float& F(unsigned off) { return *reinterpret_cast<float*>(obj + off); }

static void reset(signed char count, signed char p, signed char q, float vz, unsigned counterValue) {
    std::memset(obj, 0, sizeof obj);
    w12_bounce_host_base = counterStore - 8;
    obj[154] = static_cast<unsigned char>(count); obj[155] = p; obj[157] = q;
    F(76) = 4.0f; F(80) = -8.0f; F(84) = vz; F(16) = 10.0f; F(20) = 20.0f; F(160) = 5.0f;
    counterStore[0] = static_cast<unsigned char>(counterValue);
    *reinterpret_cast<unsigned*>(obj + 172) = 8;
    log_.clear(); allocFails = false; cutMode = 0; frameNm = 37;
}

int main() {
    int bad = 0;
    auto expect = [&](bool ok, const char* what) { if (!ok) { std::printf("fail: %s\n", what); bad = 1; } };
    // First bounce with a small P: sound plays, count becomes 1, velocity is halved, z flips, no floor mark.
    reset(0, 0, 0, -3.0f, 0);
    bounce__11FireBomblet(obj);
    expect(log_ == "lump:firebomb.lmp,catapult_fireball_hit.anm;snd1.000000;", "first bounce sound only");
    expect(obj[154] == 1 && F(76) == 2.0f && F(80) == -4.0f && F(84) == 1.5f, "first bounce state");
    // P set scales by 0.33 (0x3ea8f5c2); vz non-negative stays.
    reset(2, 1, 0, 2.0f, 0);
    bounce__11FireBomblet(obj);
    expect(log_.empty() && obj[154] == 3, "no sound after the first bounce");
    expect(F(76) == 4.0f * bits(0x3ea8f5c2u) && F(84) == 2.0f * bits(0x3ea8f5c2u), "P scale");
    // Second bounce, downward, counter below 3: rand decides the floor mark.
    for (int trial = 0; trial < 64; ++trial) {
        reset(1, 0, 0, -1.0f, 2);
        syncRandState = 0x1000u * trial + 77;
        const unsigned s1 = step(syncRandState), s2 = step(s1);
        const bool mark = ((s1 >> 16) & 3) == 0;
        bounce__11FireBomblet(obj);
        expect(syncRandState == (mark ? s2 : s1), "rand steps taken");
        expect(mark == (log_ == "floor:24.000000,9,0;"), "floor mark exactly when bits 16..17 are zero");
        if (mark) {
            expect(counterStore[0] == 3 && lastFloorZ == 6.0f && lastFloorA == static_cast<short>(static_cast<int>(s2) >> 16), "floor mark data");
        } else {
            expect(counterStore[0] == 2, "counter untouched");
        }
    }
    // Counter at 3 blocks the mark; count 0 -> becomes 1 and blocks it too.
    reset(1, 0, 0, -1.0f, 3); syncRandState = 12345;
    bounce__11FireBomblet(obj);
    expect(syncRandState == 12345 && log_.empty(), "counter at 3");
    // Extra fire: Q set, frame multiple of 37, four draws then allocation then one more draw.
    reset(1, 0, 1, 1.0f, 0); syncRandState = 999;
    bounce__11FireBomblet(obj);
    unsigned s = 999; float u[4];
    for (int i = 0; i < 4; ++i) { s = step(s); u[i] = static_cast<float>(s >> 16) * 1.52587890625e-05f; }
    s = step(s);
    expect(syncRandState == s, "five draws for the fire");
    expect(log_ == "alloc1192;fire:240.000000,0.500000;", "fire call order");
    expect(lastFire.x == 10.0f + (u[0] - 0.5f) * (u[1] * 40.0f + 10.0f), "fire x");
    expect(lastFire.y == 20.0f + (u[2] - 0.5f) * (u[3] * 40.0f + 10.0f), "fire y");
    expect(lastFire.z == 5.0f && lastFireLife == static_cast<int>((s >> 16) % 10u) * 60 + 240, "fire z and lifetime");
    // Frame not a multiple of 37 and no cut mode: nothing happens (no draws). Cut mode forces it.
    reset(1, 0, 1, 1.0f, 0); frameNm = 38; syncRandState = 5;
    bounce__11FireBomblet(obj);
    expect(syncRandState == 5 && log_.empty(), "frame gate");
    reset(1, 0, 1, 1.0f, 0); frameNm = 38; cutMode = 1; syncRandState = 5;
    bounce__11FireBomblet(obj);
    expect(log_.find("fire:") != std::string::npos, "cut mode bypasses the frame gate");
    // Failed allocation: four draws, no fifth.
    reset(1, 0, 1, 1.0f, 0); allocFails = true; syncRandState = 5;
    bounce__11FireBomblet(obj);
    s = 5; for (int i = 0; i < 4; ++i) s = step(s);
    expect(syncRandState == s && log_ == "alloc1192;", "allocation failure");
    if (!bad) std::puts("bounce ok");
    return bad;
}
