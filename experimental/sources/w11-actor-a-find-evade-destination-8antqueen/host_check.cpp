// Host contract for findEvadeDestination__8AntQueen; not an EE input. (The point at +16 shares its z with the float at +24.) Words that hold addresses are offsets from
// a host base; the random source, trig, height query and world are stubs that log. The authored function was
// also compared with the original in a private R5900 run (120 scenarios). Returns non-zero when wrong.
#define W11_ACTOR_A_HOST 1
#include <cmath>
#include <cstdio>
#include <cstring>
#include <string>
unsigned char* w11_actor_a_host_base;
static std::string calls;
static unsigned rolls[8]; static int ri;
static float heights[12]; static int hi;
static std::string num(double v) { char b[40]; std::snprintf(b, sizeof b, "%g", v); return b; }
extern "C" {
void* world = reinterpret_cast<void*>(0x1234);
unsigned eRand__10GameObject(void*) { calls += "r;"; return rolls[ri++ & 7]; }
float icos__Fi(int a) { calls += "c" + num(a) + ";"; return 1.0f; }
float isin__Fi(int a) { calls += "s" + num(a) + ";"; return 0.5f; }
float worldFindHeight__FP12_worldHeaderG6Point3f(void*, const float* p, float rad) {
    calls += "h(" + num(p[0]) + "," + num(p[1]) + "," + num(p[2]) + "," + num(rad) + ");"; return heights[hi++ % 12];
}
}
#include "find_evade_destination.cpp"
int main() {
    alignas(16) static unsigned char arena[0x4000];
    w11_actor_a_host_base = arena;
    int bad = 0;
    unsigned char* o = arena + 0x1000;
    for (int mode = 0; mode < 5; ++mode) {
        std::memset(arena, 0, sizeof arena); calls.clear(); ri = hi = 0;
        unsigned node = 0x200, blk = 0x300;
        if (mode) std::memcpy(o + 576, &node, 4);
        std::memcpy(arena + 0x200 + 12, &blk, 4);
        float tx = 100, ty = 200; std::memcpy(arena + 0x300 + 16, &tx, 4); std::memcpy(arena + 0x300 + 20, &ty, 4);
        float pos[2] = {1, 2}; std::memcpy(o + 16, pos, 8);
        float z = 10, rad = 7; std::memcpy(o + 24, &z, 4); std::memcpy(o + 28, &rad, 4);
        short ang = 100; std::memcpy(o + 56, &ang, 2);
        for (int i = 0; i < 8; ++i) rolls[i] = 0x10000u * (i + 1) + 5;     // each roll (int)e*2^-16 = i+1 + tiny
        for (int i = 0; i < 12; ++i) heights[i] = (mode == 2 || mode == 3) ? 50.0f : 10.0f + (i % 3) - 1.0f;
        if (mode == 4) { heights[0] = 50; heights[1] = 50; heights[2] = 15.0f; heights[3] = 10; }
        if (mode == 3) heights[0] = -50;
        Point3 out = {-1, -1, -1};
        Point3* ret = findEvadeDestination__8AntQueen(&out, o);
        if (ret != &out) bad = 1;
        std::string want;
        auto fail = [&](const char* m) { std::printf("fail mode %d: %s\n  calls %s\n", mode, m, calls.c_str()); bad = 1; };
        if (mode == 0 || mode == 2 || mode == 3) {
            // no node, or every height out of range: the object position comes back
            if (out.x != 1 || out.y != 2 || out.z != 10) fail("fallback position");
            if (mode == 0 && !calls.empty()) fail("no calls without node");
            if (mode != 0) {
                int hcount = 0; for (size_t i = 0; (i = calls.find("h(", i)) != std::string::npos; ++i) ++hcount;
                if (hcount != 10) fail("ten tries");
            }
        } else if (mode == 1) {
            // first try: r = 5 (short of roll 1 = 0x10005); e1 = 2*65536+5 -> 2; e2 = 3 -> p = T + (e*300+300)*trig
            float ex = tx + ((float)0x20005 * (1.0f / 65536.0f) * 300.0f + 300.0f) * 1.0f;
            float ey = ty + ((float)0x30005 * (1.0f / 65536.0f) * 300.0f + 300.0f) * 0.5f;
            if (std::fabs(out.x - ex) > 1e-3f || std::fabs(out.y - ey) > 1e-3f || out.z != 10) fail("accepted point");
            if (calls.find("c5;") == std::string::npos) fail("angle is the short of the first roll");
            if (calls.find("c" + num(100 - 32767) + ";") == std::string::npos) fail("shifted angle");
        } else if (mode == 4) {
            int hcount = 0; for (size_t i = 0; (i = calls.find("h(", i)) != std::string::npos; ++i) ++hcount;
            if (hcount != 3) fail("accepts on the third try");
        }
    }
    return bad;
}
