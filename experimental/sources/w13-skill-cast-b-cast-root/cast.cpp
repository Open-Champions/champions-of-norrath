// Observed cast__9SkillRootiP8CreatureG6Point3s at 0x002ac4f0, 500 bytes in .text.
// Steps, in order (skill = first argument, rank = second, creature = third, point = the by-value Point3, angle = the
// trailing short; the function returns zero in every case):
//  1. the skill's table (the word at skill+96) entry at byte 120 is called as (skill + adjustment, rank, 20); its integer
//     result is R. (Slot 15 of this class's table holds tweekRank__5Skillii, observed in the private image.)
//  2. the resource "spel_ew_shock.anm" is looked up in "fx.lmp" with lumpFindResource__FPcT0 and played with
//     soundPlaySequence__FP15AnimationHeaderP10GameObjectf(result, creature, 1.0);
//  3. the table entry at byte 88 (slot 11, evaluateRampAtRank__5Skillii in the image) is called as (skill + adjustment, 0, R);
//     its result is the limit N;
//  4. two local 16-byte list heads are created with their first words zero, and
//     objectGetSortedCreatureList__FG6Point3sffP8ListHeadP8Creature is called with (point, angle, 248.0, 120.0, the second
//     list, null);
//  5. when that list's first word (a node) is non-null and N is positive, the nodes are walked by the word at node+0
//     (next), taking the word at node+12 as a target. For each node: when the creature is non-null, the entry at table
//     byte 160 of the creature's own table (the word at creature+72) is called with (adjusted creature, target) and a
//     zero result skips the target; a target whose word at +12 has bit 0x01000000 set is skipped; a target for which
//     isBoss__FP10GameObject is non-zero is skipped; a target whose signed halfword at +794 is greater than zero is
//     skipped. Every other target gets a 40-byte block from blockAlloc__Fi, __9SpellRootiP8CreatureP5Skill is called
//     with (block, R, target, skill) when the block is not null, addSpell__8CreatureP5Spell is called on the target with
//     the block (also when it is null), and a counter is incremented. The walk ends at a null node or once the counter
//     reaches N;
//  6. listDestroy__FP8ListHead is called on the second list, then on the first.
// Inferred: this roots up to N creatures near the point that pass the creature's filter and are not bosses, by giving
// each a root spell. None of this is established here.
// Unknown: the node layout beyond the words at +0 and +12, what the table entry at byte 160 is, the meaning of the
// halfword at +794 and the bit 0x01000000, why the first list is created and destroyed unused, and the layouts of
// skill and creature. Nothing here ran on the EE.
#include "../w13-skill-cast-b-common/skill_access.hpp"

struct ListHead { void* first; unsigned unknown[3]; };
extern "C" {
void* lumpFindResource__FPcT0(char* lump, char* name);
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* sound, void* object, float volume);
void objectGetSortedCreatureList__FG6Point3sffP8ListHeadP8Creature(Point3 point, short angle, float first, float second, ListHead* list, void* exclude);
int isBoss__FP10GameObject(void* object);
void* blockAlloc__Fi(int size);
void spell_ctor_root(void* self, int rank, void* creature, void* skill) asm("__9SpellRootiP8CreatureP5Skill");
void addSpell__8CreatureP5Spell(void* creature, void* spell);
void listDestroy__FP8ListHead(ListHead* list);
int cast_root(void* skill, int rank, void* creature, Point3 point, short angle) asm("cast__9SkillRootiP8CreatureG6Point3s");
}
static char lump_name[] = "fx.lmp";
static char sound_name[] = "spel_ew_shock.anm";
extern "C" int cast_root(void* skill, int rank, void* creature, Point3 point, short angle) {
    unsigned char* s = static_cast<unsigned char*>(skill);
    unsigned char* c = static_cast<unsigned char*>(creature);
    int count = 0;
    const int adjusted_rank = w13scb::call_skill_entry(s, 120, rank, 20);
    soundPlaySequence__FP15AnimationHeaderP10GameObjectf(lumpFindResource__FPcT0(lump_name, sound_name), creature, 1.0f);
    const int limit = w13scb::call_skill_entry(s, 88, 0, adjusted_rank);
    ListHead unused;
    unused.first = nullptr;
    ListHead list;
    list.first = nullptr;
    objectGetSortedCreatureList__FG6Point3sffP8ListHeadP8Creature(point, angle, 248.0f, 120.0f, &list, nullptr);
    unsigned char* node = static_cast<unsigned char*>(list.first);
    if (node != nullptr && limit > 0) {
        do {
            unsigned char* target = w13scb::from_word(*reinterpret_cast<unsigned*>(node + 12));
            if (c == nullptr || w13scb::call_object_entry(c, 72, 160, target) != 0) {
                if (!(*reinterpret_cast<unsigned*>(target + 12) & 0x01000000) && !isBoss__FP10GameObject(target) && !(*reinterpret_cast<short*>(target + 794) > 0)) {
                    void* spell = blockAlloc__Fi(40);
                    if (spell) spell_ctor_root(spell, adjusted_rank, target, skill);
                    addSpell__8CreatureP5Spell(target, spell);
                    ++count;
                }
            }
            node = w13scb::from_word(*reinterpret_cast<unsigned*>(node));
        } while (node != nullptr && count < limit);
    }
    listDestroy__FP8ListHead(&list);
    listDestroy__FP8ListHead(&unused);
    return 0;
}
