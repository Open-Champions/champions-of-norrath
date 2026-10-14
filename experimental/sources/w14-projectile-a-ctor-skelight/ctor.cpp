// Observed __8SkelightG6Point3sPPcci at 0x00246310, 648 bytes. Arguments: the object, a position P (by value), a short type T, an
// argument list L, a signed byte F and an integer N. Returns nothing. In order:
//  1. The Creature constructor runs with (object, P, T, skelightResourceMap, 10.0, L). _vt$8Skelight goes to +72; the halfwords at
//     +1092, +1084, +1076 and +1068 become -1, the bytes +1064 and +1065 become 255 and the eight halfwords at +1048..+1054 and
//     +1056..+1062 become 0.
//  2. Byte +40 = 20, the 64-bit value at +160 gets 0x10000 OR-ed in, frameNm goes to +248, the single 0x3f999999 to +168, 15.0 to +28,
//     the word +960 = 4, the words +176, +192 and +236 and the halfword +572 are cleared, and 1.0 goes to +96 and +100.
//  3. When F is nonzero, eRand__10GameObject(object) % 3 + 1 (unsigned) goes to +208. initWeapon__8Skelight(object) runs.
//  4. N goes to +1104. worldDifficulty is saved, set to float(N), setParameters__8Creaturefffffi(object, 1.0 x5, 1) runs and the saved
//     value is restored. +244 is cleared, then modelGetActiveChunks__FP12_modelHeaderi(the word at (word +36) + 12, 0) goes to +552
//     as a 64-bit value, stored before animInitState runs.
//  5. animInitState(object + 288), animAdd(object + 288, the word at (word +36) + 60, 12), setState__8Skelighti(object, 13) and
//     objectAddToRunList run, the halfword +792 becomes -1 and two setAnimEventFlags__8Creatureififififi calls follow, both with the
//     single 0x3e4ccccc: (object, 3, S, 1, 0.0, 0, 0.0, 0, 0.0, 0) and (object, 4, S, 1, 1.0, 1, 0.0, 0, 0.0, 0). finishConstruction
//     runs and the word +236 becomes 1.
// Inferred: the Creature layout from the Skeleton constructor (same offsets for +1064..+1108 weapon slots). Unknown: the meaning of F
// and N beyond what is stored, of the 0x10000 bit and of the numbers 20, 15.0 and 4. Nothing here ran on the EE.
#include "../w14-projectile-a-common/access.hpp"

extern "C" {
extern unsigned char skelightResourceMap[];
extern int frameNm;
extern float worldDifficulty;
extern unsigned char vt_Skelight[] __asm__("_vt$8Skelight");
void creature_construct(void* self, Point3 position, short type, void* map, float scale, char** arguments)
    __asm__("__8CreatureG6Point3sPQ210GameObject11resourceMapfPPc");
unsigned eRand__10GameObject(void* object);
void initWeapon__8Skelight(void* self);
void setParameters__8Creaturefffffi(void* self, float a, float b, float c, float d, float e, int flag);
unsigned long long modelGetActiveChunks__FP12_modelHeaderi(void* header, int mask);
void animInitState__FP14AnimationState(void* state);
void animAdd__FP14AnimationStateP15AnimationHeaderi(void* state, void* header, int flags);
void setState__8Skelighti(void* self, int state);
void objectAddToRunList__FP10GameObject(void* object);
void setAnimEventFlags__8Creatureififififi(void* self, int a, float fa, int b, float fb, int c, float fc, int d, float fd, int e);
void finishConstruction__8Creature(void* self);
void __8SkelightG6Point3sPPcci(void* self, Point3 position, int type, char** arguments, int flag, int n);
}

// type and flag are declared as ints so that the sign extension the image performs on entry (halfword and byte) stays explicit.
extern "C" void __8SkelightG6Point3sPPcci(void* self, Point3 position, int type, char** arguments, int flag, int n) {
    using namespace w14pa;
    unsigned char* const o = static_cast<unsigned char*>(self);
    creature_construct(self, position, static_cast<short>(type), skelightResourceMap, 10.0f, arguments);
    word(o, 72) = to_word(vt_Skelight);
    for (int i = 3; i >= 0; i--) half(o, 1068 + 8 * i) = -1;
    o[1065] = 0xff;
    o[1064] = 0xff;
    for (int i = 0; i < 4; i++) {
        half(o, 1048 + 2 * i) = 0;
        half(o, 1056 + 2 * i) = 0;
    }
    o[40] = 20;
    const unsigned long long extra = dword(o, 160) | 0x10000ull;
    word(o, 248) = static_cast<unsigned>(frameNm);
    word(o, 168) = 0x3f999999u;
    real(o, 28) = 15.0f;
    dword(o, 160) = extra;
    word(o, 960) = 4;
    word(o, 176) = 0;
    word(o, 192) = 0;
    word(o, 236) = 0;
    half(o, 572) = 0;
    real(o, 100) = 1.0f;
    real(o, 96) = 1.0f;
    if (static_cast<signed char>(flag) != 0) word(o, 208) = eRand__10GameObject(self) % 3u + 1u;
    initWeapon__8Skelight(self);
    word(o, 1104) = static_cast<unsigned>(n);
    const float saved = worldDifficulty;
    worldDifficulty = static_cast<float>(n);
    setParameters__8Creaturefffffi(self, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1);
    worldDifficulty = saved;
    word(o, 244) = 0;
    const unsigned long long chunks = modelGetActiveChunks__FP12_modelHeaderi(from_word(word(from_word(word(o, 36)), 12)), 0);
    dword(o, 552) = chunks;
    animInitState__FP14AnimationState(o + 288);
    animAdd__FP14AnimationStateP15AnimationHeaderi(o + 288, from_word(word(from_word(word(o, 36)), 60)), 12);
    setState__8Skelighti(self, 13);
    objectAddToRunList__FP10GameObject(self);
    half(o, 792) = -1;
    setAnimEventFlags__8Creatureififififi(self, 3, bits(0x3e4ccccc), 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    setAnimEventFlags__8Creatureififififi(self, 4, bits(0x3e4ccccc), 1, 1.0f, 1, 0.0f, 0, 0.0f, 0);
    finishConstruction__8Creature(self);
    word(o, 236) = 1;
}
