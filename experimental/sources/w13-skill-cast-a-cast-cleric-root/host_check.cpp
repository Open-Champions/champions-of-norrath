// Host contract for cast__15SkillClericRootiP8CreatureG6Point3s; not an EE input.
// 32-bit words are offsets from a host base arena (list links, target words and table words). The table calls, list
// fill, boss test, spell constructor and addSpell are stubs that log.
#define W13_SKILL_CAST_A_HOST 1
#include <cstdio>
#include <cstring>
#include <string>
unsigned char* w13_skill_cast_a_host_base;
static std::string calls;
static int tweak_result, limit_result;
static unsigned char* skill_ptr;
static unsigned long off(const void* p) { return p ? static_cast<unsigned long>(static_cast<const unsigned char*>(p) - w13_skill_cast_a_host_base) : 0; }
int w13_skill_cast_a_host_call(unsigned function_word, void* adjusted_skill, int a1, int a2) {
    calls += "entry(" + std::to_string(function_word) + "," + std::to_string(off(adjusted_skill)) + "," + std::to_string(a1) + "," + std::to_string(a2) + ");";
    return a1 == 0 ? limit_result : tweak_result;
}
static bool filter_pass[4];
static bool boss[4];
unsigned char* targets[4];
int w13_skill_cast_a_host_call_object(unsigned function_word, void* adjusted_object, void* a1) {
    int index = -1;
    for (int i = 0; i < 4; ++i) if (a1 == targets[i]) index = i;
    calls += "filter(" + std::to_string(function_word) + "," + std::to_string(off(adjusted_object)) + "," + std::to_string(index) + ");";
    return filter_pass[index] ? 1 : 0;
}
#include "cast.cpp"
static unsigned fill_node_count;
static unsigned fill_first;
extern "C" {
void* blockAlloc__Fi(int size);
}
static unsigned char arena[0x4000] __attribute__((aligned(16)));
static unsigned char* fake_blocks[4];
static int alloc_index;
extern "C" {
void* blockAlloc__Fi(int size) { calls += "alloc(" + std::to_string(size) + ");"; return fake_blocks[alloc_index++]; }
int objectGetSortedCreatureList__FG6Point3sffP8ListHeadP8Creature(Point3 center, short angle, float a, float b, ListHead* list, void* exclude) {
    calls += "list(" + std::to_string(center.x) + "," + std::to_string(center.y) + "," + std::to_string(center.z) + "," + std::to_string(angle) + "," + std::to_string(a) + "," +
             std::to_string(b) + "," + std::to_string(list->first) + "," + std::to_string(off(exclude)) + ");";
    list->first = fill_first;
    return 0;
}
void listDestroy__FP8ListHead(ListHead* list) { calls += std::string("destroy(") + (list->first ? "filled" : "empty") + ");"; }
int isBoss__FP10GameObject(void* object) {
    for (int i = 0; i < 4; ++i) if (object == targets[i]) { calls += "boss(" + std::to_string(i) + ");"; return boss[i] ? 1 : 0; }
    return -1;
}
void addSpell__8CreatureP5Spell(void* creature, void* spell) {
    int index = -1;
    for (int i = 0; i < 4; ++i) if (creature == targets[i]) index = i;
    calls += "add(" + std::to_string(index) + "," + std::to_string(off(spell)) + ");";
}
void spell_ctor_cleric_root(void* self, int rank, void* creature, void* skill) {
    int index = -1;
    for (int i = 0; i < 4; ++i) if (creature == targets[i]) index = i;
    calls += "ctor(" + std::to_string(off(self)) + "," + std::to_string(rank) + "," + std::to_string(index) + "," + std::to_string(off(skill)) + ");";
}
}
struct Target { unsigned flags; short halfword; bool boss; bool pass; };
int main() {
    int bad = 0;
    w13_skill_cast_a_host_base = arena;
    unsigned char* const skill = arena + 0x40;
    unsigned char* const table = arena + 0x100;
    unsigned char* const caster = arena + 0x200;
    unsigned char* const caster_table = arena + 0x300;
    unsigned char* const nodes = arena + 0x400;       // 4 nodes of 0x20 bytes
    for (int i = 0; i < 4; ++i) { targets[i] = arena + 0x1000 + 0x400 * i; fake_blocks[i] = arena + 0x600 + 0x40 * i; }
    unsigned word = static_cast<unsigned>(table - arena);
    std::memcpy(skill + 96, &word, 4);
    short adjust = -8;
    std::memcpy(table + 120, &adjust, 2);
    word = 0x4321; std::memcpy(table + 124, &word, 4);
    adjust = -4;
    std::memcpy(table + 88, &adjust, 2);
    word = 0x5432; std::memcpy(table + 92, &word, 4);
    word = static_cast<unsigned>(caster_table - arena);
    std::memcpy(caster + 72, &word, 4);
    adjust = 12;
    std::memcpy(caster_table + 160, &adjust, 2);
    word = 0x6543; std::memcpy(caster_table + 164, &word, 4);
    const Target ok{0, 0, false, true};
    const struct { int rank; int tweaked; int limit; int node_count; bool use_caster; Target t[4]; int allocs_null_from; } cases[] = {
        {5, 9, 2, 3, true, {ok, ok, ok, ok}, 9},
        {0, 3, 3, 3, true, {{0x01000000, 0, false, true}, {0, 1, false, true}, ok, ok}, 9},
        {7, 2, 5, 3, false, {{0, 0, true, true}, {0, -1, false, false}, {static_cast<unsigned>(~0x01000000), 0, false, true}, ok}, 9},
        {-1, 4, 2, 3, true, {ok, ok, ok, ok}, 0},
        {1, 4, 0, 2, true, {ok, ok, ok, ok}, 9},
        {1, 4, -3, 2, true, {ok, ok, ok, ok}, 9},
        {2, 6, 4, 0, true, {ok, ok, ok, ok}, 9},
        {3, 6, 4, 4, true, {{0, 0, false, false}, {0, 0, true, true}, {0x01000000, 5, true, false}, {0, 32767, false, true}}, 9},
        {3, 6, 1, 3, false, {{0, 32767, false, true}, {0, -32768, false, true}, ok, ok}, 9}};
    for (const auto& c : cases) {
        for (int i = 0; i < 4; ++i) {
            std::memset(targets[i], 0x33, 0x340);
            std::memcpy(targets[i] + 12, &c.t[i].flags, 4);
            std::memcpy(targets[i] + 794, &c.t[i].halfword, 2);
            filter_pass[i] = c.t[i].pass; boss[i] = c.t[i].boss;
        }
        for (int i = 0; i < c.node_count; ++i) {
            const unsigned next = i + 1 < c.node_count ? static_cast<unsigned>(nodes + 0x20 * (i + 1) - arena) : 0;
            const unsigned target = static_cast<unsigned>(targets[i] - arena);
            std::memset(nodes + 0x20 * i, 0x77, 0x20);
            std::memcpy(nodes + 0x20 * i, &next, 4);
            std::memcpy(nodes + 0x20 * i + 12, &target, 4);
        }
        fill_first = c.node_count ? static_cast<unsigned>(nodes - arena) : 0;
        tweak_result = c.tweaked; limit_result = c.limit; alloc_index = 0;
        if (c.allocs_null_from == 0) { for (auto& b : fake_blocks) b = nullptr; } else { for (int i = 0; i < 4; ++i) fake_blocks[i] = arena + 0x600 + 0x40 * i; }
        calls.clear();
        void* const who = c.use_caster ? static_cast<void*>(caster) : nullptr;
        if (cast_cleric_root(skill, c.rank, who, Point3{1.5f, -2.5f, 3.5f}, 1234) != 0) bad = 1;
        std::string expected = "entry(17185," + std::to_string(skill - 8 - arena) + "," + std::to_string(c.rank) + ",20);entry(21554," + std::to_string(skill - 4 - arena) +
                               ",0," + std::to_string(c.tweaked) + ");list(1.500000,-2.500000,3.500000,1234,248.000000,120.000000,0," + "0);";
        int started = 0, ai = 0;
        if (c.node_count && c.limit > 0) {
            for (int i = 0; i < c.node_count; ++i) {
                bool take = true;
                if (c.use_caster) { expected += "filter(25923," + std::to_string(caster + 12 - arena) + "," + std::to_string(i) + ");"; take = c.t[i].pass; }
                if (take && (c.t[i].flags & 0x01000000)) take = false;
                if (take) { expected += "boss(" + std::to_string(i) + ");"; if (c.t[i].boss) take = false; }
                if (take && c.t[i].halfword > 0) take = false;
                if (take) {
                    expected += "alloc(40);";
                    const bool have = c.allocs_null_from != 0;
                    if (have) expected += "ctor(" + std::to_string(fake_blocks[ai] - arena) + "," + std::to_string(c.tweaked) + "," + std::to_string(i) + "," + std::to_string(skill - arena) + ");";
                    expected += "add(" + std::to_string(i) + "," + std::to_string(have ? fake_blocks[ai] - arena : 0) + ");";
                    ++ai; ++started;
                }
                if (started >= c.limit) break;
            }
        }
        expected += "destroy(" + std::string(c.node_count ? "filled" : "empty") + ");destroy(empty);";
        if (calls != expected) { std::puts(calls.c_str()); std::puts(expected.c_str()); bad = 2; }
    }
    return bad;
}
