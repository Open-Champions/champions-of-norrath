// Host check for playerRecomputeDamage__6PlayerPQ24item11descriptioni. Not an EE plan input.
// Build: c++ -std=c++17 -Wall -Wextra -DNORRATH_HOST_ARENA host_check.cpp
#include <cstdio>
#include <cstring>
#include <map>
#include "../w9-animation-object-c-common/host_arena.hpp"
#include "player_recompute_damage.cpp"

unsigned char player[23984 * 4];
unsigned char gSkillTree[40 + 3592 * 3 + 64];
unsigned char skills[256];
int playersAlive[4];
int nmPlayersAlive;

static int rangeLow, rangeHigh, bonus, scaleRank;
static float scaleValue;
static std::map<int, int> ranks;
static std::map<void*, void*> spells;
static std::map<void*, float> strengths;
static std::map<void*, int> insides;
static int rangeCalls, flagSeen;
extern "C" {
void getDamageRangeAll__4itemPQ24item11descriptionPiT2i(const unsigned char*, int* low, int* high, int flag) { ++rangeCalls; flagSeen = flag; *low = rangeLow; *high = rangeHigh; }
int getAttackBonus__4itemPQ24item11description(const unsigned char*) { return bonus; }
int playerGetSkillRank__Fii(int, int skill) { return ranks[skill]; }
float getDamageScale__12SkillArcheryi(void*, int rank) { scaleRank = rank; return scaleValue; }
void* __tf18SpellAncestralCall() { static int t; return &t; }
void* checkIfSpellIsRunning__8CreatureRC9type_info(void* creature, void*) { return spells.count(creature) ? spells[creature] : nullptr; }
int posInAura__18SpellAncestralCallP6Point3PfT2(void* spell, void*, float* strength, float*) { *strength = strengths[spell]; return insides[spell]; }
}

#define CHECK(c) do { if (!(c)) { std::printf("line %d: %s\n", __LINE__, #c); return 1; } } while (0)
static int* field(int index, int offset) { return reinterpret_cast<int*>(player + 23984 * index + offset); }

int main() {
    using namespace norrath_aoc;
    unsigned char* self = arena_at(0x100);
    unsigned char* desc = arena_at(0x200);
    unsigned char* base = arena_at(0x300);
    store_word(self + 960, 1);
    store_pointer(desc, base);
    player[23984 + 88] = 2; *field(1, 144) = 40; *field(1, 152) = 80;
    // no description: low = 10 + 1, high = 10 + 2, third = 20 * 2 + 10
    playerRecomputeDamage__6PlayerPQ24item11descriptioni(self, nullptr, 0);
    CHECK(*field(1, 212) == 11 && *field(1, 216) == 12 && *field(1, 208) == 50 && rangeCalls == 0);
    // plain weapon: range 5..9, bonus 7, quarter 10 added
    rangeLow = 5; rangeHigh = 9; bonus = 7; base[30] = 3; store_word(base + 32, 2);
    playerRecomputeDamage__6PlayerPQ24item11descriptioni(self, desc, 3);
    CHECK(*field(1, 212) == 15 && *field(1, 216) == 19 && *field(1, 208) == 57 && rangeCalls == 1 && flagSeen == 3);
    // bow: the other quarter (20) and the archery scale 0.5 for rank 3: 25 + round(12.5) and 29 + round(14.5)
    base[30] = 4; scaleValue = 0.5f; ranks[48] = 3; store_pointer(skills + 192, arena_at(0x400));
    playerRecomputeDamage__6PlayerPQ24item11descriptioni(self, desc, 0);
    CHECK(*field(1, 212) == 38 && *field(1, 216) == 44 && *field(1, 208) == 67 && scaleRank == 3);
    // type 1 with the skill word set: tree entry 3 of class 0 has the marker; rank byte 4: (2 * 4 + 60) * 0.01 * (2 * 0.05 + 1.15)
    base[30] = 3; store_word(base + 32, 1); *field(1, 21968) = 1; ranks[60] = 2; *field(1, 80) = 0;
    std::memset(gSkillTree, 0, sizeof gSkillTree);
    const unsigned long long marker = 0x40000ull;
    std::memcpy(gSkillTree + 40 + 56 * 3, &marker, 8);
    player[23984 + 243 + 3] = 4;
    playerRecomputeDamage__6PlayerPQ24item11descriptioni(self, desc, 0);
    CHECK(*field(1, 212) == 13 && *field(1, 216) == 16 && *field(1, 208) == 57);   // 15 * 0.85 + 0.5, 19 * 0.85 + 0.5 truncated
    // the same with a negative rank byte leaves only (0 + 60) * 0.01 * 1.25 = 0.75
    player[23984 + 243 + 3] = 0xfe;
    playerRecomputeDamage__6PlayerPQ24item11descriptioni(self, desc, 0);
    CHECK(*field(1, 212) == 11 && *field(1, 216) == 14);
    // type 8 uses the other multiplier (rank 61 absent: 1.0) times (2 * 0 + 60) * 0.01 = 0.6
    store_word(base + 32, 8);
    playerRecomputeDamage__6PlayerPQ24item11descriptioni(self, desc, 0);
    CHECK(*field(1, 212) == 9 && *field(1, 216) == 11);   // 15 * 0.6 + 0.5 = 9.5, 19 * 0.6 + 0.5 = 11.9
    // aura: player slots 0 and 2 alive; slot 0's creature has a spell in range with strength 1.5, slot 2's does not
    store_word(base + 32, 2); *field(1, 21968) = 0; ranks.clear();
    nmPlayersAlive = 2; playersAlive[0] = 0; playersAlive[1] = 2;
    unsigned char* creature0 = arena_at(0x500);
    unsigned char* spell0 = arena_at(0x600);
    store_pointer(player + 21872, creature0);
    spells[creature0] = spell0; strengths[spell0] = 1.5f; insides[spell0] = 1;
    playerRecomputeDamage__6PlayerPQ24item11descriptioni(self, desc, 0);
    CHECK(*field(1, 212) == 23 && *field(1, 216) == 29 && *field(1, 208) == 86);   // 15 * 1.5 = 22.5 -> 23, 19 * 1.5 = 28.5 -> 29, 57 * 1.5 = 85.5 -> 86
    // outside the aura and not the caster's: no change; the caster's own spell counts even outside
    insides[spell0] = 0;
    playerRecomputeDamage__6PlayerPQ24item11descriptioni(self, desc, 0);
    CHECK(*field(1, 212) == 15);
    store_pointer(spell0 + 12, self);
    playerRecomputeDamage__6PlayerPQ24item11descriptioni(self, desc, 0);
    CHECK(*field(1, 212) == 23);
    // the two status words: x1.5, then x1.2 (the float 1.2 is just below 1.2, so 15 * 1.2 truncates to 17)
    spells.clear(); nmPlayersAlive = 0; store_word(self + 904, 1);
    playerRecomputeDamage__6PlayerPQ24item11descriptioni(self, desc, 0);
    CHECK(*field(1, 212) == 22 && *field(1, 216) == 28 && *field(1, 208) == 85);
    store_word(self + 904, 0); store_word(self + 912, 1);
    playerRecomputeDamage__6PlayerPQ24item11descriptioni(self, desc, 0);
    CHECK(*field(1, 212) == 17 && *field(1, 216) == 22 && *field(1, 208) == 68);
    std::puts("ok");
    return 0;
}
