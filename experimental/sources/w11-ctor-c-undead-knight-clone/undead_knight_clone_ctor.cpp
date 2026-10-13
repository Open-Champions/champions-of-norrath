// Observed __17UndeadKnightCloneG6Point3sPPc at 0x0023d798, 1004 bytes. Arguments: object, a
// position P, a short type T and an argument list V. In the order of the image:
//  - the Creature constructor runs with a copy of P, T (sign-extended), the resource map symbol
//    udkResourceMap, scale 10.0f and V; the table pointer _vt$17UndeadKnightClone goes to +72;
//  - two counted loops of two iterations with an empty body are in the image; they have no effect
//    and are not reproduced;
//  - a light record at +2496 (48 bytes) is written and passed to lightAdd__FP5Light, with the copy
//    of P stored at +2544 just before the call. Offsets from the start of the record: bytes +8, +9
//    are 0; halfwords +10 = 255, +12 = 128, +14 = 128, +16 = -16, +18 = 255, +20 = 128, +22 = 128,
//    +24 = -16; floats +28 = 120.0, +32 = bits 0x3891a2b3, +36 = 120.0, +40 = bits 0x3891a2b3; word +44 = 0;
//  - the item-description-like record at +2624 (the same shape the Mummy constructor writes at
//    +1032): halves at +2684, +2676, +2668, +2660 become -1; bytes +2656 and +2657 become 0xff;
//    halves at +2640..+2646 and +2648..+2654 (four each, step two) become 0;
//  - byte +40 becomes 20, byte +172 becomes 16, the doubleword at +160 gains 0x0000022020020000,
//    floats +28 = 20.0, +168 = bits 0x3f999999, +88 = bits 0xbe4ccccc, +96 = 0x3f199999,
//    +104 = 0x3e999999, +224 = 0x3fa66666, +100 = 1.0; the word at +248 becomes frameNm; words
//    +176, +192, +236 and halfword +572 are cleared; the words at +2696 and +2556 become 1;
//  - strcmp(levelName + 81, text) is tested against the 7-character text "battle3": equal gives
//    setParameters__8Creaturefffffi(object, 1.4, 1.2, 1.0, 1.0, 0.01, 1), anything else gives
//    (1.4, 1.2, 1.0, 1.0, 1.2, 1) (the float values are the image's single-precision words,
//    see the constants below);
//  - animInitState__FP14AnimationState on the state at +288, animAdd__FP14AnimationStateP15AnimationHeaderi
//    with the header word at offset 84 of the block that the word at +36 points to and flags 1,
//    objectAddToSlowRunList__FP10GameObject, five setAnimEventFlags__8Creatureififififi calls
//    (see the body for their arguments) and finishConstruction__8Creature, whose result is returned.
// Inferred: this is the clone of an undead knight (from the symbol), a creature with a point light.
// Unknown: what the light and the numeric parameters mean, which level the text names. Nothing
// here ran on the EE.
struct Point3 { float x, y, z; };

extern "C" {
unsigned __8CreatureG6Point3sPQ210GameObject11resourceMapfPPc(void* self, Point3 position, short type, void* resourceMap, float scale, char** arguments);
void lightAdd__FP5Light(void* light);
int strcmp(const char* left, const char* right);
void setParameters__8Creaturefffffi(void* self, float a, float b, float c, float d, float e, int flag);
void animInitState__FP14AnimationState(void* state);
void animAdd__FP14AnimationStateP15AnimationHeaderi(void* state, void* header, int flags);
void objectAddToSlowRunList__FP10GameObject(void* object);
void setAnimEventFlags__8Creatureififififi(void* self, int a, float b, int c, float d, int e, float f, int g, float h, int i);
unsigned finishConstruction__8Creature(void* self);
extern unsigned char vt_UndeadKnightClone[] __asm__("_vt$17UndeadKnightClone");
extern unsigned char udkResourceMap[];
extern char levelName[];
extern int frameNm;
}

// The host check defines NORRATH_HOST_CHECK and maps the two 32-bit address words it plants; on the
// EE the word is the address.
#if defined(NORRATH_HOST_CHECK)
extern "C" void* host_resolve(unsigned word);
#define NORRATH_RESOLVE(word) host_resolve(word)
#else
#define NORRATH_RESOLVE(word) reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

namespace {
inline float bitsToFloat(unsigned bits) { return __builtin_bit_cast(float, bits); }
inline unsigned low(const void* p) { return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(p)); }
template <class T> inline void put(unsigned char* base, unsigned offset, T value) { *reinterpret_cast<T*>(base + offset) = value; }
}

extern "C" unsigned __17UndeadKnightCloneG6Point3sPPc(void* self, Point3 position, short type, char** arguments) {
    unsigned char* o = static_cast<unsigned char*>(self);
    __8CreatureG6Point3sPQ210GameObject11resourceMapfPPc(self, position, type, udkResourceMap, 10.0f, arguments);
    put<unsigned>(o, 72, low(vt_UndeadKnightClone));

    unsigned char* light = o + 2496;
    put<Point3>(o, 2544, position);
    light[9] = 0;
    put<short>(light, 18, 255);
    put<short>(light, 22, 128);
    put<short>(light, 10, 255);
    put<short>(light, 12, 128);
    put<short>(light, 14, 128);
    put<short>(light, 20, 128);
    put<float>(light, 36, 120.0f);
    put<float>(light, 40, bitsToFloat(0x3891a2b3u));
    put<short>(light, 24, -16);
    put<float>(light, 28, 120.0f);
    put<float>(light, 32, bitsToFloat(0x3891a2b3u));
    put<unsigned>(light, 44, 0u);
    put<short>(light, 16, -16);
    light[8] = 0;
    lightAdd__FP5Light(light);

    unsigned char* item = o + 2624;
    for (int k = 0; k < 4; ++k) put<short>(o, 2684u - 8u * k, -1);
    item[33] = 0xff;
    item[32] = 0xff;
    for (unsigned offset = 16; offset < 24; offset += 2) {
        put<short>(item, offset, 0);
        put<short>(item, 8 + offset, 0);
    }

    const unsigned long long flags = *reinterpret_cast<unsigned long long*>(o + 160);
    o[40] = 20;
    o[172] = 16;
    put<float>(o, 28, 20.0f);
    put<unsigned>(o, 248, static_cast<unsigned>(frameNm));
    put<unsigned>(o, 2696, 1u);
    put<float>(o, 88, bitsToFloat(0xbe4ccccc));
    put<float>(o, 96, bitsToFloat(0x3f199999u));
    put<float>(o, 104, bitsToFloat(0x3e999999u));
    put<float>(o, 224, bitsToFloat(0x3fa66666u));
    put<unsigned>(o, 176, 0u);
    put<float>(o, 168, bitsToFloat(0x3f999999u));
    put<unsigned>(o, 192, 0u);
    put<unsigned>(o, 236, 0u);
    put<short>(o, 572, 0);
    put<unsigned>(o, 2556, 1u);
    put<float>(o, 100, 1.0f);
    *reinterpret_cast<unsigned long long*>(o + 160) = flags | 0x0000022020020000ull;

    const float scaleB = bitsToFloat(0x3f999999u);
    if (strcmp(levelName + 81, "battle3") == 0) {
        setParameters__8Creaturefffffi(self, bitsToFloat(0x3fb33333u), scaleB, 1.0f, 1.0f, bitsToFloat(0x3c23d70au), 1);
    } else {
        setParameters__8Creaturefffffi(self, bitsToFloat(0x3fb33333u), scaleB, 1.0f, 1.0f, scaleB, 1);
    }
    animInitState__FP14AnimationState(o + 288);
    const unsigned char* const block = static_cast<const unsigned char*>(NORRATH_RESOLVE(*reinterpret_cast<unsigned*>(o + 36)));
    animAdd__FP14AnimationStateP15AnimationHeaderi(o + 288, NORRATH_RESOLVE(*reinterpret_cast<const unsigned*>(block + 84)), 1);
    objectAddToSlowRunList__FP10GameObject(self);
    const float c02 = bitsToFloat(0x3e4ccccc), c06 = bitsToFloat(0x3f199999u), c03 = bitsToFloat(0x3e999999u);
    setAnimEventFlags__8Creatureififififi(self, 3, c02, 1, c06, 1, 0.0f, 0, 0.0f, 0);
    setAnimEventFlags__8Creatureififififi(self, 4, c02, 1, c06, 2, 1.0f, 2, 0.0f, 0);
    setAnimEventFlags__8Creatureififififi(self, 5, c03, 4, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    setAnimEventFlags__8Creatureififififi(self, 6, c03, 1, bitsToFloat(0x3f4ccccc), 1, bitsToFloat(0x3fcccccc), 4, 0.0f, 0);
    setAnimEventFlags__8Creatureififififi(self, 14, 0.5f, 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    return finishConstruction__8Creature(self);
}
