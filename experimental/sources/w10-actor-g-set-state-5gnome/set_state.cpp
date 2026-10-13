// Observed setState__5Gnomei at 0x002019a0, 760 bytes. The new state is held in a stack word whose
// address goes to the Creature state processing call (setStateProcessing__8CreaturePi), which may
// change it; a nonzero result returns at once without storing. Otherwise a state of 14 or more only
// reaches the final store, and states 0..13 select a case through the table at 0x46f3d0 (t is the
// table at word +36 holding animation header pointers; a is the member at +288; every transition
// and one-shot uses the single-precision constant with bits 0x3dcccccc):
//   0: transition to t[28] (3); +256 = word(+808) + eRand % 120.
//   1: reset routing; random destination (range 200) copied to +272..283; transition to t[156]
//      (2051); +256 = word(+808) + eRand % 120 + 120; global sync of a.
//   3: reset routing; transition to t[156] (2051); global sync of a.
//   5: transition to t[100] (3).
//   6: transition to t[100] (3); one-shot of t[60 + 8 * (eRand % 3)] (flags 8); +256 = eRand % 15.
//   8: when the halfword +572 is 6 with bit 3 (0x8) of halfword +294 set, or is 9, the state becomes
//      (short)halfword(+572); otherwise a one-shot of t[92] (flags 16).
//   9: transition to t[28] (3).
//  10: transition to t[28] (6); then the table entry at (word +72) + 192 is called with the object
//      adjusted by its signed halfword.
//  13: transition to t[124] (3).
//   2, 4, 7, 11, 12: nothing.
// Finally the halfword at +572 is set to the low 16 bits of the state.
// Unknown: what the states, the +256 timer-like word and the headers mean. Nothing here ran on the EE.
#include "../w10-actor-g-common/words.hpp"

struct Point3 { float x, y, z; };

extern "C" {
int setStateProcessing__8CreaturePi(void* self, int* state);
unsigned eRand__10GameObject(void* self);
void resetRouting__8Creature(void* self);
void getSmartRandomDestination__8Creaturei(Point3* result, void* self, int range);
void animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(void* state, void* header, float blend, int flags);
void animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(void* state, void* header, float a, float b, int flags);
void animGlobalSync__FP14AnimationState(void* state);
}

namespace {
const float kBlend = 0x1.999998p-4f;

void* header(void* object, unsigned offset) {
    unsigned char* table = w10g::from_word(w10g::word_at(object, 36));
    return w10g::from_word(*reinterpret_cast<unsigned*>(table + offset));
}
}  // namespace

extern "C" void setState__5Gnomei(void* self, int state) {
    unsigned char* o = static_cast<unsigned char*>(self);
    void* a = o + 288;
    if (setStateProcessing__8CreaturePi(self, &state)) return;
    if (static_cast<unsigned>(state) < 14u) {
        switch (state) {
        case 0: {
            animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(a, header(self, 28), kBlend, 3);
            const unsigned roll = eRand__10GameObject(self);
            *reinterpret_cast<unsigned*>(o + 256) = *reinterpret_cast<unsigned*>(o + 808) + roll % 120u;
            break;
        }
        case 1: {
            resetRouting__8Creature(self);
            Point3 destination;
            getSmartRandomDestination__8Creaturei(&destination, self, 200);
            *reinterpret_cast<Point3*>(o + 272) = destination;
            animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(a, header(self, 156), kBlend, 2051);
            const unsigned roll = eRand__10GameObject(self);
            *reinterpret_cast<unsigned*>(o + 256) = *reinterpret_cast<unsigned*>(o + 808) + roll % 120u + 120u;
            animGlobalSync__FP14AnimationState(a);
            break;
        }
        case 3:
            resetRouting__8Creature(self);
            animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(a, header(self, 156), kBlend, 2051);
            animGlobalSync__FP14AnimationState(a);
            break;
        case 5:
            animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(a, header(self, 100), kBlend, 3);
            break;
        case 6: {
            animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(a, header(self, 100), kBlend, 3);
            const unsigned pick = eRand__10GameObject(self);
            animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(a, header(self, 60 + 8 * (pick % 3u)), kBlend, kBlend, 8);
            const unsigned roll = eRand__10GameObject(self);
            *reinterpret_cast<unsigned*>(o + 256) = roll % 15u;
            break;
        }
        case 8: {
            const short current = *reinterpret_cast<short*>(o + 572);
            if ((current == 6 && (*reinterpret_cast<unsigned short*>(o + 294) & 0x8u) != 0) || current == 9) {
                state = current;
            } else {
                animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(a, header(self, 92), kBlend, kBlend, 16);
            }
            break;
        }
        case 9:
            animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(a, header(self, 28), kBlend, 3);
            break;
        case 10:
            animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(a, header(self, 28), kBlend, 6);
            w10g::fn_from_word<void (*)(void*)>(w10g::entry_function(self, 192))(w10g::adjusted(self, 192));
            break;
        case 13:
            animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(a, header(self, 124), kBlend, 3);
            break;
        default:
            break;
        }
    }
    *reinterpret_cast<unsigned short*>(o + 572) = static_cast<unsigned short>(state);
}
