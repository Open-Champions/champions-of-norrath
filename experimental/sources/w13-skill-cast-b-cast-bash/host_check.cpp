// Host contract for cast__9SkillBashiP8CreatureG6Point3s; not an EE input.
// 32-bit words are offsets from a host base; the table call and the callees log.
#define W13_SKILL_CAST_B_HOST 1
#include <cstdio>
#include <cstring>
#include <string>
unsigned char* w13_skill_cast_b_host_base;
static std::string calls;
static void* fake_block;
static int tweak_result;
static char sound_marker;
int w13_skill_cast_b_host_call(unsigned function_word, void* adjusted_skill, int rank, int constant) {
    calls += "tweek(" + std::to_string(function_word) + "," + std::to_string(static_cast<unsigned char*>(adjusted_skill) - w13_skill_cast_b_host_base) + "," +
             std::to_string(rank) + "," + std::to_string(constant) + ");";
    return tweak_result;
}
#include "cast.cpp"
extern "C" {
void* lumpFindResource__FPcT0(char* lump, char* name) { calls += std::string("lump(") + lump + "," + name + ");"; return &sound_marker; }
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* sound, void* object, float volume) {
    calls += "sound(" + std::to_string(sound == &sound_marker) + "," + std::to_string(static_cast<unsigned char*>(object) - w13_skill_cast_b_host_base) + "," + std::to_string(volume) + ");";
}
void* blockAlloc__Fi(int size) { calls += "alloc(" + std::to_string(size) + ");"; return fake_block; }
void spell_ctor_bash(void* self, int rank, void* creature, void* skill) {
    calls += "ctor(" + std::to_string(self == fake_block) + "," + std::to_string(rank) + "," + std::to_string(static_cast<unsigned char*>(creature) - w13_skill_cast_b_host_base) + "," +
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
    for (int rank : ranks) {
        tweak_result = rank * 3 + 7;
        const std::string head = "tweek(17185," + std::to_string(skill - 8 - arena) + "," + std::to_string(rank) + ",19);lump(fx.lmp,spel_bw_bash.anm);sound(1," +
                                 std::to_string(creature - arena) + "," + std::to_string(1.0f) + ");alloc(40);";
        fake_block = arena + 0x100; calls.clear();
        void* r = cast_bash(skill, rank, creature, Point3{1.0f, 2.0f, 3.0f}, 5);
        const std::string expected = head + "ctor(1," + std::to_string(tweak_result) + "," + std::to_string(creature - arena) + "," + std::to_string(skill - arena) + ");";
        if (r != arena + 0x100 || calls != expected) { std::puts(calls.c_str()); return 1; }
        fake_block = nullptr; calls.clear();
        r = cast_bash(skill, rank, creature, Point3{0, 0, 0}, 0);
        if (r != nullptr || calls != head) return 2;
    }
    return 0;
}
