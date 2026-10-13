// Observed __16SpellConeOfFrostiP8CreatureP5Skill at 0x002af390, 376 bytes (private executable symbol values).
// Constructor of the spell object: arguments are the object, an int whose low byte is stored (K), a Creature
// pointer C and a Skill pointer S. Nothing is returned.
//
// Observed layout, the same Spell base as the other constructors of this family:
//   +0 byte K, +4 word 0, +8 word S, +12 word C,
//   +16 word  the high half of the sync random state after one LCG step (state * 0x19660d + 0x3c6ef35f),
//   +20 word  the _vt$16SpellConeOfFrost table (the base table is stored first and then replaced),
//   +72 Point3 at C + 16 with 0.0 added to x, and 60.0 added to z (y is copied unchanged),
//   +84 word  the sound handle returned below.
// A light record is initialised in place at +24 and handed to lightAdd__FP5Light:
//   +8 and +9 bytes 0; halfwords +10 = 77, +12 = 76, +14 = 187, +16 = 0, +18 = 77, +20 = 76, +22 = 187, +24 = 0;
//   floats +28 = 156.0, +32 = the float constant at 0x00482c8c (about 4.1e-5), +36 = 156.0, +40 = the same constant,
//   +44 = 0.0. The other bytes of the record are not written.
// Then lumpFindResource__FPcT0("fx.lmp", "spel_ew_cfrost.anm") gives the header, and soundPlaySequence__FP15AnimationHeader
// G6Point3f is called with a copy of the point at C + 16 and volume 1.0f. The result is stored at +84.
// Inferred: a cold flash at the caster and a sound. Unknown: the meaning of the light fields and of the stored point.
// Nothing here ran on the EE.
struct Point3 { float x, y, z; };

extern "C" {
extern unsigned char vt_SpellConeOfFrost[] __asm__("_vt$16SpellConeOfFrost");
extern unsigned syncRandState;
void lightAdd__FP5Light(void* light);
void* lumpFindResource__FPcT0(char* lump, char* resource);
int soundPlaySequence__FP15AnimationHeaderG6Point3f(void* header, Point3 position, float volume);
}

namespace {
inline unsigned low(const void* p) { return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(p)); }
inline float floatFromBits(unsigned bits) {
    union { unsigned u; float f; } v;
    v.u = bits;
    return v.f;
}
}

extern "C" void __16SpellConeOfFrostiP8CreatureP5Skill(void* self, int kind, void* creature, void* skill) {
    unsigned char* o = static_cast<unsigned char*>(self);
    o[0] = static_cast<unsigned char>(kind);
    *reinterpret_cast<unsigned*>(o + 4) = 0u;
    *reinterpret_cast<unsigned*>(o + 8) = low(skill);
    *reinterpret_cast<unsigned*>(o + 12) = low(creature);
    syncRandState = syncRandState * 0x19660du + 0x3c6ef35fu;
    *reinterpret_cast<unsigned*>(o + 16) = syncRandState >> 16;
    *reinterpret_cast<unsigned*>(o + 20) = low(vt_SpellConeOfFrost);

    const Point3 at = *reinterpret_cast<const Point3*>(static_cast<unsigned char*>(creature) + 16);
    Point3 flash;
    flash.x = at.x + 0.0f;
    flash.y = at.y;
    flash.z = at.z + 60.0f;
    *reinterpret_cast<Point3*>(o + 72) = flash;

    unsigned char* light = o + 24;
    light[8] = 0;
    light[9] = 0;
    *reinterpret_cast<short*>(light + 10) = 77;
    *reinterpret_cast<short*>(light + 12) = 76;
    *reinterpret_cast<short*>(light + 14) = 187;
    *reinterpret_cast<short*>(light + 16) = 0;
    *reinterpret_cast<short*>(light + 18) = 77;
    *reinterpret_cast<short*>(light + 20) = 76;
    *reinterpret_cast<short*>(light + 22) = 187;
    *reinterpret_cast<short*>(light + 24) = 0;
    const float constant = floatFromBits(0x382c598au);
    *reinterpret_cast<float*>(light + 28) = 156.0f;
    *reinterpret_cast<float*>(light + 32) = constant;
    *reinterpret_cast<float*>(light + 36) = 156.0f;
    *reinterpret_cast<float*>(light + 40) = constant;
    *reinterpret_cast<float*>(light + 44) = 0.0f;
    lightAdd__FP5Light(light);

    void* header = lumpFindResource__FPcT0(const_cast<char*>("fx.lmp"), const_cast<char*>("spel_ew_cfrost.anm"));
    const Point3 position = *reinterpret_cast<const Point3*>(static_cast<unsigned char*>(creature) + 16);
    *reinterpret_cast<int*>(o + 84) = soundPlaySequence__FP15AnimationHeaderG6Point3f(header, position, 1.0f);
}
