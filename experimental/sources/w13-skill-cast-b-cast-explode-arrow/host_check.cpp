// Host contract for cast__17SkillExplodeArrowiP8CreatureG6Point3s; not an EE input.
// 32-bit words are offsets from a host base; the table call and the callees log.
#define W13_SKILL_CAST_B_HOST 1
#include <cstdio>
#include <cstring>
#include <string>
unsigned char* w13_skill_cast_b_host_base;
static std::string calls;
static void* fake_block;
static int tweak_result;
int w13_skill_cast_b_host_call(unsigned function_word, void* adjusted_skill, int rank, int constant) {
    calls += "tweek(" + std::to_string(function_word) + "," + std::to_string(static_cast<unsigned char*>(adjusted_skill) - w13_skill_cast_b_host_base) + "," +
             std::to_string(rank) + "," + std::to_string(constant) + ");";
    return tweak_result;
}
#include "cast.cpp"
extern "C" {
void* blockAlloc__Fi(int size) { calls += "alloc(" + std::to_string(size) + ");"; return fake_block; }
void projectile_ctor(void* self, Point3 point, short angle, void* creature, int rank, void* skill) {
    calls += "ctor(" + std::to_string(self == fake_block) + "," + std::to_string(point.x) + "," + std::to_string(point.y) + "," + std::to_string(point.z) + "," +
             std::to_string(angle) + "," + std::to_string(static_cast<unsigned char*>(creature) - w13_skill_cast_b_host_base) + "," + std::to_string(rank) + "," +
             std::to_string(static_cast<unsigned char*>(skill) - w13_skill_cast_b_host_base) + ");";
}
}
static unsigned char arena[0x400] __attribute__((aligned(16)));
int main() {
    w13_skill_cast_b_host_base = arena;
    unsigned char* const skill = arena + 0x40;
    unsigned char* const creature = arena + 0x200;
    unsigned char* const table = arena + 0x300;
    const unsigned table_word = static_cast<unsigned>(table - arena);
    std::memcpy(skill + 96, &table_word, 4);
    const short adjust = -8;
    std::memcpy(table + 120, &adjust, 2);
    const unsigned function_word = 0x4321;
    std::memcpy(table + 124, &function_word, 4);
    const int ranks[] = {-3, 0, 1, 99};
    const short angles[] = {0, 5, -7, 32767, -32768};
    const std::string tw = "tweek(17185," + std::to_string(skill - 8 - arena);
    for (int rank : ranks)
        for (short angle : angles) {
            tweak_result = rank * 3 + 7;
            fake_block = arena + 0x100; calls.clear();
            int r = cast_arrow(skill, rank, creature, Point3{1.5f, -2.0f, 30.25f}, angle);
            const std::string ctor = "ctor(1," + std::to_string(1.5f) + "," + std::to_string(-2.0f) + "," + std::to_string(18.25f) + "," + std::to_string(angle) + "," +
                                     std::to_string(creature - arena) + "," + std::to_string(tweak_result) + "," + std::to_string(skill - arena) + ");";
            const std::string call = tw + "," + std::to_string(rank) + ",9);";
            const std::string expected = tw + "," + std::to_string(rank) + ",9);alloc(116);" + ctor;
            if (r != 0 || calls != expected) { std::puts(calls.c_str()); return 1; }
            fake_block = nullptr; calls.clear();
            r = cast_arrow(skill, rank, creature, Point3{0, 0, 0}, angle);
            if (r != 0 || calls != tw + "," + std::to_string(rank) + ",9);alloc(116);") return 2;
        }
    return 0;
}
