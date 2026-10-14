// Host contract for msg_run__6Sparks; not an EE input. Pointer words are offsets from a host base, every callee
// is a recording stub and rolls come from a script. The expectations use component values that are exact in
// single precision (rolls 49152 -> 0.5 and 32768 -> 0.0 after the doubling and offset).
#define W11_ACTOR_C_HOST 1
#include "msg_run.cpp"
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
unsigned char* w11_actor_c_host_base;
W11ActorCHostSlot w11_actor_c_host_slots[1];
int frameNm;
unsigned char norrath_w11_actor_c_view_7d7540[80], norrath_w11_actor_c_view_7d7590[80], norrath_w11_actor_c_view_7d7630[80], norrath_w11_actor_c_view_7d7680[80];
static std::string calls;
static std::vector<unsigned> rolls; static size_t used;
static int move_result;
static std::string vec(const Point3& p) { char b[96]; std::snprintf(b, sizeof b, "(%g,%g,%g)", p.x, p.y, p.z); return b; }
static const char* defname(const void* d) {
    return d == norrath_w11_actor_c_view_7d7540 ? "trail" : d == norrath_w11_actor_c_view_7d7590 ? "smoke" : d == norrath_w11_actor_c_view_7d7630 ? "hit" : d == norrath_w11_actor_c_view_7d7680 ? "fire" : "?";
}
static void* slot_int0(void* adjusted, int flags) { calls += "delete(" + std::to_string(static_cast<unsigned char*>(adjusted) - w11_actor_c_host_base) + "," + std::to_string(flags) + ");"; return nullptr; }
W11ActorCHostSlotInt w11_actor_c_host_slots_int[] = {slot_int0};
extern "C" {
unsigned eRand__10GameObject(void*) { return rolls.at(used++ % rolls.size()); }
int move__10ProjectileG6Point3(void* self, Point3 v) { calls += "move" + vec(v) + ";"; return move_result; }
void soundPan2D__FiP6Point3f(int s, Point3* p, float g) { calls += "pan(" + std::to_string(s) + "," + vec(*p) + "," + std::to_string(g) + ");"; }
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* d, Point3* p, Point3* v, float e) { calls += std::string("part(") + defname(d) + vec(*p) + vec(*v) + std::to_string(e) + ");"; }
void lightUpdatePosition__FP5Light(void* l) { calls += "light(" + std::to_string(static_cast<unsigned char*>(l) - w11_actor_c_host_base) + ");"; }
void makeExplosion__FG6Point3f(Point3 p, float s) { calls += "expl" + vec(p) + std::to_string(s) + ";"; }
void objectRadialDamage__FP10GameObjectG6Point3iffiii(void* o, Point3 p, int a, float b, float c, int d, int e, int f) {
    calls += "rad(" + std::to_string(static_cast<unsigned char*>(o) - w11_actor_c_host_base) + vec(p) + "," + std::to_string(a) + "," + std::to_string(b) + "," + std::to_string(c) + "," + std::to_string(d) + "," + std::to_string(e) + "," + std::to_string(f) + ");";
}
}
static std::vector<unsigned> script(size_t draws, bool oddNext, size_t trailing) {
    std::vector<unsigned> r;
    for (size_t i = 0; i < draws; ++i) { r.push_back(49152); r.push_back(32768); r.push_back(32768); }
    if (oddNext) r.push_back(32769);
    for (size_t i = 0; i < trailing; ++i) { r.push_back(49152); r.push_back(32768); r.push_back(32768); }
    return r;
}
int main() {
    static unsigned char base[8192];
    w11_actor_c_host_base = base;
    int failures = 0;
    unsigned char* self = base + 64;
    auto setup = [&](int fn, unsigned w84, int w176, unsigned w76) {
        std::memset(base, 0, sizeof base);
        frameNm = fn;
        std::memcpy(self + 84, &w84, 4);
        std::memcpy(self + 176, &w176, 4);
        const int w180 = 25; std::memcpy(self + 180, &w180, 4);
        const int w164 = 77; std::memcpy(self + 164, &w164, 4);
        std::memcpy(self + 76, &w76, 4);
        const unsigned holderWord = 3000; std::memcpy(base + 1024 + 12, &holderWord, 4);
        const unsigned table = 2000; std::memcpy(self + 72, &table, 4);
        const short adj = 4; std::memcpy(base + 2000 + 8, &adj, 2);
        const unsigned fn0 = 0; std::memcpy(base + 2000 + 12, &fn0, 4);
        const float pos[3] = {1.0f, 2.0f, 3.0f}, vel[3] = {60.0f, 120.0f, -60.0f};
        std::memcpy(self + 16, pos, 12); std::memcpy(self + 88, vel, 12);
        calls.clear(); used = 0;
    };
    auto expect = [&](const char* label, const std::string& want) {
        if (calls != want) { std::fprintf(stderr, "%s:\n got  %s\n want %s\n", label, calls.c_str(), want.c_str()); ++failures; }
    };
    std::string head = "move(60,120,-60);pan(77,(1,2,3),1.000000);";
    for (int i = 0; i < 3; ++i) head += "part(trail(1,2,3)(4.8,0,0)0.000000);";
    for (int i = 0; i < 5; ++i) head += "part(smoke(1,2,3)(4.8,0,0)0.000000);";
    // 3 trail draws + 5 * 2 smoke draws = 13 draws, then one roll that decides the extra fire smoke
    for (int odd = 0; odd < 2; ++odd)
        for (int mv = 0; mv < 2; ++mv) {
            setup(5, 10, 7, 0); move_result = mv; rolls = script(13, odd, 1);
            msg_run__6Sparks(self);
            std::string want = head;
            if (odd) want += "part(fire(-4,-8,8)(5,0,0)0.000000);";
            want += "light(164);";
            expect("active", want);
            unsigned w84; std::memcpy(&w84, self + 84, 4);
            if (w84 != (mv == 1 ? 0u : 10u)) { std::fprintf(stderr, "active: end word %u after move %d\n", w84, mv); ++failures; }
            float lightPos[3]; std::memcpy(lightPos, self + 148, 12);
            if (lightPos[0] != 1.0f || lightPos[1] != 2.0f || lightPos[2] != 3.0f) { std::fprintf(stderr, "active: light position\n"); ++failures; }
            if (used != static_cast<size_t>(13 * 3 + 1 + (odd ? 3 : 0))) { std::fprintf(stderr, "active: %zu rolls used\n", used); ++failures; }
        }
    // both sides are compared as signed ints
    setup(-1, 5, 0, 0); move_result = 0; rolls = script(13, false, 1);
    msg_run__6Sparks(self);
    if (calls.compare(0, 5, "move(") != 0) { std::fprintf(stderr, "negative frame must be below a positive end\n"); ++failures; }
    setup(5, static_cast<unsigned>(-2), 0, 0); move_result = 0; rolls = script(50, false, 0);
    msg_run__6Sparks(self);
    if (calls.compare(0, 5, "move(") == 0) { std::fprintf(stderr, "negative end word must not count as a late frame\n"); ++failures; }
    // ended: fifty burst particles, then optional damage, then the delete slot
    std::string burst;
    for (int i = 0; i < 50; ++i) burst += "part(hit(1,2,3)(28.8,0,0)0.000000);";
    for (int w76 : {0, 1024})
        for (int w176 : {5, 0, -1}) {
            setup(10, 10, w176, w76); rolls = script(50, false, 0);
            msg_run__6Sparks(self);
            std::string want = burst;
            if (w176 > 0) want += "expl(1,2,3)2.000000;rad(" + std::string(w76 ? "3000" : "64") + "(1,2,3)," + std::to_string(w176) + ",40.000000,25.000000,264224," + std::to_string(w176) + ",0);";
            want += "delete(68,3);";
            expect("ended", want);
        }
    if (!failures) std::puts("Sparks msg_run ok");
    return failures != 0;
}
