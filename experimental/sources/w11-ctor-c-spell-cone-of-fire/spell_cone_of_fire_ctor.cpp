// Observed __15SpellConeOfFireiP8CreatureP5Skill at 0x002b5898, 360 bytes. Constructor of the spell
// object: arguments are the object, an int whose low byte is stored (K), a Creature pointer C and a
// Skill pointer S. The Spell base constructor is inlined (same layout as the horn-omen effect spell):
//   +0 byte K, +4 word 0, +8 word S, +12 word C,
//   +16 word  the high half of a step of the sync random state (state * 1664525 + 1013904223),
//   +20 word  the Spell table first, then the _vt$15SpellConeOfFire table.
// A light record is initialised in place at +24 and handed to lightAdd__FP5Light (offsets from the
// start of the record, so +24 + n in the object):
//   +8 and +9 bytes 0; halfwords +10 = 255, +12 = 125, +14 = 0, +16 = 66, +18 = 255, +20 = 125,
//   +22 = 0, +24 = 66; floats +28 = 0.0, +32 = 1.0f / 0.0f, +36 = 0.0, +40 = 1.0f / 0.0f,
//   +44 = 0x3f19db22 (about 0.601). The other bytes of the record are not written.
// The quotient is computed at run time in the image (the compiler cannot fold a division by zero),
// so the stored words are whatever the EE divide gives for 1.0 / 0.0.
// After the light call the 12-byte point at C + 16 is copied to +72; the same
// point (read from C again, after the light call) goes to soundPlaySequence__FP15AnimationHeaderG6Point3f together with the
// result of lumpFindResource__FPcT0("fx.lmp", "spel_ew_cfire.anm") and 1.0f. The sound result is
// stored at +84 and passed to soundMakePersistant__Fi.
// Nothing is returned. Inferred: the light is a flash at the caster, the sound handle is kept at +84.
// Unknown: the meaning of the light fields and of the point at +72 beyond its copy. Nothing here ran on the EE.
struct Point3 { float x, y, z; };

extern "C" {
extern unsigned char vt_Spell[] __asm__("_vt$5Spell");
extern unsigned char vt_SpellConeOfFire[] __asm__("_vt$15SpellConeOfFire");
extern unsigned syncRandState;
void lightAdd__FP5Light(void* light);
void* lumpFindResource__FPcT0(char* lump, char* resource);
int soundPlaySequence__FP15AnimationHeaderG6Point3f(void* header, Point3 position, float volume);
void soundMakePersistant__Fi(int handle);
}

namespace {
inline unsigned low(const void* p) { return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(p)); }
}

extern "C" void __15SpellConeOfFireiP8CreatureP5Skill(void* self, int kind, void* creature, void* skill) {
    unsigned char* o = static_cast<unsigned char*>(self);
    o[0] = static_cast<unsigned char>(kind);
    *reinterpret_cast<unsigned*>(o + 20) = low(vt_Spell);
    syncRandState = syncRandState * 0x19660du + 0x3c6ef35fu;
    *reinterpret_cast<unsigned*>(o + 12) = low(creature);
    *reinterpret_cast<unsigned*>(o + 8) = low(skill);
    *reinterpret_cast<unsigned*>(o + 4) = 0u;
    *reinterpret_cast<unsigned*>(o + 16) = syncRandState >> 16;
    *reinterpret_cast<unsigned*>(o + 20) = low(vt_SpellConeOfFire);
    float zero = 0.0f;
    // Keep the divide at run time, as the image does.
#if defined(__mips__)
    __asm__("" : "+f"(zero));
#else
    zero = *static_cast<volatile float*>(&zero);
#endif
    const float quotient = 1.0f / zero;
    unsigned char* light = o + 24;
    light[9] = 0;
    *reinterpret_cast<short*>(light + 18) = 255;
    *reinterpret_cast<short*>(light + 20) = 125;
    *reinterpret_cast<short*>(light + 10) = 255;
    *reinterpret_cast<short*>(light + 12) = 125;
    *reinterpret_cast<float*>(light + 40) = quotient;
    *reinterpret_cast<unsigned*>(light + 44) = 0x3f19db22u;
    *reinterpret_cast<float*>(light + 28) = zero;
    *reinterpret_cast<float*>(light + 36) = zero;
    *reinterpret_cast<float*>(light + 32) = quotient;
    *reinterpret_cast<short*>(light + 24) = 66;
    *reinterpret_cast<short*>(light + 16) = 66;
    *reinterpret_cast<short*>(light + 14) = 0;
    *reinterpret_cast<short*>(light + 22) = 0;
    light[8] = 0;
    const Point3 at = *reinterpret_cast<const Point3*>(static_cast<unsigned char*>(creature) + 16);
    *reinterpret_cast<Point3*>(o + 72) = at;
    lightAdd__FP5Light(light);
    void* header = lumpFindResource__FPcT0(const_cast<char*>("fx.lmp"), const_cast<char*>("spel_ew_cfire.anm"));
    const int handle = soundPlaySequence__FP15AnimationHeaderG6Point3f(
        header, *reinterpret_cast<const Point3*>(static_cast<unsigned char*>(creature) + 16), 1.0f);
    *reinterpret_cast<int*>(o + 84) = handle;
    soundMakePersistant__Fi(handle);
}
