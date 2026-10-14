// Observed cast__16SkillGroundPoundiP8CreatureG6Point3s at 0x002909d8, 240 bytes in .text.
// The rank and the skill are not read. The by-value Point3 is copied to the frame and not read. When bit 0x40 of the
// word at creature+12 is clear nothing else happens. Otherwise, in order:
//  1. the word at creature+1400 is set to 10;
//  2. a resource named "dha_quake.anm" is looked up with lumpFindResource__FPcT0; its lump name is the string whose
//     address is the first word of the record that the word at creature+36 points at;
//  3. the returned header is changed: bytes +24 = 1, +32 = 2, +40 = 4, +48 = 8 and floats +20 = 0.25, +28 = 0.5,
//     +36 = 0.75, +44 = 1.4779719 (bits 0x3fbd2e2f, a pool word at gp-21292);
//  4. animAddOneShot__FP14AnimationStateP15AnimationHeaderffi is called with (creature + 288, the header, F, F, 10)
//     where F = 0.1 truncated (bits 0x3dcccccc, a pool word at gp-21288), passed as both floats;
//  5. the resource "spel_bw_pound.anm" is looked up in "fx.lmp" and played with
//     soundPlaySequence__FP15AnimationHeaderP10GameObjectf(result, creature, 1.0).
// The function returns zero in every case.
// Inferred: this plays the skill's animation with timing parameters written into the shared header, then its sound.
// Unknown: the field meanings of the header and of the creature words, the use of the word set at creature+1400, and
// whether the two pool floats are named variables. The two literals below reproduce the pool words exactly.
// Nothing here ran on the EE.
#include "../w13-skill-cast-b-common/skill_access.hpp"

extern "C" {
void* lumpFindResource__FPcT0(char* lump, char* name);
void animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(void* state, void* header, float first, float second, int mode);
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* sound, void* object, float volume);
void* cast_ground_pound(void* skill, int rank, void* creature, Point3 point, short angle) asm("cast__16SkillGroundPoundiP8CreatureG6Point3s");
}
static char animation_name[] = "dha_quake.anm";
static char lump_name[] = "fx.lmp";
static char sound_name[] = "spel_bw_pound.anm";
extern "C" void* cast_ground_pound(void*, int, void* creature, Point3, short) {
    unsigned char* c = static_cast<unsigned char*>(creature);
    if (*reinterpret_cast<unsigned*>(c + 12) & 0x40) {
        *reinterpret_cast<int*>(c + 1400) = 10;
        const unsigned record = *reinterpret_cast<unsigned*>(c + 36);
        const unsigned name_word = *reinterpret_cast<unsigned*>(w13scb::from_word(record));
        unsigned char* header = static_cast<unsigned char*>(lumpFindResource__FPcT0(reinterpret_cast<char*>(w13scb::from_word(name_word)), animation_name));
        header[32] = 2;
        *reinterpret_cast<float*>(header + 20) = 0.25f;
        header[24] = 1;
        *reinterpret_cast<float*>(header + 28) = 0.5f;
        *reinterpret_cast<float*>(header + 36) = 0.75f;
        header[40] = 4;
        *reinterpret_cast<float*>(header + 44) = 0x1.7a5c5ep0f;
        header[48] = 8;
        const float both = 0x1.999998p-4f;
        animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(c + 288, header, both, both, 10);
        soundPlaySequence__FP15AnimationHeaderP10GameObjectf(lumpFindResource__FPcT0(lump_name, sound_name), creature, 1.0f);
    }
    return nullptr;
}
