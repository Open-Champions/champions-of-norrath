// Observed __11DiseaseBoltsiG6Point3P10GameObjectP5Skill at 0x292178, 476 bytes. Arguments: the
// object, a short heading, an int N, a position (by value), an owner GameObject (may be null)
// and a Skill pointer. Returns nothing useful. Steps, in order:
//  1. The Projectile constructor runs with (object, position, owner, null map).
//  2. Words +84 and +88 (two list heads) = 0, word +72 = _vt$11DiseaseBolt, halfword +56 =
//     heading, word +108 = 0.
//  3. Float +100 = icos(heading) * 350.0 and float +104 = isin(halfword +56) * 350.0; halfword
//     +96 = N.
//  4. When the owner is non-null, listOnewayLink__FP8ListHeadT0Pv(object+84, owner, owner) runs.
//  5. Word +92 = the skill, word +112 = frameNm + 300, word +116 = frameNm + 10, word +120 = 0.
//  6. When the owner is non-null: a local list head is cleared and
//     objectGetSortedCreatureList(copy of the object's three floats at +16, halfword +56,
//     280 (bits 0x438bfffc), 60.0, &list, null) fills it. For each listed creature (the
//     creature word is at node+12) the owner's table (word at owner+72) entry at byte 160 -
//     a signed halfword adjusts `this`, the function word is at +4 - is called with (owner +
//     adjust, creature); a non-zero result links the creature into object+88 with
//     listOnewayLink__FP8ListHeadT0Pv(object+88, creature, creature). The local list is then
//     destroyed with listDestroy__FP8ListHead.
//  7. objectAddToRunList(object), then lumpFindResource("fx.lmp", "spel_de_disbolt.anm") and
//     soundPlaySequence(found, object, 1.0).
// Inferred: the loop selects creatures near the bolt that the owner's check accepts, because
// the list is built from the object's position and filtered by an owner call. Unknown: what N,
// the skill word, the two list heads, the frame counters and the table entry mean. Nothing here
// ran on the EE.
#include "../w11-ctor-a-common/ctor.hpp"

extern "C" {
extern unsigned char vt_DiseaseBolt[] __asm__("_vt$11DiseaseBolt");
void __11DiseaseBoltsiG6Point3P10GameObjectP5Skill(void* self, short heading, int n, Point3 position, void* owner, void* skill);
}

extern "C" void __11DiseaseBoltsiG6Point3P10GameObjectP5Skill(void* self, short heading, int n, Point3 position, void* owner, void* skill) {
    using namespace w11;
    unsigned char* const first_head = static_cast<unsigned char*>(self) + 84;
    unsigned char* const second_head = static_cast<unsigned char*>(self) + 88;
    projectile_construct(self, position, owner, nullptr);
    u32(self, 84) = 0;
    u32(self, 72) = word(vt_DiseaseBolt);
    u32(self, 88) = 0;
    i16(self, 56) = heading;
    u32(self, 108) = 0;
    f32(self, 100) = icos__Fi(heading) * 350.0f;
    f32(self, 104) = isin__Fi(i16(self, 56)) * 350.0f;
    i16(self, 96) = static_cast<short>(n);
    if (owner) listOnewayLink__FP8ListHeadT0Pv(first_head, owner, owner);
    u32(self, 92) = word(skill);
    u32(self, 112) = static_cast<unsigned>(frameNm + 300);
    u32(self, 116) = static_cast<unsigned>(frameNm + 10);
    u32(self, 120) = 0;
    if (owner) {
        unsigned list = 0;
        objectGetSortedCreatureList__FG6Point3sffP8ListHeadP8Creature(Point3{f32(self, 16), f32(self, 20), f32(self, 24)}, i16(self, 56),
                                                                      __builtin_bit_cast(float, 0x438bfffcu), 60.0f, &list, nullptr);
        for (unsigned node = list; node != 0; node = u32(pointer(node), 0)) {
            void* const creature = pointer(u32(pointer(node), 12));
            if (object_call_int1(owner, 160, creature)) listOnewayLink__FP8ListHeadT0Pv(second_head, creature, creature);
        }
        listDestroy__FP8ListHead(&list);
    }
    objectAddToRunList__FP10GameObject(self);
    soundPlaySequence__FP15AnimationHeaderP10GameObjectf(
        lumpFindResource__FPcT0(const_cast<char*>("fx.lmp"), const_cast<char*>("spel_de_disbolt.anm")), self, 1.0f);
}
