// Observed cast__13SkillEntangleiP8CreatureG6Point3s at 0x002c6460, 460 bytes in .text. The fifth argument is a signed halfword
// (the trailing "s" of the mangled name), used as an integer angle.
// Steps, in order: (1) the by-value Point3 is copied to the frame; (2) the skill's table entry at byte 120 (a signed
// halfword receiver adjustment, a function word at +4) is called with (skill + adjustment, rank, 1), giving R; (3) the
// entry at byte 88 is called the same way with (skill + adjustment, 0, R), giving a count limit; (4) two list heads are
// zeroed (only their first words are observed) and objectGetSortedCreatureList__FG6Point3sffP8ListHeadP8Creature is
// called with (the point, the angle, 249.0, 120.0, the second list, null) to fill the second one; (5) when that list is
// not empty and the limit is positive, the nodes are walked in order (next word at node+0, target word at node+12) until
// the list ends or the number of started spells reaches the limit. A node is skipped when: the casting creature is not
// null and the entry at byte 160 of the table whose address is the word at casting-creature+72 (halfword adjustment,
// function word) called with (casting creature + adjustment, target) returns zero; or bit 0x01000000 of the word at
// target+12 is set; or isBoss__FP10GameObject(target) is nonzero; or the signed halfword at target+794 is positive.
// Otherwise 40 bytes are allocated with blockAlloc__Fi, __13SpellEntangleiP8CreatureP5Skill is called with (block, R, target, skill)
// when the allocation succeeded, addSpell__8CreatureP5Spell(target, block) is called (also with a null block), and the
// started count increases by one; (6) both lists are passed to listDestroy__FP8ListHead (the filled one first). The
// function always returns zero.
// The table entries at bytes 120, 88 and 160 are named, in the class tables, tweekRank__5Skillii,
// evaluateRampAtRank__5Skillii and (for the creature) an entry whose meaning is not read here; those readings are
// inferred from the tables. Inferred: this roots up to a limit of nearby eligible creatures, nearest first, with the
// creature's own filter and a boss exclusion. Unknown: what ramp index 0 selects, what flag 0x01000000 and the halfword at
// +794 are, what the byte-160 entry decides (it reads as a can-affect test), the angle unit, the list layout beyond the
// first word, and the second (never filled here) list. Nothing here ran on the EE.
#include "../w13-skill-cast-a-common/skill_access.hpp"

struct ListHead { unsigned first; unsigned unobserved[3]; };
extern "C" {
void* blockAlloc__Fi(int size);
int objectGetSortedCreatureList__FG6Point3sffP8ListHeadP8Creature(Point3 center, short angle, float a, float b, ListHead* list, void* exclude);
void listDestroy__FP8ListHead(ListHead* list);
int isBoss__FP10GameObject(void* object);
void addSpell__8CreatureP5Spell(void* creature, void* spell);
void spell_ctor_entangle(void* self, int rank, void* creature, void* skill) asm("__13SpellEntangleiP8CreatureP5Skill");
int cast_entangle(void* skill, int rank, void* creature, Point3 point, short angle) asm("cast__13SkillEntangleiP8CreatureG6Point3s");
}
typedef int AliasedInt __attribute__((may_alias));
typedef short AliasedShort __attribute__((may_alias));
typedef unsigned AliasedWord __attribute__((may_alias));
extern "C" int cast_entangle(void* skill, int rank, void* creature, Point3 point, short angle) {
    unsigned char* const skill_bytes = static_cast<unsigned char*>(skill);
    const int tweaked = w13sca::call_skill_entry(skill_bytes, 120, rank, 1);
    const int limit = w13sca::call_skill_entry(skill_bytes, 88, 0, tweaked);
    ListHead unused;
    unused.first = 0;
    ListHead list;
    list.first = 0;
    int started = 0;
    objectGetSortedCreatureList__FG6Point3sffP8ListHeadP8Creature(point, angle, 249.0f, 120.0f, &list, nullptr);
    unsigned char* node = w13sca::from_word(list.first);
    if (list.first != 0 && limit > 0) {
        for (;;) {
            unsigned char* const target = w13sca::from_word(*reinterpret_cast<AliasedWord*>(node + 12));
            const bool allowed = !creature || w13sca::call_object_entry(static_cast<unsigned char*>(creature), 72, 160, target) != 0;
            if (allowed && !(*reinterpret_cast<AliasedInt*>(target + 12) & 0x01000000) && !isBoss__FP10GameObject(target) &&
                !(*reinterpret_cast<AliasedShort*>(target + 794) > 0)) {
                void* spell = blockAlloc__Fi(40);
                if (spell) spell_ctor_entangle(spell, tweaked, target, skill);
                addSpell__8CreatureP5Spell(target, spell);
                ++started;
            }
            const unsigned next = *reinterpret_cast<AliasedWord*>(node);
            if (next == 0 || started >= limit) break;
            node = w13sca::from_word(next);
        }
    }
    listDestroy__FP8ListHead(&list);
    listDestroy__FP8ListHead(&unused);
    return 0;
}
