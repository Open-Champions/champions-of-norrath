// Host contract for cast__15SkillPoisonRainiP8CreatureG6Point3s; not an EE input.
// 32-bit words are offsets from a host base; the table call, the trig and height lookups and the callees are stubs that
// log. Build with -ffp-contract=off so the expected values use the same single-precision steps as the source.
#define W13_SKILL_CAST_A_HOST 1
#include <cstdio>
#include <cstring>
#include <string>
unsigned char* w13_skill_cast_a_host_base;
static std::string calls;
static void* fake_block;
static int ramp_result;
static unsigned bits(float v) { unsigned u; std::memcpy(&u, &v, 4); return u; }
int w13_skill_cast_a_host_call3(unsigned function_word, void* adjusted_skill, int a1, int a2, void* a3) {
    calls += "ramp(" + std::to_string(function_word) + "," + std::to_string(static_cast<unsigned char*>(adjusted_skill) - w13_skill_cast_a_host_base) + "," +
             std::to_string(a1) + "," + std::to_string(a2) + "," + std::to_string(static_cast<unsigned char*>(a3) - w13_skill_cast_a_host_base) + ");";
    return ramp_result;
}
#include "cast.cpp"
static float cos_of(int a) { return static_cast<float>((a * 37) % 1000) / 1000.0f - 0.4f; }
static float sin_of(int a) { return static_cast<float>((a * 53) % 1000) / 997.0f - 0.7f; }
static float height_of(Point3 q, float radius) { return q.x * 0.25f + radius; }
void* world = reinterpret_cast<void*>(0x1234);
extern "C" {
void* blockAlloc__Fi(int size) { calls += "alloc(" + std::to_string(size) + ");"; return fake_block; }
float icos__Fi(int angle) { calls += "cos(" + std::to_string(angle) + ");"; return cos_of(angle); }
float isin__Fi(int angle) { calls += "sin(" + std::to_string(angle) + ");"; return sin_of(angle); }
float worldFindHeight__FP12_worldHeaderG6Point3f(void* w, Point3 q, float radius) {
    calls += "height(" + std::to_string(w == world) + "," + std::to_string(bits(q.x)) + "," + std::to_string(bits(q.y)) + "," + std::to_string(bits(q.z)) + "," + std::to_string(bits(radius)) + ");";
    return height_of(q, radius);
}
void rain_ctor_poison_rain(void* self, int rank, void* creature, Point3 q, void* skill, void* animation) {
    calls += "ctor(" + std::to_string(self == fake_block) + "," + std::to_string(rank) + "," + std::to_string(static_cast<unsigned char*>(creature) - w13_skill_cast_a_host_base) + "," +
             std::to_string(bits(q.x)) + "," + std::to_string(bits(q.y)) + "," + std::to_string(bits(q.z)) + "," + std::to_string(static_cast<unsigned char*>(skill) - w13_skill_cast_a_host_base) + "," + std::to_string(animation == nullptr) + ");";
}
}
static unsigned char arena[0x400] __attribute__((aligned(16)));
int main() {
    int bad = 0;
    w13_skill_cast_a_host_base = arena;
    unsigned char* const skill = arena + 0x40;
    unsigned char* const creature = arena + 0x200;
    unsigned char* const table = arena + 0x300;
    const unsigned table_word = static_cast<unsigned>(table - arena);
    std::memcpy(skill + 96, &table_word, 4);
    const short adjust = -8;
    std::memcpy(table + 104, &adjust, 2);
    const unsigned function_word = 0x4321;
    std::memcpy(table + 108, &function_word, 4);
    const Point3 points[] = {{1.5f, -2.25f, 30.0f}, {0, 0, 0}, {-1000.125f, 77.5f, 5.0f}};
    for (int rank : {-3, 0, 1, 99})
        for (short angle : {static_cast<short>(-32768), static_cast<short>(-1), static_cast<short>(0), static_cast<short>(1234), static_cast<short>(32767)})
            for (const Point3& p : points) {
                ramp_result = rank * 3 + 7;
                const float r = static_cast<float>(ramp_result);
                float qx = cos_of(angle) * r * 12.0f;
                float qy = sin_of(angle) * r * 12.0f;
                qx = qx + p.x;
                qy = qy + p.y;
                Point3 q{qx, qy, 0.0f};
                const float h = height_of(q, r);
                q.z = 0.0f + h + 12.0f;
                fake_block = arena + 0x100; calls.clear();
                if (cast_poison_rain(skill, rank, creature, p, angle) != 0) bad = 1;
                const std::string expected = "ramp(17185," + std::to_string(skill - 8 - arena) + ",1," + std::to_string(rank) + "," + std::to_string(creature - arena) + ");cos(" + std::to_string(angle) + ");sin(" +
                                             std::to_string(angle) + ");height(1," + std::to_string(bits(q.x)) + "," + std::to_string(bits(q.y)) + ",0," + std::to_string(bits(r)) +
                                             ");alloc(108);ctor(1," + std::to_string(rank) + "," + std::to_string(creature - arena) + "," + std::to_string(bits(q.x)) + "," +
                                             std::to_string(bits(q.y)) + "," + std::to_string(bits(q.z)) + "," + std::to_string(skill - arena) + ",1);";
                if (calls != expected) { std::puts(calls.c_str()); std::puts(expected.c_str()); bad = 2; }
                fake_block = nullptr; calls.clear();
                if (cast_poison_rain(skill, rank, creature, p, angle) != 0 || calls.find("alloc(108);") == std::string::npos || calls.find("ctor") != std::string::npos) bad = 3;
            }
    return bad;
}
