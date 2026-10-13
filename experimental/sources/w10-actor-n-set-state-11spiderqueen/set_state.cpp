// Observed setState__11SpiderQueeni at 0x0020cf30, 1000 bytes. R is the block at the word object+36,
// "anim" the animation state at +288. A copy of the argument n goes to
// setStateProcessing__8CreaturePi (which may change it); a non-zero result ends the call with
// nothing else done. Otherwise the float at +176 is set to 120.0 and n selects a case (n >= 21 and
// 2, 4, 7, 11, 12 do nothing more); the low halfword of n is stored at +572 at the end.
// "transition" is animAddTransitionTo(anim, header, time, flags); "oneShot(h, a, b, f)" is
// animAddOneShot(anim, header, a, b, f).
//  0: e = eRand(this); word +256 = word(+808) + e%300 + 120; transition(R[92], 0.3, 3).
//  1: resetRouting; getSmartRandomDestination(point, this, 200) is copied to +272..283;
//     transition(R[148], 0.3, 3).
//  3: resetRouting; transition(R[148], 0.3, 3).
//  5: transition(R[92], 0.1, 3).
//  6: oneShot(R[36], 0.1, 0.1, 8); word +964 = 6.
//  8: with s the halfword at +572: when s is 9 or 20, or s is 6 or 18 and bit 3 of the halfword at
//     +294 is set, n becomes s. Otherwise: when the word at +576 is non-zero, its record's block
//     (record word +12) has its word at +12 tested for bit 5, and if set the word at +756 of
//     that block is cleared. Then with a the halfword at +748: a in [-8191, 8191] gives
//     oneShot(R[60], 0.1, 0.1, 16); a in [8193, 24575] gives oneShot(R[84]); a signed in
//     [-24575, -8193] gives oneShot(R[76]); anything else oneShot(R[68]).
//  9: transition(R[92], 0.3, 3).
//  10: float +28 = 65.0; dropTreasure(this, point (x + 60 cos(h), y + 60 sin(h), z) with h the
//      halfword at +56, h, 36, 1, 6, 1, 6, 1, "venomsac"); the word at +12 loses bit 0;
//      transition(R[44], 0.3, 6); when bit 1 of the word at +12 is set, animSetToEnd(anim); the
//      table entry at (word +72)+192 is called.
//  13: Web reset on the member at +968; transition(R[164], 0.1, 3); oneShot(R[172], 0.1, 0.1, 8);
//      word +256 = 0; word +964 = 13.
//  14: transition(R[164], 0.1, 3).
//  15: oneShot(R[52], 0.01, 0.1, 8); word +960 = 0; word +964 = 15.
//  16: oneShot(R[108], 0.1, 0.1, 8); word +964 = 16; word +960 = 0.
//  17: when the word at +576 is non-zero: t = iatan2(dy, dx) towards that record's block point
//      (+16, +20) from the point at +16; if the low 16 bits of t minus the halfword at +56 are
//      positive as a signed value oneShot(R[116], 0.1, 0.1, 8), otherwise oneShot(R[124]). Either
//      way word +960 = 0 and word +964 = 17.
//  18: oneShot(R[132], 0.01, 0.1, 8); word +964 = 18.
//  19: transition(R[92], 0.1, 3); oneShot(R[140], 0.1, 0.1, 8).
//  20: oneShot(R[100], 0.1, 0.1, 8); word +960 = 0.
// The meaning of the states is unknown. Nothing here ran on the EE.
#include "../w10-actor-n-common/access.hpp"

extern "C" {
int setStateProcessing__8CreaturePi(void* self, int* state);
unsigned eRand__10GameObject(void* self);
void resetRouting__8Creature(void* self);
void getSmartRandomDestination__8Creaturei(void* result, void* self, int range);
void animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(void* state, void* header, float time, int flag);
void animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(void* state, void* header, float a, float b, int flag);
void animSetToEnd__FP14AnimationState(void* state);
void dropTreasure__8CreatureG6Point3iffffffPc(void* self, Point3 position, int a, float b, float c, float d, float e, float f, float g, char* name);
float icos__Fi(int angle);
float isin__Fi(int angle);
int iatan2__Fff(float y, float x);
void reset__3Web(void* web);

void setState__11SpiderQueeni(void* self, int state) {
    using namespace w10n;
    unsigned char* const o = static_cast<unsigned char*>(self);
    int n = state;
    if (setStateProcessing__8CreaturePi(self, &n) != 0) return;
    real(o, 176) = 120.0f;
    unsigned char* const block = from_word(word(o, 36));
    void* const anim = o + 288;
    auto header = [&](unsigned offset) { return from_word(word(block, offset)); };
    auto transition = [&](unsigned offset, float time, int flag) { animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(anim, header(offset), time, flag); };
    auto one_shot = [&](unsigned offset, float a, float b, int flag) { animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(anim, header(offset), a, b, flag); };
    switch (static_cast<unsigned>(n)) {
        case 0: {
            const unsigned e = eRand__10GameObject(self);
            word(o, 256) = word(o, 808) + e % 300u + 120u;
            transition(92, 0.3f, 3);
            break;
        }
        case 1: {
            resetRouting__8Creature(self);
            float point[3];
            getSmartRandomDestination__8Creaturei(point, self, 200);
            real(o, 272) = point[0]; real(o, 276) = point[1]; real(o, 280) = point[2];
            transition(148, 0.3f, 3);
            break;
        }
        case 3:
            resetRouting__8Creature(self);
            transition(148, 0.3f, 3);
            break;
        case 5:
            transition(92, 0.1f, 3);
            break;
        case 6:
            one_shot(36, 0.1f, 0.1f, 8);
            word(o, 964) = 6u;
            break;
        case 8: {
            const int s = half(o, 572);
            if (s == 9 || s == 20 || ((s == 6 || s == 18) && (static_cast<unsigned short>(half(o, 294)) & 8u) != 0u)) {
                n = s;
                break;
            }
            if (word(o, 576) != 0u) {
                unsigned char* const target = from_word(word(from_word(word(o, 576)), 12));
                if ((word(target, 12) & 0x20u) != 0u) word(target, 756) = 0u;
            }
            const unsigned short a = static_cast<unsigned short>(half(o, 748));
            if (static_cast<unsigned short>(a + 8191u) < 16383u) {
                one_shot(60, 0.1f, 0.1f, 16);
            } else if (static_cast<unsigned short>(a - 8193u) < 16383u) {
                one_shot(84, 0.1f, 0.1f, 16);
            } else if (static_cast<short>(a) < -8192 && !(static_cast<short>(a) < -24575)) {
                one_shot(76, 0.1f, 0.1f, 16);
            } else {
                one_shot(68, 0.1f, 0.1f, 16);
            }
            break;
        }
        case 9:
            transition(92, 0.3f, 3);
            break;
        case 10: {
            real(o, 28) = 65.0f;
            const float dx = icos__Fi(half(o, 56)) * 60.0f;
            const float dy = isin__Fi(half(o, 56)) * 60.0f;
            Point3 drop;
            drop.x = real(o, 16) + dx;
            drop.y = real(o, 20) + dy;
            drop.z = real(o, 24);
            dropTreasure__8CreatureG6Point3iffffffPc(self, drop, half(o, 56), 36.0f, 1.0f, 6.0f, 1.0f, 6.0f, 1.0f, const_cast<char*>("venomsac"));
            word(o, 12) &= ~1u;
            transition(44, 0.3f, 6);
            if ((word(o, 12) & 2u) != 0u) animSetToEnd__FP14AnimationState(anim);
            call_entry(o, 192);
            break;
        }
        case 13:
            reset__3Web(o + 968);
            transition(164, 0.1f, 3);
            one_shot(172, 0.1f, 0.1f, 8);
            word(o, 256) = 0u;
            word(o, 964) = 13u;
            break;
        case 14:
            transition(164, 0.1f, 3);
            break;
        case 15:
            one_shot(52, 0.01f, 0.1f, 8);
            word(o, 960) = 0u;
            word(o, 964) = 15u;
            break;
        case 16:
            one_shot(108, 0.1f, 0.1f, 8);
            word(o, 964) = 16u;
            word(o, 960) = 0u;
            break;
        case 17: {
            if (word(o, 576) != 0u) {
                unsigned char* const target = from_word(word(from_word(word(o, 576)), 12));
                const float dy = real(target, 20) - real(o, 20);
                const float dx = real(target, 16) - real(o, 16);
                const int heading = iatan2__Fff(dy, dx);
                if (static_cast<short>(static_cast<unsigned short>(heading) - static_cast<unsigned short>(half(o, 56))) > 0) {
                    one_shot(116, 0.1f, 0.1f, 8);
                } else {
                    one_shot(124, 0.1f, 0.1f, 8);
                }
            }
            word(o, 960) = 0u;
            word(o, 964) = 17u;
            break;
        }
        case 18:
            one_shot(132, 0.01f, 0.1f, 8);
            word(o, 964) = 18u;
            break;
        case 19:
            transition(92, 0.1f, 3);
            one_shot(140, 0.1f, 0.1f, 8);
            break;
        case 20:
            one_shot(100, 0.1f, 0.1f, 8);
            word(o, 960) = 0u;
            break;
        default:
            break;
    }
    half(o, 572) = static_cast<short>(n);
}
}
