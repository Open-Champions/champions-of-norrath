// Host contract for cast__16SkillShockOfFireiP8CreatureG6Point3s; not an EE input.
// 32-bit words are offsets from a host base; the three table entries and every callee log.
#define W13_SKILL_CAST_B_HOST 1
#include <cstdio>
#include <cstring>
#include <string>
unsigned char* w13_skill_cast_b_host_base;
static std::string calls;
static void* fake_block;
static unsigned char arena[0x400] __attribute__((aligned(16)));
static std::string fl(float f) { return std::to_string(f); }
static std::string off(const void* p) { return std::to_string(static_cast<const unsigned char*>(p) - arena); }
int w13_skill_cast_b_host_call(unsigned function_word, void* adjusted_skill, int a1, int a2) {
    calls += "entry" + std::to_string(function_word) + "(" + off(adjusted_skill) + "," + std::to_string(a1) + "," + std::to_string(a2) + ");";
    if (function_word == 0x4321) return a1 * 3 + 7;           // the rank adjustment
    return a1 * 100 + a2 * 2;                                  // the ramp evaluation: depends on both index and rank
}
int w13_skill_cast_b_host_call_ramp(unsigned function_word, void* adjusted_skill, int index, int rank, float scale, int zero, int* low, int* high) {
    calls += "ramp" + std::to_string(function_word) + "(" + off(adjusted_skill) + "," + std::to_string(index) + "," + std::to_string(rank) + "," + fl(scale) + "," + std::to_string(zero) + ");";
    *low = rank + 1000;
    *high = rank + 2000;
    return 12345;
}
#include "cast.cpp"
extern "C" {
float icos__Fi(int angle) { calls += "cos(" + std::to_string(angle) + ");"; return angle * 0.5f; }
float isin__Fi(int angle) { calls += "sin(" + std::to_string(angle) + ");"; return angle * 0.25f; }
void* blockAlloc__Fi(int size) { calls += "alloc(" + std::to_string(size) + ");"; return fake_block; }
void sparks_ctor(void* self, Point3 position, Point3 velocity, void* creature, void* first_vag, void* second_vag, int low, int high, int a, int b) {
    calls += "sparks(" + std::to_string(self == fake_block) + "," + fl(position.x) + "," + fl(position.y) + "," + fl(position.z) + "," + fl(velocity.x) + "," + fl(velocity.y) + "," + fl(velocity.z) + "," +
             off(creature) + "," + std::to_string(first_vag == nullptr) + "," + std::to_string(second_vag == nullptr) + "," + std::to_string(low) + "," + std::to_string(high) + "," +
             std::to_string(a) + "," + std::to_string(b) + ");";
}
}
int main() {
    w13_skill_cast_b_host_base = arena;
    unsigned char* const skill = arena + 0x40;
    unsigned char* const creature = arena + 0x200;
    unsigned char* const table = arena + 0x300;
    const unsigned table_word = static_cast<unsigned>(table - arena);
    std::memcpy(skill + 96, &table_word, 4);
    const short adjusts[3] = {-8, 4, -12};  // byte 72, 88, 120 entries
    const unsigned fns[3] = {0x5001, 0x7001, 0x4321};
    const unsigned offs[3] = {72, 88, 120};
    for (int i = 0; i < 3; ++i) { std::memcpy(table + offs[i], &adjusts[i], 2); std::memcpy(table + offs[i] + 4, &fns[i], 4); }
    const int ranks[] = {-3, 0, 1, 99};
    const short angles[] = {0, 5, -7, 32767, -32768};
    for (int rank : ranks)
        for (short angle : angles)
            for (int alloc_ok = 0; alloc_ok < 2; ++alloc_ok) {
                fake_block = alloc_ok ? arena + 0x100 : nullptr;
                calls.clear();
                const int r = cast_shock_of_fire(skill, rank, creature, Point3{1.5f, -2.0f, 30.25f}, angle);
                if (r != 0) return 1;
                const int R = rank * 3 + 7;
                const int a = 1 * 100 + R * 2, b = 2 * 100 + R * 2, c = 3 * 100 + R * 2;
                const float speed = static_cast<float>(c);
                std::string expected = "entry17185(" + off(skill - 12) + "," + std::to_string(rank) + ",20);ramp20481(" + off(skill - 8) + ",0," + std::to_string(R) + "," + fl(0x1.999998p-3f) + ",0);" +
                                       "entry28673(" + off(skill + 4) + ",1," + std::to_string(R) + ");entry28673(" + off(skill + 4) + ",2," + std::to_string(R) + ");entry28673(" + off(skill + 4) + ",3," + std::to_string(R) + ");" +
                                       "cos(" + std::to_string(angle) + ");sin(" + std::to_string(angle) + ");alloc(184);";
                if (alloc_ok) expected += "sparks(1," + fl(1.5f) + "," + fl(-2.0f) + "," + fl(30.25f) + "," + fl(angle * 0.5f * speed) + "," + fl(angle * 0.25f * speed) + "," + fl(0.0f) + "," +
                                          off(creature) + ",1,1," + std::to_string(R + 1000) + "," + std::to_string(R + 2000) + "," + std::to_string(a) + "," + std::to_string(b * 12) + ");";
                if (calls != expected) { std::puts(calls.c_str()); std::puts(expected.c_str()); return 2; }
            }
    return 0;
}
