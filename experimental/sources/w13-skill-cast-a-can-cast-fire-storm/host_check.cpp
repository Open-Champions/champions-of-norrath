// Host contract for canCast__14SkillFireStormP8Creature; not an EE input.
// 32-bit words are offsets from a host base; the table call, rank lookup, trig, height lookup are stubs that log.
// Build with -ffp-contract=off so the expected steps use single precision like the source.
#define W13_SKILL_CAST_A_HOST 1
#include <cstdio>
#include <cstring>
#include <string>
unsigned char* w13_skill_cast_a_host_base;
static std::string calls;
static int ramp_result, rank_result;
static float height_result;
static unsigned bits(float v) { unsigned u; std::memcpy(&u, &v, 4); return u; }
int w13_skill_cast_a_host_call(unsigned function_word, void* adjusted_skill, int a1, int a2) {
    calls += "ramp(" + std::to_string(function_word) + "," + std::to_string(static_cast<unsigned char*>(adjusted_skill) - w13_skill_cast_a_host_base) + "," +
             std::to_string(a1) + "," + std::to_string(a2) + ");";
    return ramp_result;
}
#include "can_cast.cpp"
static float cos_of(int a) { return static_cast<float>((a * 37) % 1000) / 1000.0f - 0.4f; }
static float sin_of(int a) { return static_cast<float>((a * 53) % 1000) / 997.0f - 0.7f; }
void* world = reinterpret_cast<void*>(0x1234);
extern "C" {
int playerGetSkillRank__Fii(int player, int skill) { calls += "rank(" + std::to_string(player) + "," + std::to_string(skill) + ");"; return rank_result; }
float icos__Fi(int angle) { calls += "cos(" + std::to_string(angle) + ");"; return cos_of(angle); }
float isin__Fi(int angle) { calls += "sin(" + std::to_string(angle) + ");"; return sin_of(angle); }
float worldFindHeight__FP12_worldHeaderG6Point3f(void* w, Point3 q, float radius) {
    calls += "height(" + std::to_string(w == world) + "," + std::to_string(bits(q.x)) + "," + std::to_string(bits(q.y)) + "," + std::to_string(bits(q.z)) + "," + std::to_string(bits(radius)) + ");";
    return height_result;
}
}
static unsigned char arena[0x1000] __attribute__((aligned(16)));
int main() {
    int bad = 0;
    w13_skill_cast_a_host_base = arena;
    unsigned char* const skill = arena + 0x40;
    unsigned char* const creature = arena + 0x200;
    unsigned char* const table = arena + 0x100;
    const unsigned table_word = static_cast<unsigned>(table - arena);
    std::memcpy(skill + 96, &table_word, 4);
    const short adjust = -8;
    std::memcpy(table + 88, &adjust, 2);
    const unsigned function_word = 0x4321;
    std::memcpy(table + 92, &function_word, 4);
    const float cx = 10.5f, cy = -4.25f, cz = 50.0f;
    const struct { int flags; int index; short angle; int rank; int ramp; float height; int want; } cases[] = {
        {0, 1, 5, 2, 3, 0.0f, 1},        {0x3f, 1, 5, 2, 3, 0.0f, 1},     {-65, 1, 5, 2, 3, 0.0f, 1},
        {0x40, 1, 1234, 3, 5, 100.0f, 1}, {0x40, 0, -5, 0, -3, 122.0f, 1}, {0x40, 2, 32767, 9, 7, 123.0f, 0},
        {0x40, 3, -32768, 1, 4, -22.0f, 1}, {0x40, 3, 0, 1, 4, -23.0f, 0}, {0x41, 2, 77, 9, 2, 122.5f, 1},
        {-1, 1, -1, 1, 1, 1e6f, 0},      {-1, 1, -1, 1, 1, -1e6f, 0},   {0x40, 0, 0, 5, 0, 122.9f, 1},
        {0x40, 0, 0, 5, 0, 123.0f, 0},    {0x40, 0, 0, 5, 0, -22.9f, 1},  {0x40, 0, 0, 5, 0, -23.0f, 0}};
    for (const auto& c : cases) {
        std::memset(creature, 0, 0x400);
        std::memcpy(creature + 12, &c.flags, 4);
        std::memcpy(creature + 960, &c.index, 4);
        std::memcpy(creature + 56, &c.angle, 2);
        std::memcpy(creature + 16, &cx, 4);
        std::memcpy(creature + 20, &cy, 4);
        std::memcpy(creature + 24, &cz, 4);
        rank_result = c.rank; ramp_result = c.ramp; height_result = c.height; calls.clear();
        const int got = can_cast_fire_storm(skill, creature);
        std::string expected;
        if (c.flags & 0x40) {
            const float r = static_cast<float>(c.ramp);
            float qx = cos_of(c.angle) * r * 12.0f;
            float qy = sin_of(c.angle) * r * 12.0f;
            qx = qx + cx;
            qy = qy + cy;
            expected = "rank(" + std::to_string(c.index) + ",7);ramp(17185," + std::to_string(skill - 8 - arena) + ",1," + std::to_string(c.rank) + ");cos(" +
                       std::to_string(c.angle) + ");sin(" + std::to_string(c.angle) + ");height(1," + std::to_string(bits(qx)) + "," + std::to_string(bits(qy)) + ",0," +
                       std::to_string(bits(r)) + ");";
        }
        if (got != c.want || calls != expected) { std::puts(calls.c_str()); bad = 1; }
    }
    return bad;
}
