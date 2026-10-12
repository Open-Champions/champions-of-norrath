#include <cstdio>
#include <cstring>
extern "C" int amxPlanarSteel__FP5__amxPi(void*, int*);
extern "C" unsigned char nmPlayers[4] = {};
extern "C" unsigned char norrath_player_progression_records_view[23984 * 2] = {};
extern "C" unsigned char* norrath_player_skill_catalog_view[63] = {};
static int creature_marker = 1;
static int skill_marker = 2;
static int block_marker = 3;
static int allocs = 0;
static int spells = 0;
static void* skill_seen = nullptr;
extern "C" void* blockAlloc__Fi(int bytes) {
    allocs += 1;
    if (bytes != 108) return nullptr;
    return &block_marker;
}
extern "C" void __22SpellPlanarSteelEffectiP8CreatureP5Skill(void*, int rank, void* creature, void* skill) {
    if (rank != 1 || creature != &creature_marker) allocs = -100;
    skill_seen = skill;
}
extern "C" void addSpell__8CreatureP5Spell(void*, void*) { spells += 1; }
static void put_float(unsigned char* bytes, float value) {
    union Bits { unsigned word; float value; } bits;
    bits.value = value;
    std::memcpy(bytes, &bits.word, 4);
}
static float get_float(const unsigned char* bytes) {
    unsigned word = 0;
    std::memcpy(&word, bytes, 4);
    union Bits { unsigned word; float value; } bits;
    bits.word = word;
    return bits.value;
}

int main() {
    norrath_player_skill_catalog_view[29] = reinterpret_cast<unsigned char*>(&skill_marker);
    std::memset(nmPlayers, 0, 4);
    put_float(norrath_player_progression_records_view + 172, 1.f);
    put_float(norrath_player_progression_records_view + 176, 9.f);
    if (amxPlanarSteel__FP5__amxPi(nullptr, nullptr) != 0) return 1;
    if (get_float(norrath_player_progression_records_view + 172) != 1.f || allocs != 0) return 2;
    nmPlayers[0] = 1;
    *reinterpret_cast<void**>(norrath_player_progression_records_view + 21872) = nullptr;
    if (amxPlanarSteel__FP5__amxPi(nullptr, nullptr) != 0) return 3;
    if (get_float(norrath_player_progression_records_view + 172) != 9.f || allocs != 0) return 4;
    put_float(norrath_player_progression_records_view + 172, 0.f);
    *reinterpret_cast<void**>(norrath_player_progression_records_view + 21872) = &creature_marker;
    if (amxPlanarSteel__FP5__amxPi(nullptr, nullptr) != 0) return 5;
    if (get_float(norrath_player_progression_records_view + 172) != 0.f) return 6;
    if (allocs != 1 || spells != 1 || skill_seen != &skill_marker) return 7;
    std::puts("amxPlanarSteel passed");
    return 0;
}
