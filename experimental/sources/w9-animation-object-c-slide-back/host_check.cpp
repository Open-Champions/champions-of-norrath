// Host contract for slideBack__8Creaturef. Not an EE plan input.
// Build: c++ -std=c++17 -ffp-contract=off -DNORRATH_HOST_ARENA host_check.cpp
#ifndef NORRATH_HOST_ARENA
#define NORRATH_HOST_ARENA
#endif
#include <string>
#include <vector>
#include "../w9-animation-object-c-common/host_arena.hpp"
#include "../w9-animation-object-c-common/host_check.hpp"
#include "slide_back.cpp"
unsigned char fxDust[80];
int levelFlags;
static std::vector<std::string> events;
static int move_result;
static unsigned char* creature;
static unsigned rand_table[64];
static unsigned rand_index;
static std::string fl(float f) { char b[40]; std::snprintf(b, sizeof b, "%.6g", f); return b; }
extern "C" float icos__Fi(int a) { events.push_back("cos" + std::to_string(a)); return 0.5f; }
extern "C" float isin__Fi(int a) { events.push_back("sin" + std::to_string(a)); return 0.25f; }
extern "C" int move2__8CreatureG6Point3(unsigned char* self, const float* push) {
    events.push_back("move2:" + fl(push[0]) + "," + fl(push[1]) + "," + fl(push[2]) + " vec:" + fl(lf_(self + 180)) + "," + fl(lf_(self + 184)) +
                     " half:" + std::to_string(self[188] | (self[189] << 8)) + (self == creature ? "" : " BADSELF"));
    return move_result;
}
extern "C" unsigned eRand__10GameObject(unsigned char* self) { events.push_back("rand"); return rand_table[rand_index++ % 64]; }
extern "C" void addEffect__FiG6Point3(int kind, const float* p) { events.push_back("effect" + std::to_string(kind) + ":" + fl(p[0]) + "," + fl(p[1]) + "," + fl(p[2])); }
extern "C" void P_AddParticle__FP11ParticleDefP6Point3T1f(unsigned char* def, const float* p, const float* v, float age) {
    events.push_back(std::string("particle") + (def == fxDust ? "" : "!BADDEF") + ":" + fl(p[0]) + "," + fl(p[1]) + "," + fl(p[2]) + " v:" + fl(v[0]) + "," + fl(v[1]) + "," + fl(v[2]) + " age:" + fl(age));
}
int main() {
    creature = arena_at(0x100);
    int failures = 0;
    for (unsigned i = 0; i != 64; ++i) rand_table[i] = (i * 9973u + 12345u) & 0xffff;
    auto setup = [&](float v, unsigned flags, int move, signed char blocked) {
        std::memset(arena_at(0), 0x6b, 0x1000);
        sw_(creature + 12, flags); sh_(creature + 748, 0xff00u); sf_(creature + 756, v);
        sf_(creature + 16, 100.0f); sf_(creature + 20, 200.0f); sf_(creature + 24, 300.0f); sf_(creature + 28, 20.0f);
        sf_(creature + 180, 1.5f); sf_(creature + 184, 2.5f); sf_(creature + 188, 3.5f); sh_(creature + 188, 0x1234);
        creature[568] = static_cast<unsigned char>(blocked);
        events.clear(); move_result = move; rand_index = 0;
    };
    auto expect = [&](bool ok, const char* m) { if (!ok) { std::printf("fail: %s\n", m); for (auto& e : events) std::printf("  %s\n", e.c_str()); ++failures; } };
    // Early exits.
    setup(4.0f, 0, 0, 0); slideBack__8Creaturef(creature, 0.0f); expect(events.empty(), "t == 0 does nothing");
    setup(4.0f, 0, 0, 0); slideBack__8Creaturef(creature, -1.0f); expect(events.empty(), "t < 0 does nothing");
    setup(0.0f, 0, 0, 0); slideBack__8Creaturef(creature, 1.0f); expect(events.empty(), "zero speed does nothing");
    setup(4.0f, 2, 0, 0); slideBack__8Creaturef(creature, 1.0f); expect(events.empty(), "flag bit 1 does nothing");
    setup(4.0f, 0xfffffffdu, 0, 0); slideBack__8Creaturef(creature, 1.0f); expect(!events.empty(), "other flag bits do not stop it");
    // Blocked move: push, vector swap, speed decay, no puffs.
    setup(4.0f, 0, 1, 0);
    slideBack__8Creaturef(creature, 2.0f);
    expect(events.size() == 3 && events[0] == "cos-256" && events[1] == "sin-256", "angle is the signed halfword");
    expect(events[2] == "move2:4,2,0 vec:0,0 half:0", "push is (0.5*4*2, 0.25*4*2, 0) with the vector and halfword cleared");
    expect(lf_(creature + 180) == 1.5f && lf_(creature + 184) == 2.5f, "vector restored (x, y)");
    expect(creature[188] == 0x34 && creature[189] == 0x12 && creature[190] == 0x60 && creature[191] == 0x40, "halfword at +188 restored");
    expect(lf_(creature + 756) == 2.0f, "speed lowered by 2.0");
    // Speed 1.0 goes to -1.0 then clamps to zero.
    setup(1.0f, 0, 2, 0); slideBack__8Creaturef(creature, 1.0f); expect(lf_(creature + 756) == 0.0f, "speed clamps at zero");
    // Speed is not lowered when it is already negative, and a negative speed stays unchanged.
    setup(-3.0f, 0, 1, 0); slideBack__8Creaturef(creature, 1.0f); expect(lf_(creature + 756) == 0.0f, "negative speed becomes zero");
    // Byte 568 set: stops before the puffs even when the move was not blocked.
    setup(4.0f, 0, 0, 1); slideBack__8Creaturef(creature, 1.0f); expect(events.size() == 3, "blocked flag byte stops the puffs");
    setup(4.0f, 0, 0, -1); slideBack__8Creaturef(creature, 1.0f); expect(events.size() == 3, "negative byte also stops the puffs");
    // Free move: two puffs. Check the exact event stream against an independent computation.
    for (int effect = 0; effect != 2; ++effect) {
        for (int blocked_bits : {1, 2, 3, 0x13}) {
            setup(4.0f, 0, blocked_bits, 0);
            levelFlags = effect ? 0x100 : 0;
            slideBack__8Creaturef(creature, 1.0f);
            expect(events.size() == 3, "a move result with either of the low two bits stops the puffs");
        }
        setup(4.0f, 0, 4, 0);
        levelFlags = effect ? 0x100 : 0;
        slideBack__8Creaturef(creature, 1.0f);
        expect(events.size() > 3, "a move result with only higher bits still makes puffs");
        setup(4.0f, 0, 0, 0);
        levelFlags = effect ? 0x100 : 0;
        slideBack__8Creaturef(creature, 1.0f);
        std::vector<std::string> got;
        for (auto& e : events) if (e != "rand") got.push_back(e);
        // Independent expectation.
        unsigned k = 0;
        auto next = [&]() { return rand_table[k++ % 64]; };
        std::vector<std::string> want = {"cos-256", "sin-256", "move2:2,1,0 vec:0,0 half:0"};
        for (int puff = 0; puff != 2; ++puff) {
            float bx = 100.0f - 10.0f + float(int(next()) % 20);
            float by = 200.0f - 10.0f + float(int(next()) % 20);
            float bz = 300.0f + float(next() % 10u);
            float vx, vy, vz;
            for (;;) {
                vx = float(next()) / 65536.0f * 2.0f - 1.0f;
                vy = float(next()) / 65536.0f * 2.0f - 1.0f;
                vz = float(next()) / 65536.0f * 2.0f - 1.0f;
                if (!(vx * vx + vy * vy + vz * vz > 1.0f)) break;
            }
            float px = bx + float(int(next() % 12u) - 6), py = by + float(int(next() % 12u) - 6), pz = bz + float(int(next() % 12u) - 6);
            if (effect) want.push_back("effect17:" + fl(px) + "," + fl(py) + "," + fl(pz));
            else want.push_back("particle:" + fl(px) + "," + fl(py) + "," + fl(pz) + " v:" + fl(vx * 10.45f) + "," + fl(vy * 10.45f) + "," + fl(vz * 10.45f) + " age:0");
        }
        if (got != want) {
            std::printf("fail: puff stream (effect=%d)\n", effect);
            for (size_t i = 0; i < got.size() || i < want.size(); ++i)
                std::printf("  got %-60s want %s\n", i < got.size() ? got[i].c_str() : "-", i < want.size() ? want[i].c_str() : "-");
            ++failures;
        }
    }
    if (failures) return 1;
    std::printf("slideBack check passed\n");
    return 0;
}
