// Observed setState__6Minioni at 0x00234e30, 1180 bytes. R is the block at the word object+36, "anim" the
// animation state at +288, t the word at +808, and P the word at +960 (a list node whose word +12 is
// another object C). notify(v) means: when P is non-zero, the table entry at (C's word +72) + 184 is
// called on the adjusted C with v. The state is the halfword at +572. A copy of the argument n goes to
// setStateProcessing__8CreaturePi (which may change it); a non-zero result ends the call. Otherwise n
// picks a case and the low halfword of n is stored at +572 at the end. e = eRand(this).
//  0, 2, 4, 7, 11, 12 and anything not below 20 (n becomes 0): w256 = t + e%120; transition(anim, R[108],
//     0.3, 3); notify(20).
//  1: resetRouting; getSmartRandomDestination(point, this, 200) is copied to +272; w256 = t + e%120 + 300;
//     transition(anim, R[164], 0.3, 0x803); animGlobalSync(anim); notify(21).
//  3: when the current state is not 3, 6, 17, 18 or 19 do the transition and sync of case 1; then notify(21).
//  5: transition(anim, R[108], 0.1, 3); notify(20).
//  6, 17, 18, 19: oneShot(anim, R[36 + 8*(e&1)] (6) | R[52] (17) | R[60] (18) | R[28] (19), 0.1, 0.1, 0x808);
//     then w976 = 0 and w972 = n.
//  8: n becomes the current state.
//  9: transition(anim, R[108], 0.1, 3).
//  10: transition(anim, R[148], 0.3, 6); notify(23); the table entry at (word +72)+192 is called.
//  13: oneShot(anim, R[132], 0.1, 0.1, 0x808); notify(19); w976 = 0; w972 = n.
//  14: oneShot(anim, R[124], 0.1, 0.1, 0x810).
//  15: oneShot(anim, R[140], 0.1, 0.1, 0x808); notify(18); w976 = 0; w972 = n.
//  16: oneShot(anim, R[68], 0.1, 0.1, 0x808); w976 = 0; w972 = n; w980 = t + e%300 + 600.
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

void setState__6Minioni(void* self, int state) {
    using namespace w10j;
    unsigned char* const o = static_cast<unsigned char*>(self);
    int n = state;
    if (setStateProcessing__8CreaturePi(self, &n) != 0) return;
    unsigned char* const block = from_word(word(o, 36));
    void* const anim = o + 288;
    auto header = [&](unsigned offset) { return from_word(word(block, offset)); };
    auto transition = [&](unsigned offset, float time, int flag) {
        animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(anim, header(offset), time, flag);
    };
    auto one_shot = [&](unsigned offset, int flag) {
        animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(anim, header(offset), 0.1f, 0.1f, flag);
    };
    auto notify = [&](int value) {
        const unsigned node = word(o, 960);
        if (node != 0u) call_entry(from_word(word(from_word(node), 12)), 184, static_cast<unsigned long>(value));
    };
    auto settle = [&]() {
        word(o, 976) = 0u;
        sword(o, 972) = n;
    };
    switch (n) {
        case 1:
        case 3: {
            if (n == 1) {
                resetRouting__8Creature(self);
                float point[3];
                getSmartRandomDestination__8Creaturei(point, self, 200);
                real(o, 272) = point[0]; real(o, 276) = point[1]; real(o, 280) = point[2];
                const unsigned e = eRand__10GameObject(self);
                word(o, 256) = word(o, 808) + e % 120u + 300u;
                transition(164, 0.3f, 0x803);
                animGlobalSync__FP14AnimationState(anim);
            } else {
                const int s = half(o, 572);
                if (s != 3 && s != 6 && s != 17 && s != 18 && s != 19) {
                    transition(164, 0.3f, 0x803);
                    animGlobalSync__FP14AnimationState(anim);
                }
            }
            notify(21);
            break;
        }
        case 5:
            transition(108, 0.1f, 3);
            notify(20);
            break;
        case 6: {
            const unsigned e = eRand__10GameObject(self);
            one_shot(36 + 8 * (e & 1u), 0x808);
            settle();
            break;
        }
        case 17:
            one_shot(52, 0x808);
            settle();
            break;
        case 18:
            one_shot(60, 0x808);
            settle();
            break;
        case 19:
            one_shot(28, 0x808);
            settle();
            break;
        case 8:
            n = half(o, 572);
            break;
        case 9:
            transition(108, 0.1f, 3);
            break;
        case 10:
            transition(148, 0.3f, 6);
            notify(23);
            call_entry(o, 192);
            break;
        case 13:
            one_shot(132, 0x808);
            notify(19);
            settle();
            break;
        case 14:
            one_shot(124, 0x810);
            break;
        case 15:
            one_shot(140, 0x808);
            notify(18);
            settle();
            break;
        case 16: {
            one_shot(68, 0x808);
            word(o, 976) = 0u;
            const unsigned e = eRand__10GameObject(self);
            sword(o, 972) = n;
            word(o, 980) = word(o, 808) + e % 300u + 600u;
            break;
        }
        default: {
            n = 0;
            const unsigned e = eRand__10GameObject(self);
            word(o, 256) = word(o, 808) + e % 120u;
            transition(108, 0.3f, 3);
            notify(20);
            break;
        }
        case 0: {
            const unsigned e = eRand__10GameObject(self);
            word(o, 256) = word(o, 808) + e % 120u;
            transition(108, 0.3f, 3);
            notify(20);
            break;
        }
    }
    half(o, 572) = static_cast<short>(n);
}
}
