// Host contract for cast__19SkillSummonSkeletoniP8CreatureG6Point3s; not an EE input.
// Build with -ffp-contract=off. 32-bit words are offsets from a host base; every callee logs, and the expected log is
// produced by an independently written model of the steps in cast.cpp's comment.
#define W13_SKILL_CAST_B_HOST 1
#include <cstdio>
#include <cstring>
#include <string>
#include "../w13-skill-cast-b-common/skill_access.hpp"
unsigned char* w13_skill_cast_b_host_base;
static std::string calls;
static unsigned char arena[0x1000] __attribute__((aligned(16)));
static char model_marker, sound_marker;
static int shadow_points;
static unsigned char blocks[2][2048];
static bool light_ok, object_ok;
static int ramp_b;
static std::string fb(float f) { unsigned u; std::memcpy(&u, &f, 4); return std::to_string(u); }
static std::string off(const void* p) { return p ? std::to_string(static_cast<const unsigned char*>(p) - arena) : std::string("null"); }
static std::string pt(const Point3& p) { return fb(p.x) + "," + fb(p.y) + "," + fb(p.z); }
int w13_skill_cast_b_host_call(unsigned function_word, void* adjusted_skill, int a1, int a2) {
    calls += "ramp" + std::to_string(function_word) + "(" + off(adjusted_skill) + "," + std::to_string(a1) + "," + std::to_string(a2) + ");";
    return a1 == 1 ? ramp_b : a1 * 1000 + a2 * 7 + 30;
}
int w13_skill_cast_b_host_call_object(unsigned, void*, void*) { return 0; }
int w13_skill_cast_b_host_call_ramp(unsigned, void*, int, int, float, int, int*, int*) { return 0; }
void* w13_skill_cast_b_host_call_object0(unsigned function_word, void* adjusted_object) {
    calls += "model" + std::to_string(function_word) + "(" + off(adjusted_object) + ");";
    return &model_marker;
}
#include "cast.cpp"
extern "C" {
unsigned eRandState, syncRandState;
void* world = &sound_marker;
unsigned char summonskeletonParticle1[16], summonskeletonParticle2[16];
unsigned char skelightParams[64];
char vt_spell_base[48], vt_spell_summon_skeleton[48];
}
extern "C" {
void* blockAlloc__Fi(int size) {
    calls += "alloc(" + std::to_string(size) + ");";
    if (size == 1112) return light_ok ? static_cast<void*>(blocks[0]) : nullptr;
    return object_ok ? static_cast<void*>(blocks[1]) : nullptr;
}
int modelGetNmShadowPoints__FP12_modelHeader(void* model) { calls += "points(" + std::to_string(model == &model_marker) + ");"; return shadow_points; }
void modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f(void* model, void* animation, Point3 position, short angle, int index, Point3* out, float* a, void* b, float scale) {
    calls += "shadow(" + std::to_string(model == &model_marker) + "," + off(animation) + "," + pt(position) + "," + std::to_string(angle) + "," + std::to_string(index) + "," + std::to_string(a == nullptr) +
             "," + std::to_string(b == nullptr) + "," + fb(scale) + ");";
    out->x = index * 1.5f + 0.25f;
    out->y = index * 0.5f - 3.0f;
    out->z = index * 2.0f + 100.0f;
}
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* definition, Point3* position, Point3* velocity, float scale) {
    calls += std::string("particle") + (definition == summonskeletonParticle1 ? "1" : definition == summonskeletonParticle2 ? "2" : "?") + "(" + pt(*position) + "," + pt(*velocity) + "," + fb(scale) + ");";
}
float icos__Fi(int angle) { calls += "cos(" + std::to_string(angle) + ");"; return angle * 0.5f; }
float isin__Fi(int angle) { calls += "sin(" + std::to_string(angle) + ");"; return angle * 0.25f; }
float worldFindHeight__FP12_worldHeaderG6Point3f(void* header, Point3 p, float scale) {
    calls += "height(" + std::to_string(header == &sound_marker) + "," + pt(p) + "," + fb(scale) + ");";
    return p.x * 0.125f + 5.0f;
}
void skelight_ctor(void* self, Point3 p, short angle, char** params, char flag, int value) {
    calls += "skelight(" + std::to_string(self == blocks[0]) + "," + pt(p) + "," + std::to_string(angle) + "," + (params ? std::to_string(reinterpret_cast<unsigned char*>(params) - skelightParams) : std::string("null")) + "," +
             std::to_string(flag) + "," + std::to_string(value) + ");";
}
void* lumpFindResource__FPcT0(char* lump, char* name) { calls += std::string("lump(") + lump + "," + name + ");"; return &sound_marker; }
void soundPlaySequence__FP15AnimationHeaderG6Point3f(void* sound, Point3 p, float volume) { calls += "sound(" + std::to_string(sound == &sound_marker) + "," + pt(p) + "," + fb(volume) + ");"; }
void listOnewayLink__FP8ListHeadT0Pv(void* list, void* node, void* data) {
    calls += "link(" + std::to_string(list == blocks[1] + 24) + "," + std::to_string(node == blocks[0]) + "," + std::to_string(data == blocks[0]) + ");";
}
}
static void put32(unsigned char* where, unsigned value) { std::memcpy(where, &value, 4); }
// --- independent model -------------------------------------------------------------------------------------------
static unsigned step(unsigned s) { return s * 1664525u + 1013904223u; }
static float unit_float(unsigned s) { return static_cast<float>(s >> 16) / 65536.0f; }
static Point3 ball(unsigned& s) {
    for (;;) {
        s = step(s); const float a = unit_float(s);
        s = step(s); const float b = unit_float(s);
        s = step(s); const float c = unit_float(s);
        const float x = 2.0f * a - 1.0f, y = 2.0f * b - 1.0f, z = 2.0f * c - 1.0f;
        float len = x * x;
        len = len + y * y;
        len = len + z * z;
        if (!(len > 1.0f)) return Point3{x, y, z};
    }
}
int main() {
    w13_skill_cast_b_host_base = arena;
    unsigned char* const skill = arena + 0x40;
    unsigned char* const creature = arena + 0x200;
    unsigned char* const skill_table = arena + 0xa00;
    unsigned char* const creature_table = arena + 0xb00;
    const unsigned seeds[] = {1u, 12345u, 0xdeadbeefu, 0u};
    unsigned seed = 7;
    int trial_count = 0;
    for (int shadow : {0, 1, 3, 7})
        for (int rank : {-1, 0, 1, 5})
            for (int flagcase = 0; flagcase < 8; ++flagcase)
              for (int ramp_choice = 0; ramp_choice < 4; ++ramp_choice)
                for (int alloc_case = 0; alloc_case < 3; ++alloc_case) {
                    seed = seed * 1103515245u + 12345u;
                    ++trial_count;
                    shadow_points = shadow;
                    ramp_b = ramp_choice == 0 ? 0 : ramp_choice == 1 ? 25 : ramp_choice == 2 ? 80 : 1000 + rank * 7 + 30;
                    light_ok = alloc_case >= 1;
                    object_ok = alloc_case == 2;
                    const bool flag_set = (flagcase & 1) || flagcase == 6;
                    const unsigned player_index = flagcase >> 1;      // 0..3 (flagcase 6 and 7 are overridden below)
                    const unsigned player_word = (flagcase == 7) ? 9u : (flagcase == 6 ? 4u : player_index);  // flagcase 6 and 7 are out-of-range indexes
                    std::memset(arena, 0x11, sizeof arena);
                    std::memset(blocks, 0xcc, sizeof blocks);
                    put32(skill + 96, static_cast<unsigned>(skill_table - arena));
                    const short skill_adjust = -12;
                    std::memcpy(skill_table + 88, &skill_adjust, 2); put32(skill_table + 92, 0x7001);
                    put32(creature + 72, static_cast<unsigned>(creature_table - arena));
                    const short creature_adjust = -20;
                    std::memcpy(creature_table + 128, &creature_adjust, 2); put32(creature_table + 132, 0x5555);
                    put32(creature + 12, flag_set ? 0x41u : 0x01u);
                    put32(creature + 960, player_word);
                    const float cx = 4.5f, cy = -3.25f, cz = 80.0f;
                    std::memcpy(creature + 16, &cx, 4); std::memcpy(creature + 20, &cy, 4); std::memcpy(creature + 24, &cz, 4);
                    const short facing = -1234;
                    std::memcpy(creature + 56, &facing, 2);
                    eRandState = seeds[seed % 4];
                    syncRandState = (trial_count % 5 == 0 && ramp_choice == 0) ? 0x25d60fe5u : (trial_count % 5 == 1 && ramp_choice == 1) ? 0x65d60fe5u : seeds[(seed >> 3) % 4] + 77;  // 0x25d60fe5 advances to a state whose high half is zero; 0x65d60fe5 to one whose high half is 0x4000 (roll 0.25, the boundary for ramp 25)
                    const short angle = static_cast<short>(seed >> 9);
                    const Point3 P{10.5f, -20.0f, 1000.0f};
                    calls.clear();
                    const unsigned expected_erand0 = eRandState, expected_sync0 = syncRandState;
                    void* r = cast_summon_skeleton(skill, rank, creature, P, angle);
                    // ---- expected
                    std::string e;
                    unsigned er = expected_erand0, sy = expected_sync0;
                    const std::string model_call = "model21845(" + off(creature - 20) + ");";
                    e += model_call + "points(1);";
                    Point3 pos{cx, cy, cz};
                    if (shadow != 0) {
                        for (int i = 0; i < 32; ++i) {
                            e += model_call;
                            er = step(er);
                            const int index = static_cast<int>(er >> 16) % shadow;
                            Point3 q{index * 1.5f + 0.25f, index * 0.5f - 3.0f, index * 2.0f + 100.0f};
                            e += "shadow(1," + off(creature + 288) + "," + pt(pos) + "," + std::to_string(facing) + "," + std::to_string(index) + ",1,1," + fb(1.0f) + ");";
                            const Point3 u1 = ball(er);
                            q.x = q.x + u1.x * 0.23699799180030823f; q.y = q.y + u1.y * 0.23699799180030823f; q.z = q.z + u1.z * 0.23699799180030823f;
                            const Point3 u2 = ball(er);
                            const Point3 v{u2.x * 0.06800499558448792f, u2.y * 0.06800499558448792f, u2.z * 0.06800499558448792f};
                            e += "particle1(" + pt(q) + "," + pt(v) + "," + fb(0.0f) + ");";
                        }
                        for (int i = 0; i < 200; ++i) {
                            e += model_call;
                            er = step(er);
                            const int index = static_cast<int>(er >> 16) % shadow;
                            const Point3 q{index * 1.5f + 0.25f, index * 0.5f - 3.0f, index * 2.0f + 100.0f};
                            e += "shadow(1," + off(creature + 288) + "," + pt(pos) + "," + std::to_string(facing) + "," + std::to_string(index) + ",1,1," + fb(1.0f) + ");";
                            const Point3 v{(P.x - q.x) * -2.0004379749298096f, (P.y - q.y) * -2.0004379749298096f, (P.z - q.z) * -2.0004379749298096f};
                            e += "particle2(" + pt(q) + "," + pt(v) + "," + fb(0.0f) + ");";
                        }
                    }
                    e += "cos(" + std::to_string(angle) + ");sin(" + std::to_string(angle) + ");";
                    Point3 spot{P.x + angle * 0.5f * 36.0f, P.y + angle * 0.25f * 36.0f, 0.0f};
                    spot.z = spot.x * 0.125f + 5.0f;
                    e += "height(1," + pt(Point3{spot.x, spot.y, P.z}) + "," + fb(12.0f) + ");";
                    sy = step(sy);
                    const float roll = unit_float(sy);
                    const int value = 0 * 1000 + rank * 7 + 30, ramp = ramp_b;
                    e += "ramp28673(" + off(skill - 12) + ",0," + std::to_string(rank) + ");ramp28673(" + off(skill - 12) + ",1," + std::to_string(rank) + ");";
                    const bool use_params = flag_set && player_word < 4;
                    const int flag = roll < static_cast<float>(ramp) * 0.009999999776482582f ? 1 : 0;
                    e += "alloc(1112);";
                    if (light_ok) e += "skelight(1," + pt(spot) + "," + std::to_string(angle) + "," + (use_params ? std::to_string(8 * player_word) : std::string("null")) + "," + std::to_string(flag) + "," + std::to_string(value) + ");";
                    e += "lump(fx.lmp,spel_de_sumskel.anm);sound(1," + pt(P) + "," + fb(1.0f) + ");";
                    void* want = nullptr;
                    if (light_ok) {
                        e += "alloc(28);";
                        if (object_ok) {
                            sy = step(sy);
                            e += "link(1,1,1);";
                            want = blocks[1];
                        }
                    }
                    // --- compare
                    if (r != want) { std::puts("return"); return 1; }
                    if (calls != e) { std::puts(calls.c_str()); std::puts(e.c_str()); return 2; }
                    if (eRandState != er) return 3;
                    if (syncRandState != sy) return 4;
                    short group[4]; std::memcpy(group, creature + 148, 8);
                    if (group[0] != -128 || group[1] != -110 || group[2] != -128 || group[3] != 30) return 5;
                    if (light_ok) {
                        unsigned long long v160; std::memcpy(&v160, blocks[0] + 160, 8);
                        short h792; std::memcpy(&h792, blocks[0] + 792, 2);
                        unsigned long long want160 = 0xccccccccccccccccull;
                        if (rank > 0) {
                            want160 |= 0x400000ull;
                            if (h792 != -1) return 6;
                            if (flag_set ? blocks[0][85] != static_cast<unsigned char>(player_word) : blocks[0][85] != 0xcc) return 7;
                        } else {
                            if (h792 != static_cast<short>(0xcccc)) return 8;
                            if (blocks[0][85] != 0xcc) return 9;
                        }
                        if (v160 != want160) return 10;
                    }
                    if (want) {
                        unsigned w[7]; std::memcpy(w, blocks[1], 28);
                        if ((w[0] & 0xff) != static_cast<unsigned>(rank & 0xff) || w[1] != 0 || w[2] != word_of(skill) || w[3] != word_of(creature) || w[4] != (sy >> 16) ||
                            w[5] != word_of(vt_spell_summon_skeleton) || w[6] != 0) return 11;
                    }
                }
    (void)trial_count;
    return 0;
}
