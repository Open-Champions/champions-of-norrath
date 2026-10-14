// Host check only; not a build input.
#define NORRATH_HOST_ARENA
static unsigned char arena[1 << 16];
static const unsigned arena_base = 0x100000u;
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + (address - arena_base); }
#include "process_play_list.cpp"
#include <cstring>
#include <cstdlib>
#include <cmath>
#include <initializer_list>
enum { PD = 0x1000, ND = 0x1800, ED = 0x2000, AN = 0x3000, KEYS = 0x3400 };
static long long times[3]; static int timeCalls, timingRet, timingCalls, slerps;
static int plays[3] = {0, 1, 0x7fffffff};
extern "C" long long GetTime__7VIScene(void*) { return times[timeCalls++ > 1 ? 2 : timeCalls - 1]; }
extern "C" int Begin__Ct6VIPool1Z13VIHSpritePlay(void*) { return plays[0]; }
extern "C" int End__Ct6VIPool1Z13VIHSpritePlay(void*) { return 0x7fffffff; }
extern "C" int Next__Ct6VIPool1Z13VIHSpritePlayi(void*, int p) { return p == plays[0] ? plays[1] : 0x7fffffff; }
extern "C" int Next__Ct6VIPool1Z17VIHSpritePlayNodei(void*, int p) { return (p + 1) & 7; }
extern "C" unsigned char* Animation__7VIScenei(void*, int) { return arena + AN; }
extern "C" int ProcessPlayTiming__9VIHSpriteiR7VIScene(void*, int p, void*) { ++timingCalls; return p == 1 && timingRet ? -2 : 0; }
extern "C" void Slerp__6VIQuatRC6VIQuatT1f(float* o, const float* a, const float* b, float t) { ++slerps; for (int i = 0; i < 4; ++i) o[i] = a[i] * (1 - t) + b[i] * t; }
static void put(unsigned off, int v) { std::memcpy(arena + off, &v, 4); }
static void putf(unsigned off, float v) { std::memcpy(arena + off, &v, 4); }
static float getf(unsigned off) { float v; std::memcpy(&v, arena + off, 4); return v; }
static int geti(unsigned off) { int v; std::memcpy(&v, arena + off, 4); return v; }
int main() {
    alignas(8) static unsigned char o[1024];
    const long long nine = 9;
    for (int compact = 0; compact < 2; ++compact) for (int flag8 = 0; flag8 < 2; ++flag8) for (int mode = 0; mode < 4; ++mode) for (int ntype = 0; ntype < 3; ++ntype) {
        std::memset(arena, 0, sizeof arena); std::memset(o, 0, sizeof o);
        timeCalls = timingCalls = slerps = 0; timingRet = 0; times[0] = 5; times[1] = 5; times[2] = 5;
        std::memcpy(o + 184, &nine, 8);
        put(0, 0); // placeholder so arena base address zero is mapped
        unsigned pd = arena_base + PD, nd = arena_base + ND, ed = arena_base + ED;
        std::memcpy(o + 128, &pd, 4); std::memcpy(o + 152, &nd, 4); std::memcpy(o + 72, &ed, 4);
        put(AN + 8, compact); put(AN + 40, arena_base + KEYS);
        // play 0: one node (position 3), keys 1 and 2
        put(PD + 8, flag8); put(PD + 12, mode); putf(PD + 32, 0.25f); put(PD + 44, 1); put(PD + 48, 2); putf(PD + 56, 0.5f); put(PD + 80, 1); put(PD + 84, 3);
        // play 1 has a zero count
        put(PD + 104 + 80, 0);
        put(ND + 52 * 3 + 0, ntype); put(ND + 52 * 3 + 4, 2); put(ND + 52 * 3 + 8, 4);
        float pose[8] = {0.1f, 0.2f, 0.3f, 0.4f, 2.0f, 1.0f, 3.0f, 5.0f};
        for (int i = 0; i < 8; ++i) putf(ND + 52 * 3 + 12 + (i < 4 ? 4 * i : i == 4 ? 16 : 20 + 4 * (i - 5)), pose[i]);
        unsigned e = ED + 256 * 2;
        put(e + 12, 10);
        float cur[8] = {1, 2, 3, 4, 8, 16, 32, 64};
        for (int i = 0; i < 4; ++i) putf(e + 16 + 4 * i, cur[i]);
        putf(e + 32, cur[4]); for (int i = 0; i < 3; ++i) putf(e + 36 + 4 * i, cur[5 + i]);
        float k1[8] = {0.5f, 0.25f, -0.5f, 0.75f, 1.5f, 10.0f, 20.0f, 30.0f}, k2[8] = {-1.0f, 2.0f, 0.5f, 0.25f, 3.5f, -10.0f, 4.0f, 7.0f};
        if (!compact) {
            for (int i = 0; i < 8; ++i) { putf(KEYS + 32 * (4 + 1) + 4 * i, k1[i]); putf(KEYS + 32 * (4 + 2) + 4 * i, k2[i]); }
        } else {
            short s1[8] = {16384, -8192, 4096, 1000, 512, 1024, -512, 256}, s2[8] = {-16384, 100, 200, 300, 1024, 2048, -1024, 64};
            for (int i = 0; i < 8; ++i) { std::memcpy(arena + KEYS + 16 * (4 + 1) + 2 * i, &s1[i], 2); std::memcpy(arena + KEYS + 16 * (4 + 2) + 2 * i, &s2[i], 2); }
            for (int i = 0; i < 4; ++i) { k1[i] = s1[i] * 0x1.0002p-15f; k2[i] = s2[i] * 0x1.0002p-15f; }
            for (int i = 4; i < 8; ++i) { k1[i] = s1[i] / 512.0f; k2[i] = s2[i] / 512.0f; }
        }
        if (ntype == 2) { for (int i = 0; i < 8; ++i) { k1[i] = pose[i]; k2[i] = pose[i]; } }
        for (int frame : {5, 10, 11}) {
            // restore entry for each frame
            put(e + 12, 10);
            for (int i = 0; i < 4; ++i) putf(e + 16 + 4 * i, cur[i]);
            putf(e + 32, cur[4]); for (int i = 0; i < 3; ++i) putf(e + 36 + 4 * i, cur[5 + i]);
            for (int i = 0; i < 8; ++i) putf(e + 224 + 4 * (i < 4 ? i : i == 4 ? 4 : i), 100.0f + i);
            put(ND + 52 * 3, ntype);
            std::memcpy(o + 184, &nine, 8); timeCalls = timingCalls = slerps = 0;
            if (ProcessPlayList__9VIHSpriteiR7VIScene(o, frame, nullptr) != 0 || timingCalls != 2) return 1;
            float want[8];
            if (frame >= 10) { for (int i = 0; i < 8; ++i) want[i] = 100.0f + i; want[4] = 104.0f;
                // layout in the table is quaternion, scale, position at +224
                float q0 = getf(e + 16), p0 = getf(e + 36);
                if (q0 != 100.0f || getf(e + 32) != 104.0f || p0 != 105.0f) return 2;
                continue; }
            float t1[8], t2[8], t3[8];
            for (int i = 0; i < 8; ++i) { t1[i] = k1[i]; t2[i] = k2[i]; }
            if (flag8 == 0) for (int i = 0; i < 8; ++i) t3[i] = t1[i];
            else {
                for (int i = 0; i < 4; ++i) t3[i] = t1[i] * (1 - 0.5f) + t2[i] * 0.5f;
                t3[4] = t1[4] + (t2[4] - t1[4]) * 0.5f;
                for (int i = 5; i < 8; ++i) t3[i] = (t2[i] - t1[i]) * 0.5f + t1[i];
            }
            if (mode == 1) {
                for (int i = 0; i < 4; ++i) t3[i] = cur[i] * (1 - 0.25f) + t3[i] * 0.25f;
                t3[4] = cur[4] + (t3[4] - cur[4]) * 0.25f;
                for (int i = 5; i < 8; ++i) t3[i] = (t3[i] - cur[i]) * 0.25f + cur[i];
            }
            for (int i = 0; i < 8; ++i) want[i] = (mode == 0 || mode == 1) ? t3[i] : cur[i];
            for (int i = 0; i < 4; ++i) if (getf(e + 16 + 4 * i) != want[i]) return 3;
            if (getf(e + 32) != want[4]) return 4;
            for (int i = 0; i < 3; ++i) if (std::fabs(getf(e + 36 + 4 * i) - want[5 + i]) > 1e-3f * std::fabs(want[5 + i]) + 1e-6f) return 5;
            int expectType = ntype == 1 ? 2 : ntype;
            if (geti(ND + 52 * 3) != expectType) return 6;
            if (ntype == 1) { // stored pose is t1
                for (int i = 0; i < 4; ++i) if (getf(ND + 52 * 3 + 12 + 4 * i) != t1[i]) return 7;
            }
        }
        // negative timing result stops
        timingRet = 1; timeCalls = timingCalls = 0; std::memcpy(o + 184, &nine, 8);
        if (ProcessPlayList__9VIHSpriteiR7VIScene(o, 5, nullptr) != -1) return 8;
        // same time stamp: nothing happens
        long long same = 5; std::memcpy(o + 184, &same, 8); timeCalls = timingCalls = 0; timingRet = 0;
        if (ProcessPlayList__9VIHSpriteiR7VIScene(o, 5, nullptr) != 0 || timingCalls != 0) return 9;
    }
    return 0;
}
