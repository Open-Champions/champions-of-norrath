// Observed __18SpellDiseaseShieldiP8CreatureP5Skill at 0x002955c0, 764 bytes. Constructor of the
// spell object: arguments are the object, an int whose low byte is stored (K), a Creature pointer C
// and a Skill pointer S. The Spell base constructor is inlined (same layout as the horn-omen effect
// spell):
//   +0 byte K, +4 word 0, +8 word S, +12 word C,
//   +16 word  the high half of a step of the sync random state (state * 1664525 + 1013904223),
//   +20 word  the Spell table first, then the _vt$18SpellDiseaseShield table.
// Then, in the order of the image:
//  1. The skill's table (word at S + 96) is read as 8-byte entries {signed halfword adjustment, unused
//     halfword, function address}; entry at table + 104 is called as fn(S + adjustment, 2, K as a
//     signed char, C) and 60 times its int result plus frameNm goes to +420. Entry 104 of the
//     Skill base table is evaluateRampAtRank__5SkilliiP10GameObject.
//  2. The same entry is called again as fn(S + adjustment, 1, K as a signed char, C); 12 times the
//     result goes to +424. (The image reloads S, K and C from the object between the two calls; the
//     values are the ones just stored.)
//  3. Sixteen times (i = 0..15) two points are drawn with eRand__5Spell(this), which steps the
//     random state at +16 the same way as the base (state * 1664525 + 1013904223, high half):
//     each coordinate is float(draw) * 2^-16 * 2 - 1.0. A point is redrawn, all three coordinates, while
//     the squared length is above 1.0 (compared as 1.0 < x*x + y*y + z*z, summed x, y, z).
//     The first point goes to +24 + 12*i, scaled by the float at C + 28 after it is accepted; the
//     second goes to +216 + 12*i, unscaled.
//  4. The 12-byte point at C + 16 is copied to +408, lumpFindResource__FPcT0("fx.lmp",
//     "spel_de_dishield.anm") runs, and soundPlaySequence__FP15AnimationHeaderG6Point3f gets its
//     result, a copy of the point at C + 16 and 1.0f. The sound result is not stored.
// Inferred: a ring of 16 random offsets inside a sphere of the creature's radius at +28, and 16 unit
// directions; +420 and +424 look like an end frame and a second duration (the 60 and 12 are frame
// counts per unit of the call's result).
// Unknown: what the integer arguments 2 and 1 of the entry call select, the meaning of +424 and of the
// two point arrays beyond their generation. Nothing here ran on the EE.
struct Point3 { float x, y, z; };

extern "C" {
extern unsigned char vt_Spell[] __asm__("_vt$5Spell");
extern unsigned char vt_SpellDiseaseShield[] __asm__("_vt$18SpellDiseaseShield");
extern unsigned syncRandState;
extern int frameNm;
int eRand__5Spell(void* self);
void* lumpFindResource__FPcT0(char* lump, char* resource);
int soundPlaySequence__FP15AnimationHeaderG6Point3f(void* header, Point3 position, float volume);
}

namespace {
inline unsigned low(const void* p) { return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(p)); }

struct Entry {
    short adjust;
    short unused;
    int (*function)(void*, int, int, void*);
};

inline int call_entry104(void* skill, int a, int b, void* c) {
    char* object = static_cast<char*>(skill);
    const Entry& entry = *reinterpret_cast<const Entry*>(*reinterpret_cast<char**>(object + 96) + 104);
    return entry.function(object + entry.adjust, a, b, c);
}

inline float unit(void* self) {
    return static_cast<float>(eRand__5Spell(self)) * 0x1p-16f * 2.0f - 1.0f;
}
}  // namespace

extern "C" void __18SpellDiseaseShieldiP8CreatureP5Skill(void* self, int kind, void* creature, void* skill) {
    unsigned char* o = static_cast<unsigned char*>(self);
    o[0] = static_cast<unsigned char>(kind);
    *reinterpret_cast<unsigned*>(o + 20) = low(vt_Spell);
    syncRandState = syncRandState * 0x19660du + 0x3c6ef35fu;
    *reinterpret_cast<unsigned*>(o + 12) = low(creature);
    *reinterpret_cast<unsigned*>(o + 8) = low(skill);
    *reinterpret_cast<unsigned*>(o + 4) = 0u;
    *reinterpret_cast<unsigned*>(o + 16) = syncRandState >> 16;
    o[0] = static_cast<unsigned char>(kind);
    *reinterpret_cast<unsigned*>(o + 20) = low(vt_SpellDiseaseShield);
    const signed char k = static_cast<signed char>(kind);
    *reinterpret_cast<int*>(o + 420) = frameNm + call_entry104(skill, 2, k, creature) * 60;
    *reinterpret_cast<int*>(o + 424) = call_entry104(skill, 1, k, creature) * 12;
    const float radius = *reinterpret_cast<const float*>(static_cast<unsigned char*>(creature) + 28);
    for (int i = 0; i < 16; ++i) {
        float* a = reinterpret_cast<float*>(o + 24 + 12 * i);
        do {
            a[0] = unit(self);
            a[1] = unit(self);
            a[2] = unit(self);
        } while (1.0f < a[0] * a[0] + a[1] * a[1] + a[2] * a[2]);
        a[2] = a[2] * radius;
        a[0] = a[0] * radius;
        a[1] = a[1] * radius;
        float* b = reinterpret_cast<float*>(o + 216 + 12 * i);
        do {
            b[0] = unit(self);
            b[1] = unit(self);
            b[2] = unit(self);
        } while (1.0f < b[0] * b[0] + b[1] * b[1] + b[2] * b[2]);
    }
    const Point3 at = *reinterpret_cast<const Point3*>(static_cast<unsigned char*>(creature) + 16);
    *reinterpret_cast<Point3*>(o + 408) = at;
    void* header = lumpFindResource__FPcT0(const_cast<char*>("fx.lmp"), const_cast<char*>("spel_de_dishield.anm"));
    soundPlaySequence__FP15AnimationHeaderG6Point3f(header, *reinterpret_cast<const Point3*>(static_cast<unsigned char*>(creature) + 16), 1.0f);
}
