#include <cstdio>
#include <cstring>
extern "C" int amxHornOmen__FP5__amxPi(void*, int*);
extern "C" unsigned char nmPlayers[4] = {};
extern "C" unsigned char norrath_player_progression_records_view[23984 * 2] = {};
extern "C" unsigned char* norrath_player_skill_catalog_view[63] = {};
static int marker = 1;
static int skill_marker = 2;
static int allocs = 0;
static int constructs = 0;
static int spells = 0;
static void* constructed_skill = nullptr;
static void* spelled = nullptr;
static void* spell_block = nullptr;
static int fail_alloc = 0;
static int shrink = 0;
extern "C" void* blockAlloc__Fi(int bytes) {
    allocs += 1;
    if (bytes != 32 || fail_alloc) return nullptr;
    return &marker;
}
extern "C" void __19SpellHornOmenEffectiP8CreatureP5Skill(void* block, int rank, void* creature, void* skill) {
    constructs += 1;
    if (block != &marker || rank != 1 || creature != &marker) constructs = -100;
    constructed_skill = skill;
}
extern "C" void addSpell__8CreatureP5Spell(void* creature, void* spell) {
    spells += 1;
    spelled = creature;
    spell_block = spell;
    if (shrink) nmPlayers[0] = 1;
}

int main() {
    norrath_player_skill_catalog_view[28] = reinterpret_cast<unsigned char*>(&allocs);
    norrath_player_skill_catalog_view[29] = reinterpret_cast<unsigned char*>(&skill_marker);
    std::memset(nmPlayers, 0, 4);
    if (amxHornOmen__FP5__amxPi(nullptr, nullptr) != 0) return 1;
    if (allocs != 0) return 2;
    nmPlayers[0] = 2;
    *reinterpret_cast<void**>(norrath_player_progression_records_view + 21872) = nullptr;
    *reinterpret_cast<void**>(norrath_player_progression_records_view + 23984 + 21872) = &marker;
    if (amxHornOmen__FP5__amxPi(nullptr, nullptr) != 0) return 3;
    if (allocs != 1 || constructs != 1 || spells != 1) return 4;
    if (constructed_skill != &skill_marker || spelled != &marker || spell_block != &marker) return 5;
    shrink = 1;
    nmPlayers[0] = 2;
    *reinterpret_cast<void**>(norrath_player_progression_records_view + 21872) = &marker;
    *reinterpret_cast<void**>(norrath_player_progression_records_view + 23984 + 21872) = &marker;
    if (amxHornOmen__FP5__amxPi(nullptr, nullptr) != 0) return 6;
    if (allocs != 2 || spells != 2) return 7;
    shrink = 0;
    fail_alloc = 1;
    nmPlayers[0] = 1;
    *reinterpret_cast<void**>(norrath_player_progression_records_view + 21872) = &marker;
    if (amxHornOmen__FP5__amxPi(nullptr, nullptr) != 0) return 8;
    if (spells != 3 || spell_block != nullptr || constructs != 2) return 9;
    std::puts("amxHornOmen passed");
    return 0;
}
