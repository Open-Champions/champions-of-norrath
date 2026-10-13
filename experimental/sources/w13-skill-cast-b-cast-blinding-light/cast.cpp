// Observed cast__18SkillBlindingLightiP8CreatureG6Point3s at 0x002b9be0, 396 bytes in .text.
// Steps, in order (creature = the third argument, point = the by-value Point3, angle = the trailing short):
//  1. eight halfwords of the creature are written: at byte offsets 140, 142, 144, 146 the values 86, 128, 250, 110 and
//     at 148, 150, 152, 154 the same four values again;
//  2. a local 16-byte list head is created with its first word zero and engineSetScreenColor__Fiiii(128, 128, 128, 0)
//     is called;
//  3. objectGetSortedCreatureList__FG6Point3sffP8ListHeadP8Creature is called with (point, angle, 360.0, 90.0, the
//     local list, null);
//  4. the resource "spel_he_blindlight.anm" is looked up in "fx.lmp" and played with
//     soundPlaySequence__FP15AnimationHeaderP10GameObjectf(result, creature, 1.0);
//  5. when the list's first word is non-null and the creature is non-null, the entry at table byte 160 of the creature's
//     own table (the word at creature+72; a signed halfword adjusts the receiver, a function word is at +4) is
//     called with (adjusted creature, the word at node+12 where node is the list's first word). When that returns
//     non-zero, the three floats at (that same word)+16 are copied, 84 bytes are allocated with blockAlloc__Fi (the
//     list's first word is read again for this) and, when the block is not null,
//     __19BlindingLightEffectG6Point3iP8CreatureP5Skill is called with (block, the copied point by reference, rank,
//     creature, skill);
//  6. listDestroy__FP8ListHead is called on the local list and the function returns zero.
// Inferred: the halfword group is two copies of a four-value colour, the list holds creatures sorted by distance
// within a 360/90 range, and the entry at table byte 160 filters them (it is the same entry the convert-undead
// precondition calls); the effect is placed at the first creature that passes. None of this is established here.
// Unknown: the list node layout beyond the word at +12, what the table entry is, the meaning of 360.0 and 90.0 and of
// the halfword values, and the layout of skill and creature. Nothing here ran on the EE.
#include "../w13-skill-cast-b-common/skill_access.hpp"

struct ListHead { void* first; unsigned unknown[3]; };
extern "C" {
void engineSetScreenColor__Fiiii(int first, int second, int third, int fourth);
void objectGetSortedCreatureList__FG6Point3sffP8ListHeadP8Creature(Point3 point, short angle, float first, float second, ListHead* list, void* exclude);
void* lumpFindResource__FPcT0(char* lump, char* name);
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* sound, void* object, float volume);
void* blockAlloc__Fi(int size);
void effect_ctor(void* self, Point3 point, int rank, void* creature, void* skill) asm("__19BlindingLightEffectG6Point3iP8CreatureP5Skill");
void listDestroy__FP8ListHead(ListHead* list);
int cast_blinding_light(void* skill, int rank, void* creature, Point3 point, short angle) asm("cast__18SkillBlindingLightiP8CreatureG6Point3s");
}
static char lump_name[] = "fx.lmp";
static char sound_name[] = "spel_he_blindlight.anm";
extern "C" int cast_blinding_light(void* skill, int rank, void* creature, Point3 point, short angle) {
    unsigned char* c = static_cast<unsigned char*>(creature);
    short* colours = reinterpret_cast<short*>(c + 140);
    colours[1] = 128;
    colours[2] = 250;
    colours[3] = 110;
    colours[5] = 128;
    colours[6] = 250;
    colours[7] = 110;
    colours[0] = 86;
    colours[4] = 86;
    ListHead list;
    list.first = nullptr;
    engineSetScreenColor__Fiiii(128, 128, 128, 0);
    objectGetSortedCreatureList__FG6Point3sffP8ListHeadP8Creature(point, angle, 360.0f, 90.0f, &list, nullptr);
    soundPlaySequence__FP15AnimationHeaderP10GameObjectf(lumpFindResource__FPcT0(lump_name, sound_name), creature, 1.0f);
    if (list.first != nullptr && creature != nullptr) {
        unsigned char* node = static_cast<unsigned char*>(list.first);
        if (w13scb::call_object_entry(c, 72, 160, w13scb::from_word(*reinterpret_cast<unsigned*>(node + 12))) != 0) {
            unsigned char* target = w13scb::from_word(*reinterpret_cast<unsigned*>(static_cast<unsigned char*>(list.first) + 12));
            const Point3 place = *reinterpret_cast<Point3*>(target + 16);
            void* effect = blockAlloc__Fi(84);
            if (effect) effect_ctor(effect, place, rank, creature, skill);
        }
    }
    listDestroy__FP8ListHead(&list);
    return 0;
}
