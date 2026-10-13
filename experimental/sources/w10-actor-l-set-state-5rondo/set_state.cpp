#include "../w10-actor-l-common/access.hpp"
// Observed setState__5Rondoi at 0x00232e48, 708 bytes. The state travels in a local int whose
// address goes to setStateProcessing__8CreaturePi (which may change it); a nonzero result ends the
// call at once with no store. Otherwise the state read back selects a case, and every case ends by
// storing the low 16 bits of the local at object+572 (a case may change the local first). States
// 2, 3, 4 and 11 or more do nothing else (the image also stores zero to +572 first on those
// paths; the final store overwrites it, so it is not reproduced). "Animation state" is the member
// at object+288 and "group" the table whose address is the word at object+36; the group words
// are the animation headers. Every rate is the constant 0.1.
//   0: roll = eRand(object) % 120; word at +256 = word at +808 + roll + 120 (stored first). Then,
//      when the word at +964 is zero, animAddTransitionTo(state, group[44], 0.1, 3), else
//      animAddTransitionTo(state, group[52], 0.1, 3).
//   1: resetRouting; getSmartRandomDestination(result, object, 200) fills a Point3 that is copied
//      to object+272; roll = eRand % 120; word at +256 = word at +808 + roll + 300 (stored before
//      the transition); when the doubleword at +160 has any bit of 0x00c00000 set the transition is
//      animAddTransitionTo(state, group[76], 0.1, 2051), else group[108]; then
//      animGlobalSync(state).
//   5, 9: animAddTransitionTo(state, group[52], 0.1, 3).
//   6: animAddOneShot(state, group[28], 0.1, 0.1, 8).
//   7: enemy = alternateEnemyInZone(object, signed halfword at +56, 32767, 0.0, 100.0). If it is
//      nonzero: destroy the list at +960, link the enemy with listOnewayLink(list, enemy, enemy),
//      animAddTransitionTo(state, group[76], 0.1, 2051) and word at +256 = word at +808 + 120;
//      otherwise the local becomes the signed halfword currently at +572.
//   8: if the signed halfword at +572 is 9 the local becomes 9, else animAddOneShot(state,
//      group[36], 0.1, 0.1, 16).
//  10: the table at object+72, entry at table offset 192, is called with the adjusted object only;
//      then animAddTransitionTo(state, group[44], 0.1, 6).
// Unknown: state names; meaning of the words at +160, +256, +272, +808, +964 and the list at +960.
// Nothing here ran on the EE.
extern "C" {
int setStateProcessing__8CreaturePi(void* self, int* state);
unsigned eRand__10GameObject(void* self);
void resetRouting__8Creature(void* self);
void getSmartRandomDestination__8Creaturei(void* result, void* self, int range);
void animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(void* state, void* header, float rate, int flags);
void animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(void* state, void* header, float first, float second, int flags);
void animGlobalSync__FP14AnimationState(void* state);
void* alternateEnemyInZone__8Creatureffss(void* self, float a, float b, short c, short d);
void listDestroy__FP8ListHead(void* list);
void listOnewayLink__FP8ListHeadT0Pv(void* list, void* head, void* item);

void setState__5Rondoi(void* self, int requested) {
    unsigned char* const o = static_cast<unsigned char*>(self);
    int state = requested;
    if (setStateProcessing__8CreaturePi(self, &state) != 0) return;
    void* const animation = o + 288;
    auto header = [&](unsigned offset) {
        return w10l::from_word(w10l::word(w10l::from_word(w10l::word(o, 36)), offset));
    };
    switch (static_cast<unsigned>(state)) {
    case 0: {
        const unsigned roll = eRand__10GameObject(self) % 120u;
        w10l::word(o, 256) = w10l::word(o, 808) + roll + 120u;
        if (w10l::word(o, 964) != 0)
            animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(animation, header(52), 0.1f, 3);
        else
            animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(animation, header(44), 0.1f, 3);
        break;
    }
    case 1: {
        resetRouting__8Creature(self);
        Point3 destination;
        getSmartRandomDestination__8Creaturei(&destination, self, 200);
        *reinterpret_cast<Point3*>(o + 272) = destination;
        const unsigned roll = eRand__10GameObject(self) % 120u;
        w10l::word(o, 256) = w10l::word(o, 808) + roll + 300u;
        if ((w10l::dword(o, 160) & 0x00c00000ull) != 0)
            animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(animation, header(76), 0.1f, 2051);
        else
            animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(animation, header(108), 0.1f, 2051);
        animGlobalSync__FP14AnimationState(animation);
        break;
    }
    case 5:
    case 9:
        animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(animation, header(52), 0.1f, 3);
        break;
    case 6:
        animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(animation, header(28), 0.1f, 0.1f, 8);
        break;
    case 7: {
        void* const enemy = alternateEnemyInZone__8Creatureffss(self, 0.0f, 100.0f, w10l::half(o, 56), 32767);
        if (enemy != nullptr) {
            listDestroy__FP8ListHead(o + 960);
            listOnewayLink__FP8ListHeadT0Pv(o + 960, enemy, enemy);
            animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(animation, header(76), 0.1f, 2051);
            w10l::word(o, 256) = w10l::word(o, 808) + 120u;
        } else {
            state = w10l::half(o, 572);
        }
        break;
    }
    case 8:
        if (w10l::half(o, 572) == 9) state = 9;
        else animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(animation, header(36), 0.1f, 0.1f, 16);
        break;
    case 10:
        w10l::call_entry(o, 192);
        animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(animation, header(44), 0.1f, 6);
        break;
    default:
        break;
    }
    w10l::half(o, 572) = static_cast<short>(state);
}
}
