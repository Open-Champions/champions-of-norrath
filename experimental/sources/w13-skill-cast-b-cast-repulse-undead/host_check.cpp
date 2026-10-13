// Host contract for cast__18SkillRepulseUndeadiP8CreatureG6Point3s; not an EE input.
// 32-bit words are offsets from a host base; the table entries and every callee log.
#define W13_SKILL_CAST_B_HOST 1
#include <cstdio>
#include <cstring>
#include <string>
unsigned char* w13_skill_cast_b_host_base;
static std::string calls;
static void* alloc_results[3];
static unsigned char blocks[3][256];
static int alloc_calls;
static char sound_marker;
static unsigned char arena[0x400] __attribute__((aligned(16)));
static std::string fl(float f) { return std::to_string(f); }
static std::string off(const void* p) { return p ? std::to_string(static_cast<const unsigned char*>(p) - arena) : std::string("null"); }
int w13_skill_cast_b_host_call(unsigned function_word, void* adjusted_skill, int a1, int a2) {
    calls += "entry" + std::to_string(function_word) + "(" + off(adjusted_skill) + "," + std::to_string(a1) + "," + std::to_string(a2) + ");";
    return a1 * 100 + a2 * 3 + 5;
}
int w13_skill_cast_b_host_call_object(unsigned, void*, void*) { return 0; }
int w13_skill_cast_b_host_call_ramp(unsigned, void*, int, int, float, int, int*, int*) { return 0; }
#include "cast.cpp"
static bool soul_ok, light_ok;
extern "C" {
float icos__Fi(int angle) { calls += "cos(" + std::to_string(angle) + ");"; return angle * 0.5f; }
float isin__Fi(int angle) { calls += "sin(" + std::to_string(angle) + ");"; return angle * 0.25f; }
void* blockAlloc__Fi(int size) {
    calls += "alloc(" + std::to_string(size) + ");";
    if (size == 176) return soul_ok ? static_cast<void*>(blocks[0]) : nullptr;
    return light_ok ? static_cast<void*>(blocks[1]) : nullptr;
}
void lost_soul_ctor(void* self, Point3 p, Point3 v, int angle, void* vag, int a, int b) {
    calls += "soul(" + std::to_string(self == blocks[0]) + "," + fl(p.x) + "," + fl(p.y) + "," + fl(p.z) + "," + fl(v.x) + "," + fl(v.y) + "," + fl(v.z) + "," + std::to_string(angle) + "," +
             std::to_string(vag == nullptr) + "," + std::to_string(a) + "," + std::to_string(b) + ");";
}
void light_effect_ctor(void* self, Point3 p, float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, float f9, float f10, float f11, float f12, void* object, int mode) {
    calls += "light(" + std::to_string(self == blocks[1]) + "," + fl(p.x) + "," + fl(p.y) + "," + fl(p.z) + "," + fl(f1) + "," + fl(f2) + "," + fl(f3) + "," + fl(f4) + "," + fl(f5) + "," + fl(f6) + "," +
             fl(f7) + "," + fl(f8) + "," + fl(f9) + "," + fl(f10) + "," + fl(f11) + "," + fl(f12) + "," + std::to_string(object == nullptr) + "," + std::to_string(mode) + ");";
}
void gameRadialDamage__FP10GameObjectG6Point3iffiii(void* creature, Point3 c, int a, float b, float cc, int d, int e, int f) {
    calls += "damage(" + off(creature) + "," + fl(c.x) + "," + fl(c.y) + "," + fl(c.z) + "," + std::to_string(a) + "," + fl(b) + "," + fl(cc) + "," + std::to_string(d) + "," + std::to_string(e) + "," + std::to_string(f) + ");";
}
void* lumpFindResource__FPcT0(char* lump, char* name) { calls += std::string("lump(") + lump + "," + name + ");"; return &sound_marker; }
void soundPlaySequence__FP15AnimationHeaderG6Point3f(void* sound, Point3 p, float volume) {
    calls += "sound(" + std::to_string(sound == &sound_marker) + "," + fl(p.x) + "," + fl(p.y) + "," + fl(p.z) + "," + fl(volume) + ");";
}
}
int main() {
    w13_skill_cast_b_host_base = arena;
    unsigned char* const skill = arena + 0x40;
    unsigned char* const creature = arena + 0x200;
    unsigned char* const table = arena + 0x300;
    const unsigned table_word = static_cast<unsigned>(table - arena);
    std::memcpy(skill + 96, &table_word, 4);
    const short adjust = 12;
    std::memcpy(table + 88, &adjust, 2);
    const unsigned function_word = 0x7001;
    std::memcpy(table + 92, &function_word, 4);
    const int ranks[] = {-3, -2, 0, 1, 4, 60, 4000};
    const float xs[] = {1.5f, -0.0f};
    for (int rank : ranks)
        for (float x : xs)
            for (int mask = 0; mask < 4; ++mask) {
                soul_ok = mask & 1; light_ok = mask & 2;
                calls.clear();
                const Point3 pt{x, 2.25f, 30.0f};
                const int r = cast_repulse_undead(skill, rank, creature, pt, 99);
                if (r != 0) return 1;
                std::string expected;
                const int step = 0x10000 / (rank + 4);
                int angle = 0;
                do {
                    expected += "cos(" + std::to_string(angle) + ");sin(" + std::to_string(angle) + ");alloc(176);";
                    if (soul_ok) expected += "soul(1," + fl(x + 0.0f) + "," + fl(2.25f) + "," + fl(66.0f) + "," + fl(angle * 0.5f * 450.0f) + "," + fl(angle * 0.25f * 450.0f) + "," + fl(50.0f) + "," + std::to_string(angle) + ",1,1,0);";
                    angle += step;
                } while (!(0xffff < angle));
                expected += "alloc(188);";
                if (light_ok) expected += "light(1," + fl(x + 0.0f) + "," + fl(2.25f) + "," + fl(66.0f) + "," + fl(100.0f) + "," + fl(100.0f) + "," + fl(255.0f) + "," + fl(-96.0f) + "," + fl(0.0f) + "," + fl(0.0f) + "," +
                                          fl(0.0f) + "," + fl(0.0f) + "," + fl(120.0f) + "," + fl(360.0f) + "," + fl(0.0f) + "," + fl(1.0f) + ",1,0);";
                const int A = 0 * 100 + rank * 3 + 5, B = 1 * 100 + rank * 3 + 5;
                expected += "entry28673(" + off(skill + 12) + ",0," + std::to_string(rank) + ");entry28673(" + off(skill + 12) + ",1," + std::to_string(rank) + ");";
                expected += "damage(" + off(creature) + "," + fl(x) + "," + fl(2.25f) + "," + fl(30.0f) + "," + std::to_string(A) + "," + fl(static_cast<float>(B * 12)) + "," + fl(static_cast<float>(B * 24)) + "," +
                            std::to_string(0x0a100900) + ",64,0);lump(fx.lmp,spel_he_repulse.anm);sound(1," + fl(x) + "," + fl(2.25f) + "," + fl(30.0f) + "," + fl(1.0f) + ");";
                if (calls != expected) { std::puts(calls.c_str()); std::puts(expected.c_str()); return 2; }
            }
    return 0;
}
