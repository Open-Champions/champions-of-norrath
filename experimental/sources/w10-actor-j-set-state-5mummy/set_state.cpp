// Observed setState__5Mummyi at 0x00239008, 2196 bytes. R is the block at the word object+36, "anim" the
// animation state at +288, t the word at +808, and k the word at +960 (the "set" 0/2, 4 or other picks a
// header offset in R in most cases below, written (a|b|c)). The state is the halfword at +572. A copy of
// the argument n goes to setStateProcessing__8CreaturePi (which may change it); a non-zero result ends the
// call. Otherwise the word at +1104 is cleared, terminateSpell(this, typeinfo of SpellConeOfFire) runs, and
// n selects a case (n >= 18 and 2, 4, 7, 11, 12 do nothing more). The low halfword of n is stored at +572
// at the end. e = eRand(this).
//  16: the word at +12 loses bits 0 and 3 (&= ~9); k = 0..5 stores 44, 48, 44, 48, 46, 50 at +964 (k >= 6
//      leaves it); initType(this, 0); objectAddToRunList(this); bit 32 of the 64-bit word at +160 is set;
//      halfwords +146 = 50, +150 = 128, +152 = 255, +154 = 150, +140 = 0, +142 = 128, +144 = 255,
//      +148 = 0; n becomes 0 and the case 0 steps follow.
//  0: transition(anim, R[84|204|324], 0.1, 3); w256 = t + e%120.
//  1: resetRouting; getSmartRandomDestination(point, this, 200) is copied to +272; transition(anim,
//     R[108|252|340], 0.1, 0x803); animGlobalSync(anim).
//  3: resetRouting; the state is kept (n = 3 when the current state is 6, left as 14 when it is 14) or
//     otherwise transition(anim, R[100|244|340], 0.1, 0x803) and sync.
//  5: transition(anim, R[84|164|324], 0.1, 3); sync; w256 = t + e%120 + 60.
//  6: k = 0 or 2: oneShot(anim, R[28 + 8*(e&1)], 0.1, 0.1, 0x808); else R[116 + 8*(e%3)].
//  8: when the state is 6 or 14 and bit 3 of halfword +294 is set, or the state is 9, n becomes the state;
//     otherwise oneShot(anim, R[76|196|316], 0.1, 0.1, 16).
//  9: transition(anim, R[84], 0.3, 3).
//  10: bit 24 of the 64-bit word set: fxCreatureToDust(R[12], anim, point +16, halfword +56, float +224,
//      float +228); the word at +12 gains 0x4002; bit 41 of the 64-bit word is set; objectAddToDeleteList.
//      Otherwise, unless bit 32 is set, dropTreasure(this, point +16, halfword +56, 36, 2, 1.6, 0.6, 1.4,
//      0.25, "mummytongue") runs, then transition(anim, R[60|180|300 + 8*(e&1)], 0.3, 0x806). Either way
//      dropWeapon(this) and the table entry at (word +72)+192 follow.
//  13: transition(anim, R[84], 0.1, 3); oneShot(anim, R[260 + 8*(e%3)], 0.1, 0.1, 0x808); w256 =
//      (int)((float)e2 * 2^-16 * 120) + 240 with a second roll e2.
//  14: oneShot(anim, R[140 + 8*(e%3)], 0.1, 0.1, 0x808).
//  15: w1112 = t + 600; transition(anim, R[84], 0.1, 3); oneShot(anim, R[348], 0.1, 0.1, 0x808).
//  17: resetRouting; transition(anim, R[92|236|332], 0.1, 0x803); sync.
// The meaning of the states is unknown. Nothing here ran on the EE.
#include "../w10-actor-j-common/access.hpp"

extern "C" {
int setStateProcessing__8CreaturePi(void* self, int* state);
void terminateSpell__8CreatureRC9type_info(void* creature, const void* typeinfo);
void* __tf15SpellConeOfFire();
void resetRouting__8Creature(void* self);
void getSmartRandomDestination__8Creaturei(void* result, void* self, int range);
unsigned eRand__10GameObject(void* self);
void initType__5Mummyb(void* self, bool flag);
void objectAddToRunList__FP10GameObject(void* object);
void objectAddToDeleteList__FP10GameObject(void* object);
void animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(void* state, void* header, float time, int flag);
void animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(void* state, void* header, float a, float b, int flag);
void animGlobalSync__FP14AnimationState(void* state);
void fxCreatureToDust__FP12_modelHeaderP14AnimationStateG6Point3sff(void* model, void* animation, Point3 position, short a, float b, float c);
void dropTreasure__8CreatureG6Point3iffffffPc(void* self, Point3 position, int a, float b, float c, float d, float e, float f, float g, char* name);
void dropWeapon__5Mummy(void* self);

void setState__5Mummyi(void* self, int state) {
    using namespace w10j;
    unsigned char* const o = static_cast<unsigned char*>(self);
    int n = state;
    if (setStateProcessing__8CreaturePi(self, &n) != 0) return;
    word(o, 1104) = 0u;
    terminateSpell__8CreatureRC9type_info(self, __tf15SpellConeOfFire());
    unsigned char* const block = from_word(word(o, 36));
    void* const anim = o + 288;
    auto header = [&](unsigned offset) { return from_word(word(block, offset)); };
    // Picks one of three header offsets by the word at +960: 0 or 2, 4, or anything else.
    auto by_kind = [&](unsigned a, unsigned b, unsigned c) {
        const unsigned k = word(o, 960);
        return header(k == 0u || k == 2u ? a : k == 4u ? b : c);
    };
    auto transition = [&](void* h, float time, int flag) { animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(anim, h, time, flag); };
    auto one_shot = [&](void* h, int flag) { animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(anim, h, 0.1f, 0.1f, flag); };
    switch (n) {
        case 16:
            word(o, 12) &= ~9u;
            if (word(o, 960) < 6u) {
                static const unsigned values[6] = {44, 48, 44, 48, 46, 50};
                word(o, 964) = values[word(o, 960)];
            }
            initType__5Mummyb(self, false);
            objectAddToRunList__FP10GameObject(self);
            dword(o, 160) |= 0x100000000ull;
            half(o, 146) = 50;
            half(o, 150) = 128;
            half(o, 152) = 255;
            half(o, 154) = 150;
            n = 0;
            half(o, 140) = 0;
            half(o, 142) = 128;
            half(o, 144) = 255;
            half(o, 148) = 0;
            // fall through to the state 0 steps
        case 0: {
            transition(by_kind(84, 204, 324), 0.1f, 3);
            const unsigned e = eRand__10GameObject(self);
            word(o, 256) = word(o, 808) + e % 120u;
            break;
        }
        case 1:
        case 17: {
            resetRouting__8Creature(self);
            if (n == 1) {
                float point[3];
                getSmartRandomDestination__8Creaturei(point, self, 200);
                real(o, 272) = point[0]; real(o, 276) = point[1]; real(o, 280) = point[2];
                transition(by_kind(108, 252, 340), 0.1f, 0x803);
            } else {
                transition(by_kind(92, 236, 332), 0.1f, 0x803);
            }
            animGlobalSync__FP14AnimationState(anim);
            break;
        }
        case 3: {
            resetRouting__8Creature(self);
            const int s = half(o, 572);
            if (s == 6) break;
            if (s == 14) { n = 14; break; }
            transition(by_kind(100, 244, 340), 0.1f, 0x803);
            animGlobalSync__FP14AnimationState(anim);
            break;
        }
        case 5: {
            transition(by_kind(84, 164, 324), 0.1f, 3);
            animGlobalSync__FP14AnimationState(anim);
            const unsigned e = eRand__10GameObject(self);
            word(o, 256) = word(o, 808) + e % 120u + 60u;
            break;
        }
        case 6: {
            const unsigned k = word(o, 960);
            if (k == 0u || k == 2u) {
                const unsigned e = eRand__10GameObject(self);
                one_shot(header(28 + 8 * (e & 1u)), 0x808);
            } else {
                const unsigned e = eRand__10GameObject(self);
                one_shot(header(116 + 8 * (e % 3u)), 0x808);
            }
            break;
        }
        case 8: {
            const int s = half(o, 572);
            if (((s == 6 || s == 14) && (half(o, 294) & 8) != 0) || s == 9) {
                n = s;
            } else {
                one_shot(by_kind(76, 196, 316), 16);
            }
            break;
        }
        case 9:
            transition(header(84), 0.3f, 3);
            break;
        case 10: {
            if ((dword(o, 160) & 0x01000000ull) != 0ull) {
                fxCreatureToDust__FP12_modelHeaderP14AnimationStateG6Point3sff(header(12), anim, *reinterpret_cast<Point3*>(o + 16), half(o, 56),
                                                                               real(o, 224), real(o, 228));
                word(o, 12) |= 0x4002u;
                dword(o, 160) |= 0x20000000000ull;
                objectAddToDeleteList__FP10GameObject(self);
            } else {
                if ((dword(o, 160) & 0x100000000ull) == 0ull) {
                    dropTreasure__8CreatureG6Point3iffffffPc(self, *reinterpret_cast<Point3*>(o + 16), half(o, 56), 36.0f, 2.0f, 1.6f, 0.6f, 1.4f, 0.25f,
                                                             const_cast<char*>("mummytongue"));
                }
                const unsigned e = eRand__10GameObject(self);
                const unsigned k = word(o, 960);
                const unsigned base = (k == 0u || k == 2u) ? 60u : k == 4u ? 180u : 300u;
                transition(header(base + 8 * (e & 1u)), 0.3f, 0x806);
            }
            dropWeapon__5Mummy(self);
            call_entry(o, 192);
            break;
        }
        case 13: {
            transition(header(84), 0.1f, 3);
            const unsigned e = eRand__10GameObject(self);
            one_shot(header(260 + 8 * (e % 3u)), 0x808);
            const unsigned e2 = eRand__10GameObject(self);
            word(o, 256) = static_cast<unsigned>(static_cast<int>(static_cast<float>(static_cast<int>(e2)) * bits(0x37800000u) * bits(0x42f00000u)) + 240);
            break;
        }
        case 14: {
            const unsigned e = eRand__10GameObject(self);
            one_shot(header(140 + 8 * (e % 3u)), 0x808);
            break;
        }
        case 15:
            word(o, 1112) = word(o, 808) + 600u;
            transition(header(84), 0.1f, 3);
            one_shot(header(348), 0x808);
            break;
        default:
            break;
    }
    half(o, 572) = static_cast<short>(n);
}
}
