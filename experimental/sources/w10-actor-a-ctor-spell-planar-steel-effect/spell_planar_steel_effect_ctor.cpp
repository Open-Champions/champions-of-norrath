// Observed __22SpellPlanarSteelEffectiP8CreatureP5Skill at 0x002c8bc0, 488 bytes.
// Constructor of the spell object. Arguments: the object, a byte value, a Creature pointer and a
// Skill pointer. The Spell base constructor is inlined. In the order of the image (word offsets
// are in bytes from the object):
//  - byte +0 gets the byte value; +4 gets zero, +8 the Skill, +12 the Creature, +16 the high half
//    of one step of the sync random state (state * 1664525 + 1013904223); +20 gets the Spell
//    table and then the SpellPlanarSteelEffect table.
//  - An embedded light record at +48 is filled: bytes +8 and +9 (record offsets) are zero;
//    halves at record +10, +12, +14, +16 are 75, 75, 200, 255 and at +18, +20, +22, +24 are
//    75, 75, 200, 255; floats at +28 and +36 are zero; floats at +32 and +40 are 1.0f / 0.0f
//    (the original divides at run time; the R5900 result is bits 0x7fffffff, the largest value
//    its format has, and the reviewed compiler folds the expression to the same bits);
//    the float at +44 is 0.65f (an executable constant). The 12 bytes at Creature+16 are copied to
//    +96. Then lightAdd__FP5Light runs on the record. Byte +8 is cleared in its delay slot.
//  - The byte, Skill and Creature are stored again. +36 gets frameNm + 240, +32 gets zero, and the
//    halfword at +28 gets (short)(float at Creature+32 * 3.0f). Halfword +26 gets 32767 and
//    halfword +24 gets zero (a two-element countdown loop).
//  - lumpFindResource is called with "fx.lmp" and "spel_gatescroll.anm", and soundPlaySequence is
//    called with the result, the 12 bytes at Creature+16 passed by value, and 1.0f.
// No return value is claimed. Inferred: the light colour and range meanings. Unknown: the purpose
// of the countdown stores at +24/+26 and of the float at +32 of the Creature. Nothing here ran on the EE.
struct Point3 { float x, y, z; };

extern "C" {
extern unsigned char vt_Spell[] __asm__("_vt$5Spell");
extern unsigned char vt_SpellPlanarSteelEffect[] __asm__("_vt$22SpellPlanarSteelEffect");
extern unsigned syncRandState;
extern int frameNm;
void lightAdd__FP5Light(void* light);
void* lumpFindResource__FPcT0(char* lump, char* name);
int soundPlaySequence__FP15AnimationHeaderG6Point3f(void* header, Point3 position, float gain);
}

namespace {
unsigned low32(const void* pointer) { return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(pointer)); }
unsigned as_bits(float value) { return __builtin_bit_cast(unsigned, value); }
}

extern "C" void __22SpellPlanarSteelEffectiP8CreatureP5Skill(void* object, int kind, void* creature, void* skill) {
    unsigned char* bytes = static_cast<unsigned char*>(object);
    unsigned* words = static_cast<unsigned*>(object);
    const unsigned char* c = static_cast<const unsigned char*>(creature);
    const Point3 position = *reinterpret_cast<const Point3*>(c + 16);
    bytes[0] = static_cast<unsigned char>(kind);
    words[5] = low32(vt_Spell);
    words[1] = 0;
    words[3] = low32(creature);
    syncRandState = syncRandState * 0x19660du + 0x3c6ef35fu;
    words[2] = low32(skill);
    words[5] = low32(vt_SpellPlanarSteelEffect);
    words[4] = syncRandState >> 16;
    unsigned char* light = bytes + 48;
    unsigned short* halves = reinterpret_cast<unsigned short*>(light);
    unsigned* light_words = reinterpret_cast<unsigned*>(light);
    const float huge = 1.0f / 0.0f;
    light[9] = 0;
    reinterpret_cast<Point3*>(bytes + 96)[0] = position;
    halves[5] = 75;
    halves[6] = 75;
    halves[7] = 200;
    halves[8] = 255;
    halves[9] = 75;
    halves[10] = 75;
    halves[11] = 200;
    halves[12] = 255;
    light_words[7] = as_bits(0.0f);
    light_words[9] = as_bits(0.0f);
    light_words[8] = as_bits(huge);
    light_words[10] = as_bits(huge);
    light_words[11] = as_bits(0.65f);
    light[8] = 0;
    lightAdd__FP5Light(light);
    bytes[0] = static_cast<unsigned char>(kind);
    words[2] = low32(skill);
    words[3] = low32(creature);
    words[9] = static_cast<unsigned>(frameNm + 240);
    words[8] = 0;
    reinterpret_cast<short*>(bytes + 28)[0] = static_cast<short>(__builtin_bit_cast(float, *reinterpret_cast<const unsigned*>(c + 32)) * 3.0f);
    int value = 32767;
    for (int i = 1; i >= 0; --i) {
        reinterpret_cast<unsigned short*>(bytes + 24)[i] = static_cast<unsigned short>(value);
        value -= 32767;
    }
    soundPlaySequence__FP15AnimationHeaderG6Point3f(
        lumpFindResource__FPcT0(const_cast<char*>("fx.lmp"), const_cast<char*>("spel_gatescroll.anm")), position, 1.0f);
}
