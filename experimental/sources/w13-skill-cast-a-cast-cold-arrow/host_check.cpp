// Host contract for cast__14SkillColdArrowiP8CreatureG6Point3s; not an EE input.
// 32-bit words are offsets from a host base; the table call and the callees log.
#define W13_SKILL_CAST_A_HOST 1
#include <cstdio>
#include <cstring>
#include <string>
unsigned char* w13_skill_cast_a_host_base;
static std::string calls;
static void* fake_block;
static int tweak_result;
int w13_skill_cast_a_host_call(unsigned function_word, void* adjusted_skill, int rank, int constant) {
    calls += "tweek(" + std::to_string(function_word) + "," + std::to_string(static_cast<unsigned char*>(adjusted_skill) - w13_skill_cast_a_host_base) + "," +
             std::to_string(rank) + "," + std::to_string(constant) + ");";
    return tweak_result;
}
#include "cast.cpp"
extern "C" {
void* blockAlloc__Fi(int size) { calls += "alloc(" + std::to_string(size) + ");"; return fake_block; }
void projectile_ctor_cold_arrow(void* self, Point3 point, short angle, void* creature, int rank, void* skill) {
    calls += "ctor(" + std::to_string(self == fake_block) + "," + std::to_string(point.x) + "," + std::to_string(point.y) + "," + std::to_string(point.z) + "," +
             std::to_string(angle) + "," + std::to_string(static_cast<unsigned char*>(creature) - w13_skill_cast_a_host_base) + "," + std::to_string(rank) + "," +
             std::to_string(static_cast<unsigned char*>(skill) - w13_skill_cast_a_host_base) + ");";
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
    std::memcpy(table + 120, &adjust, 2);
    const unsigned function_word = 0x4321;
    std::memcpy(table + 124, &function_word, 4);
    for (int rank : {-3, 0, 1, 99})
        for (short angle : {static_cast<short>(-32768), static_cast<short>(-1), static_cast<short>(0), static_cast<short>(32767)}) {
            tweak_result = rank * 3 + 7;
            fake_block = arena + 0x100; calls.clear();
            if (cast_cold_arrow(skill, rank, creature, Point3{1.5f, -2.0f, 30.25f}, angle) != 0) bad = 1;
            const std::string expected = "alloc(176);tweek(17185," + std::to_string(skill - 8 - arena) + "," + std::to_string(rank) + ",20);ctor(1,1.500000,-2.000000,18.250000," +
                                         std::to_string(angle) + "," + std::to_string(creature - arena) + "," + std::to_string(tweak_result) + "," + std::to_string(skill - arena) + ");";
            if (calls != expected) { std::puts(calls.c_str()); bad = 2; }
            fake_block = nullptr; calls.clear();
            if (cast_cold_arrow(skill, rank, creature, Point3{0, 0, 0}, angle) != 0 || calls != "alloc(176);") bad = 3;
        }
    return bad;
}
