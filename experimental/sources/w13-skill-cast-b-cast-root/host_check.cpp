// Host contract for cast__9SkillRootiP8CreatureG6Point3s; not an EE input.
// 32-bit words are offsets from a host base; the table entries, the node list and every callee log.
#define W13_SKILL_CAST_B_HOST 1
#include <cstdio>
#include <cstring>
#include <string>
unsigned char* w13_skill_cast_b_host_base;
static std::string calls;
static unsigned char arena[0x1000] __attribute__((aligned(16)));
static void* list_first;
static void* fake_block;
static int limit_result;
static int entry_passes[8];
static int boss_flags[8];
static char sound_marker;
static std::string fl(float f) { return std::to_string(f); }
static std::string off(const void* p) { return p ? std::to_string(static_cast<const unsigned char*>(p) - arena) : std::string("null"); }
static int target_index(const void* p) { return static_cast<int>((static_cast<const unsigned char*>(p) - (arena + 0x800)) / 0x100); }
int w13_skill_cast_b_host_call(unsigned function_word, void* adjusted_skill, int a1, int a2) {
    calls += "entry" + std::to_string(function_word) + "(" + off(adjusted_skill) + "," + std::to_string(a1) + "," + std::to_string(a2) + ");";
    return function_word == 0x4321 ? a1 * 3 + 7 : limit_result;
}
int w13_skill_cast_b_host_call_object(unsigned function_word, void* adjusted, void* argument) {
    calls += "filter" + std::to_string(function_word) + "(" + off(adjusted) + "," + std::to_string(target_index(argument)) + ");";
    return entry_passes[target_index(argument)];
}
int w13_skill_cast_b_host_call_ramp(unsigned, void*, int, int, float, int, int*, int*) { return 0; }
#include "cast.cpp"
extern "C" {
void* lumpFindResource__FPcT0(char* lump, char* name) { calls += std::string("lump(") + lump + "," + name + ");"; return &sound_marker; }
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* sound, void* object, float volume) {
    calls += "sound(" + std::to_string(sound == &sound_marker) + "," + off(object) + "," + fl(volume) + ");";
}
void objectGetSortedCreatureList__FG6Point3sffP8ListHeadP8Creature(Point3 p, short angle, float first, float second, ListHead* list, void* exclude) {
    calls += "sorted(" + fl(p.x) + "," + fl(p.y) + "," + fl(p.z) + "," + std::to_string(angle) + "," + fl(first) + "," + fl(second) + "," + std::to_string(list->first == nullptr) + "," + std::to_string(exclude == nullptr) + ");";
    list->first = list_first;
}
int isBoss__FP10GameObject(void* object) { calls += "boss(" + std::to_string(target_index(object)) + ");"; return boss_flags[target_index(object)]; }
void* blockAlloc__Fi(int size) { calls += "alloc(" + std::to_string(size) + ");"; return fake_block; }
void spell_ctor_root(void* self, int rank, void* creature, void* skill) {
    calls += "ctor(" + std::to_string(self == fake_block) + "," + std::to_string(rank) + "," + std::to_string(target_index(creature)) + "," + off(skill) + ");";
}
void addSpell__8CreatureP5Spell(void* creature, void* spell) { calls += "add(" + std::to_string(target_index(creature)) + "," + std::to_string(spell != nullptr && spell == fake_block) + ");"; }
void listDestroy__FP8ListHead(ListHead* list) { calls += std::string("destroy(") + (list->first != nullptr ? "set" : "unset") + ");"; }
}
static void put32(unsigned char* where, unsigned value) { std::memcpy(where, &value, 4); }
int main() {
    w13_skill_cast_b_host_base = arena;
    unsigned char* const skill = arena + 0x40;
    unsigned char* const creature = arena + 0x200;
    unsigned char* const table = arena + 0x600;
    const unsigned table_word = static_cast<unsigned>(table - arena);
    const int limits[] = {-1, 0, 1, 2, 5};
    unsigned seed = 12345;
    for (int trial = 0; trial < 4000; ++trial) {
        seed = seed * 1103515245u + 12345u;
        const int count = (seed >> 8) % 5;
        const bool null_creature = (seed >> 11) % 5 == 0, alloc_ok = (seed >> 13) & 1;
        limit_result = limits[(seed >> 15) % 5];
        const int rank = static_cast<int>((seed >> 17) % 7) - 3;
        const short angle = static_cast<short>(seed >> 20);
        std::memset(arena, 0x11, sizeof arena);
        put32(skill + 96, table_word);
        const short adjusts[3] = {-12, 4, -16};
        put32(table + 120, 0); std::memcpy(table + 120, &adjusts[0], 2); put32(table + 124, 0x4321);
        put32(table + 88, 0); std::memcpy(table + 88, &adjusts[1], 2); put32(table + 92, 0x7001);
        put32(creature + 72, 0x700);
        put32(arena + 0x700 + 160, 0); std::memcpy(arena + 0x700 + 160, &adjusts[2], 2); put32(arena + 0x700 + 164, 0x5555);
        // targets 0..3 live at arena+0x800+0x100*i; nodes at arena+0x180+0x20*i? use arena+0x900.. region offsets below
        unsigned char* nodes[5];
        for (int i = 0; i < 5; ++i) nodes[i] = arena + 0x400 + 0x20 * i;
        for (int i = 0; i < count; ++i) {
            unsigned char* target = arena + 0x800 + 0x100 * i;
            put32(nodes[i], i + 1 < count ? static_cast<unsigned>(nodes[i + 1] - arena) : 0);
            put32(nodes[i] + 12, static_cast<unsigned>(target - arena));
            const unsigned bits = seed >> (3 + 2 * i);
            put32(target + 12, (bits & 1) ? 0x01000000u : 0u);
            const short hw = (bits & 2) ? 3 : ((bits & 8) ? -2 : 0);
            std::memcpy(target + 794, &hw, 2);
            entry_passes[i] = (seed >> (7 + i)) & 1;
            boss_flags[i] = (seed >> (9 + i)) & 1;
        }
        list_first = count ? static_cast<void*>(nodes[0]) : nullptr;
        fake_block = alloc_ok ? arena + 0x300 : nullptr;
        unsigned char* const used_creature = null_creature ? nullptr : creature;
        calls.clear();
        const int r = cast_root(skill, rank, used_creature, Point3{1.5f, 2.5f, 3.5f}, angle);
        if (r != 0) return 1;
        const int R = rank * 3 + 7;
        std::string expected = "entry17185(" + off(skill - 12) + "," + std::to_string(rank) + ",20);lump(fx.lmp,spel_ew_shock.anm);sound(1," + off(used_creature) + "," + fl(1.0f) + ");" +
                               "entry28673(" + off(skill + 4) + ",0," + std::to_string(R) + ");sorted(" + fl(1.5f) + "," + fl(2.5f) + "," + fl(3.5f) + "," + std::to_string(angle) + "," + fl(248.0f) + "," + fl(120.0f) + ",1,1);";
        int applied = 0;
        if (count > 0 && limit_result > 0) {
            for (int i = 0; i < count; ++i) {
                unsigned char* target = arena + 0x800 + 0x100 * i;
                bool skip = false;
                if (!null_creature) {
                    expected += "filter21845(" + off(creature - 16) + "," + std::to_string(i) + ");";
                    if (!entry_passes[i]) skip = true;
                }
                unsigned flags; std::memcpy(&flags, target + 12, 4);
                short hw; std::memcpy(&hw, target + 794, 2);
                if (!skip) {
                    if (flags & 0x01000000) skip = true;
                    else {
                        expected += "boss(" + std::to_string(i) + ");";
                        if (boss_flags[i]) skip = true;
                        else if (hw > 0) skip = true;
                    }
                }
                if (!skip) {
                    expected += "alloc(40);";
                    if (alloc_ok) expected += "ctor(1," + std::to_string(R) + "," + std::to_string(i) + "," + off(skill) + ");";
                    expected += "add(" + std::to_string(i) + "," + std::to_string(alloc_ok) + ");";
                    ++applied;
                }
                if (applied >= limit_result) break;
            }
        }
        expected += std::string("destroy(") + (count ? "set" : "unset") + ");destroy(unset);";
        if (calls != expected) { std::puts(calls.c_str()); std::puts(expected.c_str()); return 2; }
    }
    return 0;
}
