// Observed setState__9MummyKingi at 0x0023ae90, 1112 bytes. R is the block at the word object+36 and
// "anim" is the animation state at +288; words and halfwords are at byte offsets of the object, and the
// state is the halfword at +572. A copy of the argument goes to setStateProcessing__8CreaturePi (which may
// change it); a non-zero result ends the call. Otherwise bit 9 (0x200) of the 64-bit word at +160 is
// cleared and the word n picks a case (n >= 18 and the states 2, 4, 7, 11, 12 do nothing more); in every
// case the low halfword of n is stored at +572 at the end. t = the word at +808, e = eRand(this).
//  0: w256 = t + e%120; transition(anim, R[68], 0.1, 3).
//  1: resetRouting; getSmartRandomDestination(point, this, 200) copied to +272; w256 = t + e%120 + 300;
//     transition(anim, R[108], 0.1, 0x803); animGlobalSync(anim).
//  3: resetRouting; when the state is not one of 3, 6, 13, 15, 8 do the same transition and sync as in 1.
//  5 and 9: transition(anim, R[68], 0.1, 3); case 9 skips it (n becomes the state) when the state is 17.
//  6: oneShot(anim, R[28], 0.1, 0.1, 0x808).
//  8: when (state is 6 and bit 3 of halfword +294 is set) or the state is 9 or 17, n becomes the state;
//     otherwise oneShot(anim, R[60], 0.1, 0.1, 16).
//  10: dropTreasure(this, point +16, halfword +56, 36, 2, 1.6, 0.6, 1.4, 0.25, "mummytongue");
//      transition(anim, R[44], 0.1, 6); the table entry at (word +72)+192 is called on the adjusted object.
//  13: byte +1160 = e & 1; oneShot(anim, R[36], 0.1, 0.1, 0x808).
//  14: transition(anim, R[92], 0.1, 3); w256 = t + 120; bit 9 of the 64-bit word is set.
//  15: w1132 = t + 300; oneShot(anim, R[84], 0.1, 0.1, 0x808).
//  16: bit 9 set; w256 = t; w1104 = t + 600; w1128 = 0; transition(anim, R[100], 0.1, 3).
//  17: w1152 = t + 600; transition(anim, R[92], 0.1, 3); a DustDevil(point +16) is built in blockAlloc(88)
//      when one is obtained; then bit 9 set, w1156 = 0, w256 = t.
// The meaning of the states is unknown. Nothing here ran on the EE.
#include "../w10-actor-j-common/access.hpp"

extern "C" {
int setStateProcessing__8CreaturePi(void* self, int* state);
void resetRouting__8Creature(void* self);
void getSmartRandomDestination__8Creaturei(void* result, void* self, int range);
unsigned eRand__10GameObject(void* self);
void animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(void* state, void* header, float time, int flag);
void animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(void* state, void* header, float a, float b, int flag);
void animGlobalSync__FP14AnimationState(void* state);
void* blockAlloc__Fi(int size);
void __9DustDevilG6Point3(void* self, Point3 position);
void dropTreasure__8CreatureG6Point3iffffffPc(void* self, Point3 position, int a, float b, float c, float d, float e, float f, float g, char* name);

void setState__9MummyKingi(void* self, int state) {
    using namespace w10j;
    unsigned char* const o = static_cast<unsigned char*>(self);
    int n = state;
    if (setStateProcessing__8CreaturePi(self, &n) != 0) return;
    dword(o, 160) &= ~0x200ull;
    unsigned char* const block = from_word(word(o, 36));
    void* const anim = o + 288;
    auto transition = [&](unsigned offset, int flag) {
        animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(anim, from_word(word(block, offset)), 0.1f, flag);
    };
    auto one_shot = [&](unsigned offset, int flag) {
        animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(anim, from_word(word(block, offset)), 0.1f, 0.1f, flag);
    };
    switch (n) {
        case 0: {
            const unsigned e = eRand__10GameObject(self);
            word(o, 256) = word(o, 808) + e % 120u;
            transition(68, 3);
            break;
        }
        case 1: {
            resetRouting__8Creature(self);
            float point[3];
            getSmartRandomDestination__8Creaturei(point, self, 200);
            real(o, 272) = point[0]; real(o, 276) = point[1]; real(o, 280) = point[2];
            const unsigned e = eRand__10GameObject(self);
            word(o, 256) = word(o, 808) + e % 120u + 300u;
            transition(108, 0x803);
            animGlobalSync__FP14AnimationState(anim);
            break;
        }
        case 3: {
            resetRouting__8Creature(self);
            const int s = half(o, 572);
            if (s != 3 && s != 6 && s != 13 && s != 15 && s != 8) {
                transition(108, 0x803);
                animGlobalSync__FP14AnimationState(anim);
            }
            break;
        }
        case 5:
            transition(68, 3);
            break;
        case 6:
            one_shot(28, 0x808);
            break;
        case 8: {
            const int s = half(o, 572);
            if ((s == 6 && (half(o, 294) & 8) != 0) || s == 9 || s == 17) {
                n = s;
            } else {
                one_shot(60, 16);
            }
            break;
        }
        case 9:
            if (half(o, 572) == 17) {
                n = 17;
            } else {
                transition(68, 3);
            }
            break;
        case 10:
            dropTreasure__8CreatureG6Point3iffffffPc(self, *reinterpret_cast<Point3*>(o + 16), half(o, 56), 36.0f, 2.0f, 1.6f, 0.6f, 1.4f, 0.25f,
                                                     const_cast<char*>("mummytongue"));
            transition(44, 6);
            call_entry(o, 192);
            break;
        case 13: {
            const unsigned e = eRand__10GameObject(self);
            o[1160] = static_cast<unsigned char>(e & 1u);
            one_shot(36, 0x808);
            break;
        }
        case 14:
            transition(92, 3);
            word(o, 256) = word(o, 808) + 120u;
            dword(o, 160) |= 0x200ull;
            break;
        case 15:
            word(o, 1132) = word(o, 808) + 300u;
            one_shot(84, 0x808);
            break;
        case 16:
            dword(o, 160) |= 0x200ull;
            word(o, 256) = word(o, 808);
            word(o, 1104) = word(o, 808) + 600u;
            word(o, 1128) = 0u;
            transition(100, 3);
            break;
        case 17: {
            word(o, 1152) = word(o, 808) + 600u;
            transition(92, 3);
            void* const devil = blockAlloc__Fi(88);
            if (devil != nullptr) __9DustDevilG6Point3(devil, *reinterpret_cast<Point3*>(o + 16));
            dword(o, 160) |= 0x200ull;
            word(o, 1156) = 0u;
            word(o, 256) = word(o, 808);
            break;
        }
        default:
            break;
    }
    half(o, 572) = static_cast<short>(n);
}
}
