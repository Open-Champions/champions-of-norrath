// Observed __8SkeletonG6Point3sPPcPci at 0x00243868, 2640 bytes: the Skeleton constructor. Arguments: the object, a position
// P (by value), a short type T, an argument list, a name string and an int K (the resource-map kind). Returns nothing
// meaningful. R() is eRand__10GameObject(object). In order:
//  1. The Creature constructor runs with (object, P, T, skeletonResourceMap + 64 * K bytes, 10.0, argument list). Words
//     +960 and +964 are cleared, _vt$8Skeleton goes to +72, the halfwords at +1108, +1100, +1092 and +1084 become -1, bytes
//     +1080 and +1081 become -1 and the eight halfwords at +1064..+1070 and +1072..+1078 become 0.
//  2. skeletonResMap is a table of words: [0] is a lump-name pointer, [1] a stamp, then pairs (name pointer, resource) from
//     [2], ended by a null name. When [1] differs from resourceFrame, every pair's resource becomes
//     lumpFindResource__FPcT0(lump, name); [1] is then set to resourceFrame (re-read after the loop).
//  3. Byte +40 = 20, the 64-bit value at +160 gets 0x10000 OR-ed in, byte +172 = 16, the single 0x3f999999 goes to +168, 18.0
//     to +28, frameNm to +248, 1.0 to +96 and +100, and +176, +192, +236 and the halfword at +572 are cleared.
//  4. R() % 5 (unsigned) goes to word +968 and byte +972 is cleared. The name is then compared case-blind (strticmp__FPCcT0)
//     with seventeen texts, in this order, each of which sets (byte +972, word +968): "skeletonred" (2, keep), "skeletonblue"
//     (1, keep), "skeletonswordohwred" (2, 0), "...ohwblue" (1, 0), "skeletonsworddhwred" (2, 1), "...dhwblue" (1, 1),
//     "skeletonaxered" (2, 2), "...axeblue" (1, 2), "skeletonspearred" (2, 3), "...spearblue" (1, 3), "skeletonbowred" and
//     "skeletonbowblue" (both 2, 4), then "skeletonswordohw" (keep, 0), "skeletonsworddhw" (keep, 1), "skeletonaxe" (keep, 2),
//     "skeletonspear" (keep, 3) and "skeletonbow" (keep, 4). The first match wins.
//  5. The word +208 is cleared. When +968 is 0..4: a roll f = (float)(int)R() * 2^-16 is compared with a per-variant single
//     (0x3dccca2d, 0x3dccca2d, 0x3dcccab3, 0x3dcccccc, 0x3dcccbc0 for 0..4) and below it:
//       0: +208 = 1; +976 = 77 if R() & 1 else 79; +1120 = 91 + 2 * (R() & 1).
//       1: +208 = 1; +976 = 89 (no further roll).
//       2: +208 = 1; +976 = 85 if R() & 1 else 87.
//       3: +208 = (R() & 1) + 1; +976 = 95 if R() & 1 else 97 (that second roll is taken whether or not the test passed).
//       4: +208 = (R() & 3) + 1; +976 = 83 and +1124 = 0 either way.
//  6. +244 is cleared; s = R(); when s & 1, +244 |= 1 << (R() % 3). When +968 == 4, +244 |= 0x80; otherwise when s & 2,
//     +244 |= 1 << (R() % 12 + 9). Unless bit 14 of +244 is set, +244 |= 1 << (R() % 5 + 21). Then +244 |= 1 << ((R() & 1) + 5).
//  7. The 64-bit result of modelGetActiveChunks__FP12_modelHeaderi(the word at (word +36) + 12, +244) goes to +552.
//     initType__8Skeletonb(object, 0), animInitState__FP14AnimationState(object + 288) and animAdd__FP14AnimationStateP15
//     AnimationHeaderi(object + 288, the word at skeletonResMap + 104 * (+968) + 68, 1) run, then setState__8Skeletoni(object,
//     0) and objectAddToSlowRunList. Fifteen setAnimEventFlagsResMap calls (object, skeletonResMap, index, ...) follow, with
//     the indices 1, 2, 3, 14, 15, 16, 27, 28, 29, 40, 41, 42, 53, 54, 55; see the table in the code. finishConstruction last.
// Inferred: the seventeen texts name armour/weapon colour variants. Unknown: the meaning of the fields. The tail calls'
// numeric arguments are as observed. Nothing here ran on the EE.
#include "../w10-actor-m-common/access.hpp"

extern "C" {
extern unsigned char skeletonResourceMap[];
extern unsigned skeletonResMap[];
extern unsigned resourceFrame;
extern int frameNm;
extern unsigned char vt_Skeleton[] __asm__("_vt$8Skeleton");
void creature_construct(void* self, Point3 position, short type, void* map, float scale, char** arguments)
    __asm__("__8CreatureG6Point3sPQ210GameObject11resourceMapfPPc");
void* lumpFindResource__FPcT0(char* lump, char* name);
unsigned eRand__10GameObject(void* object);
int strticmp__FPCcT0(const char* a, const char* b);
unsigned long long modelGetActiveChunks__FP12_modelHeaderi(void* header, int mask);
void initType__8Skeletonb(void* self, bool flag);
void animInitState__FP14AnimationState(void* state);
void animAdd__FP14AnimationStateP15AnimationHeaderi(void* state, void* header, int flags);
void setState__8Skeletoni(void* self, int state);
void objectAddToSlowRunList__FP10GameObject(void* object);
void setAnimEventFlagsResMap__8CreaturePQ210GameObject11resourceMapififififi(void* self, void* map, int a, float fa, int b, float fb, int c, float fc,
                                                                              int d, float fd, int e);
void finishConstruction__8Creature(void* self);
void __8SkeletonG6Point3sPPcPci(void* self, Point3 position, short type, char** arguments, char* name, int kind);
}

namespace {
inline float roll_fraction(void* self) { return static_cast<float>(static_cast<int>(eRand__10GameObject(self))) * w10m::bits(0x37800000u); }
inline bool matches(const char* name, const char* text) { return strticmp__FPCcT0(name, text) == 0; }
}

extern "C" void __8SkeletonG6Point3sPPcPci(void* self, Point3 position, short type, char** arguments, char* name, int kind) {
    using namespace w10m;
    unsigned char* o = static_cast<unsigned char*>(self);
    creature_construct(self, position, type, skeletonResourceMap + (kind << 6), 10.0f, arguments);
    word(o, 960) = 0;
    word(o, 964) = 0;
    word(o, 72) = to_word(vt_Skeleton);
    for (int i = 3; i >= 0; i--) *reinterpret_cast<short*>(o + 1084 + 8 * i) = -1;
    o[1081] = 0xff;
    o[1080] = 0xff;
    for (int i = 0; i < 4; i++) {
        *reinterpret_cast<unsigned short*>(o + 1064 + 2 * i) = 0;
        *reinterpret_cast<unsigned short*>(o + 1072 + 2 * i) = 0;
    }
    if (skeletonResMap[1] != resourceFrame) {
        for (unsigned* entry = skeletonResMap + 2; entry[0] != 0; entry += 2)
            entry[1] = to_word(lumpFindResource__FPcT0(reinterpret_cast<char*>(from_word(skeletonResMap[0])), reinterpret_cast<char*>(from_word(entry[0]))));
        skeletonResMap[1] = resourceFrame;
    }
    o[40] = 20;
    dword(o, 160) = dword(o, 160) | 0x10000ull;
    real(o, 168) = bits(0x3f999999u);
    real(o, 28) = 18.0f;
    word(o, 248) = static_cast<unsigned>(frameNm);
    o[172] = 16;
    real(o, 96) = 1.0f;
    word(o, 176) = 0;
    word(o, 192) = 0;
    word(o, 236) = 0;
    *reinterpret_cast<unsigned short*>(o + 572) = 0;
    real(o, 100) = 1.0f;
    word(o, 968) = eRand__10GameObject(self) % 5u;
    o[972] = 0;
    if (matches(name, "skeletonred")) {
        o[972] = 2;
    } else if (matches(name, "skeletonblue")) {
        o[972] = 1;
    } else if (matches(name, "skeletonswordohwred")) {
        word(o, 968) = 0;
        o[972] = 2;
    } else if (matches(name, "skeletonswordohwblue")) {
        word(o, 968) = 0;
        o[972] = 1;
    } else if (matches(name, "skeletonsworddhwred")) {
        word(o, 968) = 1;
        o[972] = 2;
    } else if (matches(name, "skeletonsworddhwblue")) {
        word(o, 968) = 1;
        o[972] = 1;
    } else if (matches(name, "skeletonaxered")) {
        word(o, 968) = 2;
        o[972] = 2;
    } else if (matches(name, "skeletonaxeblue")) {
        word(o, 968) = 2;
        o[972] = 1;
    } else if (matches(name, "skeletonspearred")) {
        word(o, 968) = 3;
        o[972] = 2;
    } else if (matches(name, "skeletonspearblue")) {
        word(o, 968) = 3;
        o[972] = 1;
    } else if (matches(name, "skeletonbowred") || matches(name, "skeletonbowblue")) {
        word(o, 968) = 4;
        o[972] = 2;
    } else if (matches(name, "skeletonswordohw")) {
        word(o, 968) = 0;
    } else if (matches(name, "skeletonsworddhw")) {
        word(o, 968) = 1;
    } else if (matches(name, "skeletonaxe")) {
        word(o, 968) = 2;
    } else if (matches(name, "skeletonspear")) {
        word(o, 968) = 3;
    } else if (matches(name, "skeletonbow")) {
        word(o, 968) = 4;
    }
    word(o, 208) = 0;
    switch (word(o, 968)) {
        case 0:
            if (roll_fraction(self) < bits(0x3dccca2du)) word(o, 208) = 1;
            word(o, 976) = (eRand__10GameObject(self) & 1u) != 0 ? 77u : 79u;
            word(o, 1120) = 91u + 2u * (eRand__10GameObject(self) & 1u);
            break;
        case 1:
            if (roll_fraction(self) < bits(0x3dccca2du)) word(o, 208) = 1;
            word(o, 976) = 89;
            break;
        case 2:
            if (roll_fraction(self) < bits(0x3dcccab3u)) word(o, 208) = 1;
            word(o, 976) = (eRand__10GameObject(self) & 1u) != 0 ? 85u : 87u;
            break;
        case 3:
            if (roll_fraction(self) < bits(0x3dccccccu)) word(o, 208) = (eRand__10GameObject(self) & 1u) + 1u;
            word(o, 976) = (eRand__10GameObject(self) & 1u) != 0 ? 95u : 97u;
            break;
        case 4:
            if (roll_fraction(self) < bits(0x3dcccbc0u)) word(o, 208) = (eRand__10GameObject(self) & 3u) + 1u;
            word(o, 1124) = 0;
            word(o, 976) = 83;
            break;
        default:
            break;
    }
    word(o, 244) = 0;
    const unsigned s = eRand__10GameObject(self);
    if ((s & 1u) != 0) word(o, 244) |= 1u << (eRand__10GameObject(self) % 3u);
    if (word(o, 968) == 4) {
        word(o, 244) |= 0x80u;
    } else if ((s & 2u) != 0) {
        word(o, 244) |= 1u << (eRand__10GameObject(self) % 12u + 9u);
    }
    if ((word(o, 244) & 0x4000u) == 0) word(o, 244) |= 1u << (eRand__10GameObject(self) % 5u + 21u);
    word(o, 244) |= 1u << ((eRand__10GameObject(self) & 1u) + 5u);
    unsigned char* const model = from_word(word(o, 36));
    dword(o, 552) = modelGetActiveChunks__FP12_modelHeaderi(from_word(*reinterpret_cast<unsigned*>(model + 12)), static_cast<int>(word(o, 244)));
    initType__8Skeletonb(self, false);
    animInitState__FP14AnimationState(o + 288);
    animAdd__FP14AnimationStateP15AnimationHeaderi(o + 288, from_word(skeletonResMap[(104u * word(o, 968) + 68u) / 4u]), 1);
    setState__8Skeletoni(self, 0);
    objectAddToSlowRunList__FP10GameObject(self);
    void* const map = skeletonResMap;
    const float a = bits(0x3e999999u), b = bits(0x3f666666u), c = bits(0x3e4cccccu), d = bits(0x3fa66666u);
    static const int plain[] = {1, 2, 3, 14, 15, 16};
    for (int index : plain) setAnimEventFlagsResMap__8CreaturePQ210GameObject11resourceMapififififi(self, map, index, a, 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    setAnimEventFlagsResMap__8CreaturePQ210GameObject11resourceMapififififi(self, map, 27, a, 1, b, 1, 0.0f, 0, 0.0f, 0);
    setAnimEventFlagsResMap__8CreaturePQ210GameObject11resourceMapififififi(self, map, 28, a, 1, b, 1, 0.0f, 0, 0.0f, 0);
    static const int plain_two[] = {29, 40, 41, 42};
    for (int index : plain_two) setAnimEventFlagsResMap__8CreaturePQ210GameObject11resourceMapififififi(self, map, index, a, 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    for (int index = 53; index <= 55; ++index)
        setAnimEventFlagsResMap__8CreaturePQ210GameObject11resourceMapififififi(self, map, index, c, 2, d, 1, 0.0f, 0, 0.0f, 0);
    finishConstruction__8Creature(self);
}
