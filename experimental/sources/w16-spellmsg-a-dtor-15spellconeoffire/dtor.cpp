// Observed _$_15SpellConeOfFire at 0x002b5a00, 216 bytes. O is the object (first argument), F the flags (second argument).
// In order it: stores the table _vt$15SpellConeOfFire at O+20; calls soundStopSound__Fi with the word at O+84 and stores -1
// there; calls lightDeactivate__FP5Light on O+24; calls lumpFindResource__FPcT0 with the strings "fx.lmp" and
// "spel_ew_cfire_end.anm"; calls soundPlaySequence__FP15AnimationHeaderG6Point3f with that result, the 12-byte point at
// (the object that is the word at O+12) + 16 and 1.0f; calls the Light destructor _$_5Light on O+24 with the argument 2;
// stores the table _vt$5Spell at O+20; calls listDestroy__FP8ListHead on O+4; and finally, when bit 0 of F is set, calls
// blockFree__FPv on O. Other flag bits do not free.
// Inferred: the destructor of the class (from its symbol name), which stops the fire sound, plays an end animation sound at
// the caster's position and releases the light the constructor set up. Unknown: the meaning of the sound result slot beyond
// the handle role the constructor gives it. Nothing here ran on the EE.
struct Point3 { float x, y, z; };

extern "C" {
extern unsigned char vt_Spell[] __asm__("_vt$5Spell");
extern unsigned char vt_SpellConeOfFire[] __asm__("_vt$15SpellConeOfFire");
void soundStopSound__Fi(int handle);
void lightDeactivate__FP5Light(void* light);
void* lumpFindResource__FPcT0(char* lump, char* resource);
int soundPlaySequence__FP15AnimationHeaderG6Point3f(void* header, Point3 position, float volume);
void light_destructor(void* light, int flags) __asm__("_$_5Light");
void listDestroy__FP8ListHead(void* head);
void blockFree__FPv(void* block);
}

namespace {
inline unsigned low(const void* p) { return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(p)); }
}

extern "C" void destructor_15SpellConeOfFire(void* self, int flags) __asm__("_$_15SpellConeOfFire");
extern "C" void destructor_15SpellConeOfFire(void* self, int flags) {
    unsigned char* const o = static_cast<unsigned char*>(self);
    *reinterpret_cast<unsigned*>(o + 20) = low(vt_SpellConeOfFire);
    const int release = flags & 1;
    soundStopSound__Fi(*reinterpret_cast<int*>(o + 84));
    *reinterpret_cast<int*>(o + 84) = -1;
    lightDeactivate__FP5Light(o + 24);
    void* const animation = lumpFindResource__FPcT0(const_cast<char*>("fx.lmp"), const_cast<char*>("spel_ew_cfire_end.anm"));
    const unsigned char* const caster = reinterpret_cast<const unsigned char*>(static_cast<__UINTPTR_TYPE__>(*reinterpret_cast<unsigned*>(o + 12)));
    const Point3 at = *reinterpret_cast<const Point3*>(caster + 16);
    soundPlaySequence__FP15AnimationHeaderG6Point3f(animation, at, 1.0f);
    light_destructor(o + 24, 2);
    *reinterpret_cast<unsigned*>(o + 20) = low(vt_Spell);
    listDestroy__FP8ListHead(o + 4);
    if (release != 0) blockFree__FPv(o);
}
