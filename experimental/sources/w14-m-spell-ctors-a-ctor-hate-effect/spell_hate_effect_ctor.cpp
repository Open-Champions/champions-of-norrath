// Observed __15SpellHateEffectiiP8CreatureP10GameObjectP5Skill at 0x001ff6f8, 264 bytes (private executable symbol
// values). Constructor of the spell object. Arguments: the object, an int (kept at +36), a second int K (its low
// byte at +0 and the whole word at +40), a Creature pointer C (+12), a GameObject pointer G and a Skill pointer S
// (+44). Nothing is returned.
//
// Observed layout, the same Spell base as the other constructors of this family:
//   +0 byte K, +4 word 0, +8 word 0, +12 word C,
//   +16 word  the high half of the sync random state after one LCG step (state * 0x19660d + 0x3c6ef35f),
//   +20 word  the _vt$15SpellHateEffect table (the base table is stored first and then replaced),
//   +24 word 0 (an embedded list head), +28 word 0 (a second embedded list head),
//   +32 word  frameNm + 60 * first int, +36 word first int, +40 word K, +44 word S.
// Then, in order: when G is not null, listOnewayLink(head at +24, G, G); when C is not null,
// listOnewayLink(head at +28, C, C).
// Inferred: a hate effect that links itself into two lists. Unknown: the meaning of the list heads, and of the
// frame value stored at +32 (the 60-byte stride is observed, its record type is not). Nothing here ran on the EE.

extern "C" {
extern unsigned char vt_SpellHateEffect[] __asm__("_vt$15SpellHateEffect");
extern unsigned syncRandState;
extern int frameNm;
void listOnewayLink__FP8ListHeadT0Pv(void* head, void* link, void* data);
}

namespace {
inline unsigned low(const void* p) { return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(p)); }
}

extern "C" void __15SpellHateEffectiiP8CreatureP10GameObjectP5Skill(void* self, int first, int kind, void* creature, void* gameObject, void* skill) {
    unsigned char* o = static_cast<unsigned char*>(self);
    o[0] = static_cast<unsigned char>(kind);
    *reinterpret_cast<unsigned*>(o + 4) = 0u;
    *reinterpret_cast<unsigned*>(o + 8) = 0u;
    *reinterpret_cast<unsigned*>(o + 12) = low(creature);
    syncRandState = syncRandState * 0x19660du + 0x3c6ef35fu;
    *reinterpret_cast<unsigned*>(o + 16) = syncRandState >> 16;
    *reinterpret_cast<unsigned*>(o + 20) = low(vt_SpellHateEffect);
    *reinterpret_cast<unsigned*>(o + 24) = 0u;
    *reinterpret_cast<unsigned*>(o + 28) = 0u;
    if (gameObject != nullptr) listOnewayLink__FP8ListHeadT0Pv(o + 24, gameObject, gameObject);
    if (creature != nullptr) listOnewayLink__FP8ListHeadT0Pv(o + 28, creature, creature);
    *reinterpret_cast<unsigned*>(o + 32) = static_cast<unsigned>(frameNm) + static_cast<unsigned>(first) * 60u;
    *reinterpret_cast<int*>(o + 36) = first;
    *reinterpret_cast<int*>(o + 40) = kind;
    *reinterpret_cast<unsigned*>(o + 44) = low(skill);
}
