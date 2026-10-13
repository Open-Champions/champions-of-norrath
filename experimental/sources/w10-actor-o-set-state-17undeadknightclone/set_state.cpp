// Observed setState__17UndeadKnightClonei at 0x0023f470, 824 bytes. R is the block at the word
// object+36, "anim" the animation state at +288, h the signed halfword at +572, wait the word at +256,
// base the word at +808. A copy n of the argument goes to setStateProcessing__8CreaturePi (which may
// change it); a non-zero result ends the call with nothing else done. Otherwise n selects a case
// (the jump table has 18 words). "transition" is animAddTransitionTo(anim, header, rate, flags) and
// "oneShot" is animAddOneShot(anim, header, rate, rate, 0x808); every rate is the .lit4 word
// 0x3dcccccc. "setState(3)" is the table entry at (word +72) + 184 called with 3. At the end the low
// halfword of n is stored at +572.
//  0: wait = base + eRand % 120; transition(R[84], 3).
//  1: resetRouting; getSmartRandomDestination(point, this, 200) goes to +272..283;
//     transition(R[108], 0x803); animGlobalSync(anim).
//  3: resetRouting; transition(R[108], 0x803); animGlobalSync(anim).
//  5: transition(R[60], 3).
//  8, 9: n becomes h.
//  10: wait = frameNm + 3; bits 0x400d of the word at +12 are set; transition(R[124], 6); the table
//      entry at (word +72) + 192 is called.
//  13: oneShot(R[116]).
//  14: transition(R[60], 3); oneShot(R[28]); wait = eRand % 20.
//  15: transition(R[60], 3); oneShot(R[36]); wait = eRand % 20.
//  16: transition(R[60], 3); oneShot(R[44]); wait = eRand % 20.
//  17: transition(R[60], 3); oneShot(R[52]); wait = eRand % 20.
//  2, 4, 6, 7, 11, 12 and n >= 18 (unsigned): setState(3) is called and n is kept.
// The meaning of the states is unknown. Nothing here ran on the EE.
#include "../w10-actor-o-common/access.hpp"

extern "C" {
int setStateProcessing__8CreaturePi(void* self, int* state);
void resetRouting__8Creature(void* self);
void getSmartRandomDestination__8Creaturei(void* result, void* self, int range);
void animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(void* state, void* header, float rate, int flag);
void animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(void* state, void* header, float a, float b, int flag);
void animGlobalSync__FP14AnimationState(void* state);
unsigned eRand__10GameObject(void* object);
extern int frameNm;

void setState__17UndeadKnightClonei(void* self, int state) {
    using namespace w10o;
    unsigned char* const o = static_cast<unsigned char*>(self);
    int n = state;
    if (setStateProcessing__8CreaturePi(self, &n) != 0) return;
    unsigned char* const block = from_word(word(o, 36));
    void* const anim = o + 288;
    const unsigned fast = 0x3dccccccu;
    auto header = [&](unsigned offset) { return from_word(word(block, offset)); };
    auto transition = [&](unsigned offset, int flag) {
        animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(anim, header(offset), bits(fast), flag);
    };
    auto one_shot = [&](unsigned offset) {
        animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(anim, header(offset), bits(fast), bits(fast), 0x808);
    };
    auto roll_wait = [&]() {
        const unsigned r = eRand__10GameObject(self);
        word(o, 256) = r % 20u;
    };
    switch (static_cast<unsigned>(n)) {
        case 0: {
            const unsigned r = eRand__10GameObject(self);
            word(o, 256) = word(o, 808) + r % 120u;
            transition(84, 3);
            break;
        }
        case 1: {
            resetRouting__8Creature(self);
            Point3 point;
            getSmartRandomDestination__8Creaturei(&point, self, 200);
            real(o, 272) = point.x; real(o, 276) = point.y; real(o, 280) = point.z;
            transition(108, 0x803);
            animGlobalSync__FP14AnimationState(anim);
            break;
        }
        case 3:
            resetRouting__8Creature(self);
            transition(108, 0x803);
            animGlobalSync__FP14AnimationState(anim);
            break;
        case 5:
            transition(60, 3);
            break;
        case 14:
            transition(60, 3);
            one_shot(28);
            roll_wait();
            break;
        case 15:
            transition(60, 3);
            one_shot(36);
            roll_wait();
            break;
        case 16:
            transition(60, 3);
            one_shot(44);
            roll_wait();
            break;
        case 17:
            transition(60, 3);
            one_shot(52);
            roll_wait();
            break;
        case 13:
            one_shot(116);
            break;
        case 8:
        case 9:
            n = half(o, 572);
            break;
        case 10:
            word(o, 256) = static_cast<unsigned>(frameNm + 3);
            word(o, 12) = word(o, 12) | 0x400du;
            transition(124, 6);
            call_entry(o, 192);
            break;
        default:
            call_entry(o, 184, 3);
            break;
    }
    half(o, 572) = static_cast<short>(n);
}
}
