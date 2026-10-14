// Host contract for cast__18SkillBlindingLightiP8CreatureG6Point3s; not an EE input.
// 32-bit words are offsets from a host base; the table call and every callee log.
#define W13_SKILL_CAST_B_HOST 1
#include <cstdio>
#include <cstring>
#include <string>
unsigned char* w13_skill_cast_b_host_base;
#include "cast.cpp"
static std::string calls;
static unsigned char arena[0x800] __attribute__((aligned(16)));
static void* fake_block;
static int entry_result;
static void* list_first;
static char sound_marker;
static std::string fl(float f) { return std::to_string(f); }
int w13_skill_cast_b_host_call(unsigned, void*, int, int) { return 0; }
int w13_skill_cast_b_host_call_object(unsigned function_word, void* adjusted, void* argument) {
    calls += "entry(" + std::to_string(function_word) + "," + std::to_string(static_cast<unsigned char*>(adjusted) - arena) + "," + std::to_string(static_cast<unsigned char*>(argument) - arena) + ");";
    return entry_result;
}
extern "C" {
void engineSetScreenColor__Fiiii(int a, int b, int c, int d) { calls += "color(" + std::to_string(a) + "," + std::to_string(b) + "," + std::to_string(c) + "," + std::to_string(d) + ");"; }
void objectGetSortedCreatureList__FG6Point3sffP8ListHeadP8Creature(Point3 p, short angle, float first, float second, ListHead* list, void* exclude) {
    calls += "sorted(" + fl(p.x) + "," + fl(p.y) + "," + fl(p.z) + "," + std::to_string(angle) + "," + fl(first) + "," + fl(second) + "," + std::to_string(list->first == nullptr) + "," + std::to_string(exclude == nullptr) + ");";
    list->first = list_first;
}
void* lumpFindResource__FPcT0(char* lump, char* name) { calls += std::string("lump(") + lump + "," + name + ");"; return &sound_marker; }
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* sound, void* object, float volume) {
    calls += "sound(" + std::to_string(sound == &sound_marker) + "," + std::to_string(static_cast<unsigned char*>(object) - arena) + "," + fl(volume) + ");";
}
void* blockAlloc__Fi(int size) { calls += "alloc(" + std::to_string(size) + ");"; return fake_block; }
void effect_ctor(void* self, Point3 p, int rank, void* creature, void* skill) {
    calls += "ctor(" + std::to_string(self == fake_block) + "," + fl(p.x) + "," + fl(p.y) + "," + fl(p.z) + "," + std::to_string(rank) + "," +
             std::to_string(static_cast<unsigned char*>(creature) - arena) + "," + std::to_string(static_cast<unsigned char*>(skill) - arena) + ");";
}
void listDestroy__FP8ListHead(ListHead* list) { calls += std::string("destroy(") + (list->first == list_first ? "same" : "other") + ");"; }
}
static void put32(unsigned char* where, unsigned value) { std::memcpy(where, &value, 4); }
int main() {
    w13_skill_cast_b_host_base = arena;
    unsigned char* const creature = arena + 0x40;
    unsigned char* const skill = arena + 0x500;
    unsigned char* const node = arena + 0x600;
    unsigned char* const target = arena + 0x700;
    const unsigned table = 0x480;
    const short adjust = -16;
    const unsigned function = 0x5555;
    const float tx = 5.5f, ty = -6.25f, tz = 700.0f;
    for (int case_index = 0; case_index < 8; ++case_index) {
        const bool has_node = case_index & 1, passes = case_index & 2, alloc_ok = case_index & 4;
        std::memset(arena, 0x11, sizeof arena);
        put32(creature + 72, table);
        std::memcpy(arena + table + 160, &adjust, 2);
        put32(arena + table + 164, function);
        put32(node + 12, target - arena);
        std::memcpy(target + 16, &tx, 4); std::memcpy(target + 20, &ty, 4); std::memcpy(target + 24, &tz, 4);
        list_first = has_node ? node : nullptr;
        entry_result = passes ? 9 : 0;
        fake_block = alloc_ok ? arena + 0x300 : nullptr;
        calls.clear();
        const int rank = case_index - 3;
        const int r = cast_blinding_light(skill, rank, creature, Point3{1.5f, 2.5f, 3.5f}, -77);
        if (r != 0) return 1;
        short group[8];
        std::memcpy(group, creature + 140, 16);
        const short want[8] = {86, 128, 250, 110, 86, 128, 250, 110};
        if (std::memcmp(group, want, 16) != 0) return 2;
        std::string expected = "color(128,128,128,0);sorted(" + fl(1.5f) + "," + fl(2.5f) + "," + fl(3.5f) + ",-77," + fl(360.0f) + "," + fl(90.0f) + ",1,1);lump(fx.lmp,spel_he_blindlight.anm);sound(1," +
                               std::to_string(creature - arena) + "," + fl(1.0f) + ");";
        if (has_node) {
            expected += "entry(21845," + std::to_string(creature - 16 - arena) + "," + std::to_string(target - arena) + ");";
            if (passes) {
                expected += "alloc(84);";
                if (alloc_ok) expected += "ctor(1," + fl(tx) + "," + fl(ty) + "," + fl(tz) + "," + std::to_string(rank) + "," + std::to_string(creature - arena) + "," + std::to_string(skill - arena) + ");";
            }
        }
        expected += "destroy(same);";
        if (calls != expected) { std::puts(calls.c_str()); std::puts(expected.c_str()); return 4; }
    }
    // null creature is only reachable after the halfword stores, which dereference it, so it is not exercised.
    return 0;
}
