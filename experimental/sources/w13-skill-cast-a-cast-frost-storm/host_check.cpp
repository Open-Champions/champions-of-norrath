// Host contract for cast__15SkillFrostStormiP8CreatureG6Point3s; not an EE input.
// 32-bit words are offsets from a host base; the table calls, trig, allocation, spell constructor, resource lookup and
// sound call are stubs that log. Build with -ffp-contract=off.
#define W13_SKILL_CAST_A_HOST 1
#include <cstdio>
#include <cstring>
#include <string>
unsigned char* w13_skill_cast_a_host_base;
static std::string calls;
static void* fake_block;
static int tweak_result;
static int ramps[4];
static unsigned bits(float v) { unsigned u; std::memcpy(&u, &v, 4); return u; }
static long off(const void* p) { return static_cast<long>(static_cast<const unsigned char*>(p) - w13_skill_cast_a_host_base); }
int w13_skill_cast_a_host_call(unsigned function_word, void* adjusted_skill, int a1, int a2) {
    calls += "entry(" + std::to_string(function_word) + "," + std::to_string(off(adjusted_skill)) + "," + std::to_string(a1) + "," + std::to_string(a2) + ");";
    return a2 == 20 ? tweak_result : ramps[a1];
}
#include "cast.cpp"
static float cos_of(int a) { return static_cast<float>((a * 37) % 1000) / 1000.0f - 0.4f; }
static float sin_of(int a) { return static_cast<float>((a * 53) % 1000) / 997.0f - 0.7f; }
static unsigned char arena[0x400] __attribute__((aligned(16)));
static char lump_result;
extern "C" {
void* blockAlloc__Fi(int size) { calls += "alloc(" + std::to_string(size) + ");"; return fake_block; }
float icos__Fi(int angle) { calls += "cos(" + std::to_string(angle) + ");"; return cos_of(angle); }
float isin__Fi(int angle) { calls += "sin(" + std::to_string(angle) + ");"; return sin_of(angle); }
void spell_ctor_frost_storm(void* self, Point3 q, int a, float b, int c, void* creature, Point3 e) {
    calls += "ctor(" + std::to_string(self == fake_block) + "," + std::to_string(bits(q.x)) + "," + std::to_string(bits(q.y)) + "," + std::to_string(bits(q.z)) + "," + std::to_string(a) + "," +
             std::to_string(bits(b)) + "," + std::to_string(c) + "," + std::to_string(off(creature)) + "," + std::to_string(bits(e.x)) + "," + std::to_string(bits(e.y)) + "," +
             std::to_string(bits(e.z)) + ");";
}
void* lumpFindResource__FPcT0(char* lump, char* name) { calls += std::string("lump(") + lump + "," + name + ");"; return &lump_result; }
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* sequence, void* object, float volume) {
    calls += "sound(" + std::to_string(sequence == &lump_result) + "," + std::to_string(off(object)) + "," + std::to_string(bits(volume)) + ");";
}
}
int main() {
    int bad = 0;
    w13_skill_cast_a_host_base = arena;
    unsigned char* const skill = arena + 0x40;
    unsigned char* const creature = arena + 0x200;
    unsigned char* const table = arena + 0x300;
    const unsigned table_word = static_cast<unsigned>(table - arena);
    std::memcpy(skill + 96, &table_word, 4);
    short adjust = -8;
    std::memcpy(table + 120, &adjust, 2);
    unsigned function_word = 0x4321;
    std::memcpy(table + 124, &function_word, 4);
    adjust = -4;
    std::memcpy(table + 88, &adjust, 2);
    function_word = 0x5432;
    std::memcpy(table + 92, &function_word, 4);
    const Point3 points[] = {{1.5f, -2.25f, 30.0f}, {0, 0, 0}, {-1000.125f, 77.5f, 5.0f}};
    const int ramp_sets[][4] = {{3, -4, 7, 9}, {0, 0, 0, 0}, {100000, -100000, 100000, -1}, {-7, 8, -9, 10}};
    for (int rank : {-3, 0, 99})
        for (short angle : {static_cast<short>(-32768), static_cast<short>(-1), static_cast<short>(0), static_cast<short>(1234), static_cast<short>(32767)})
            for (const Point3& p : points)
                for (const auto& rs : ramp_sets)
                    for (void* block : {static_cast<void*>(arena + 0x100), static_cast<void*>(nullptr)}) {
                        tweak_result = rank * 5 + 1;
                        std::memcpy(ramps, rs, sizeof ramps);
                        fake_block = block;
                        calls.clear();
                        if (cast_frost_storm(skill, rank, creature, p, angle) != 0) bad = 1;
                        const float dx = cos_of(angle) * 3.0f * 12.0f, dy = sin_of(angle) * 3.0f * 12.0f;
                        const float qx = p.x + dx, qy = p.y + dy, qz = p.z;
                        const float r = static_cast<float>(rs[3]);
                        const float ex = cos_of(angle) * r, ey = sin_of(angle) * r;
                        std::string expected = "entry(17185," + std::to_string(off(skill - 8)) + "," + std::to_string(rank) + ",20);cos(" + std::to_string(angle) + ");sin(" + std::to_string(angle) + ");";
                        for (int i = 0; i < 4; ++i) expected += "entry(21554," + std::to_string(off(skill - 4)) + "," + std::to_string(i) + "," + std::to_string(tweak_result) + ");";
                        expected += "cos(" + std::to_string(angle) + ");sin(" + std::to_string(angle) + ");alloc(172);";
                        if (block) expected += "ctor(1," + std::to_string(bits(qx)) + "," + std::to_string(bits(qy)) + "," + std::to_string(bits(qz)) + "," + std::to_string(rs[2] * 60) + "," +
                                               std::to_string(bits(static_cast<float>(rs[1] * 12))) + "," + std::to_string(rs[0]) + "," + std::to_string(off(creature)) + "," +
                                               std::to_string(bits(ex)) + "," + std::to_string(bits(ey)) + "," + std::to_string(bits(0.0f)) + ");";
                        expected += "lump(fx.lmp,spel_ew_froststorm.anm);sound(1," + std::to_string(off(creature)) + "," + std::to_string(bits(1.0f)) + ");";
                        if (calls != expected) { std::puts(calls.c_str()); std::puts(expected.c_str()); bad = 2; }
                    }
    return bad;
}
