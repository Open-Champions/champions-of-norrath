#include "../w10-actor-l-common/access.hpp"
// Observed setState__8Scorpioni at 0x001f29c8, 1020 bytes. The state travels in a local int whose
// address goes to setStateProcessing__8CreaturePi (which may change it); a nonzero result ends the
// call at once with no store. Otherwise the state value read back after that call selects a case
// (values of 16 or more, and 2, 4, 7, 11, 12, 13, do nothing but the final store). Every case
// ends by storing the low 16 bits of the local at object+572 (a case may change the local first).
// "Animation state" is the member at object+288; "group" is the table whose address is the word at
// object+36, and its words are the animation headers indexed below. The rates are the constants
// 0.3f, 0.1f and the ones written in the code.
//   0: roll = eRand(object) % 60; word at +256 = word at +808 + roll + 120 (stored before the
//      transition call); animAddTransitionTo(state, group[132], 0.3, 3).
//   1: resetRouting; getSmartRandomDestination(result, object, 200) fills a Point3 that is copied
//      to object+272; animAddTransitionTo(state, group[148], 0.3, 2051).
//   3: resetRouting; animAddTransitionTo(state, group[156], 0.3, 2051).
//   5: animAddTransitionTo(state, group[140], 0.1, 3).
//   6: animAddTransitionTo(state, group[140], 0.1, 3); then if eRand % 6 == 0: animAddOneShot(
//      state, group[68], 0.1, 0.1, 8) and the local becomes 13; else animAddOneShot(state,
//      group[(eRand % 3) * 8 + 44], 0.1, 0.1, 8). Last, word at +256 = eRand % 20.
//   8: if the signed halfword at +572 is 9, or is 6 with bit 8 of the halfword at +294 set,
//      the local becomes that halfword; else animAddOneShot(state, group[124], 0.1, 0.1, 16).
//   9: animAddTransitionTo(state, group[132], 0.3, 3).
//  10: if eRand % 3 == 0: loseBodyPart(object, eRand % 6). Then dropTreasure(object, copy of the
//      Point3 at +16, signed halfword at +56, 36.0, 2.0, 1.6, 0.6, 1.4, 0.25, text at 0x46e0c0);
//      animAddTransitionTo(state, group[(eRand & 1) * 8 + 92], 0.3, 2054); then the table at
//      object+72 entry at table offset 192 is called with the adjusted object only.
//  14: animAddTransitionTo(state, group[140], 0.1, 3); animAddOneShot(state, group[108], 0.1, 0.1, 2056).
//  15: as 14 but group[116].
// Unknown: the names of the states and what the words at +256, +272 and +572 mean; the exact role
// of the text at 0x46e0c0 (it begins with the characters "poisongland"). Nothing here ran on the EE.
extern "C" {
int setStateProcessing__8CreaturePi(void* self, int* state);
unsigned eRand__10GameObject(void* self);
void resetRouting__8Creature(void* self);
void getSmartRandomDestination__8Creaturei(void* result, void* self, int range);
void animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(void* state, void* header, float rate, int flags);
void animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(void* state, void* header, float first, float second, int flags);
void loseBodyPart__8Scorpioni(void* self, int part);
void dropTreasure__8CreatureG6Point3iffffffPc(void* self, const void* position, int a, float b, float c, float d, float e, float f, float g, char* text);

void setState__8Scorpioni(void* self, int requested) {
    unsigned char* const o = static_cast<unsigned char*>(self);
    int state = requested;
    if (setStateProcessing__8CreaturePi(self, &state) != 0) return;
    void* const animation = o + 288;
    auto header = [&](unsigned offset) {
        return w10l::from_word(w10l::word(w10l::from_word(w10l::word(o, 36)), offset));
    };
    switch (static_cast<unsigned>(state)) {
    case 0: {
        const unsigned roll = eRand__10GameObject(self) % 60u;
        w10l::word(o, 256) = w10l::word(o, 808) + roll + 120u;
        animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(animation, header(132), 0.3f, 3);
        break;
    }
    case 1: {
        resetRouting__8Creature(self);
        Point3 destination;
        getSmartRandomDestination__8Creaturei(&destination, self, 200);
        *reinterpret_cast<Point3*>(o + 272) = destination;
        animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(animation, header(148), 0.3f, 2051);
        break;
    }
    case 3:
        resetRouting__8Creature(self);
        animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(animation, header(156), 0.3f, 2051);
        break;
    case 5:
        animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(animation, header(140), 0.1f, 3);
        break;
    case 6: {
        animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(animation, header(140), 0.1f, 3);
        if (eRand__10GameObject(self) % 6u != 0u) {
            const unsigned pick = eRand__10GameObject(self) % 3u;
            animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(
                animation, w10l::from_word(w10l::word(w10l::from_word(w10l::word(o, 36)) + pick * 8, 44)), 0.1f, 0.1f, 8);
        } else {
            animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(animation, header(68), 0.1f, 0.1f, 8);
            state = 13;
        }
        w10l::word(o, 256) = eRand__10GameObject(self) % 20u;
        break;
    }
    case 8: {
        const int current = w10l::half(o, 572);
        if (current == 9 || (current == 6 && (static_cast<unsigned short>(w10l::half(o, 294)) & 8u) != 0u)) state = current;
        else animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(animation, header(124), 0.1f, 0.1f, 16);
        break;
    }
    case 9:
        animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(animation, header(132), 0.3f, 3);
        break;
    case 10: {
        if (eRand__10GameObject(self) % 3u == 0u) loseBodyPart__8Scorpioni(self, static_cast<int>(eRand__10GameObject(self) % 6u));
        Point3 position = *reinterpret_cast<Point3*>(o + 16);
        dropTreasure__8CreatureG6Point3iffffffPc(self, &position, w10l::half(o, 56), 36.0f, 2.0f, 1.6f, 0.6f, 1.4f, 0.25f,
                                                 reinterpret_cast<char*>(static_cast<__UINTPTR_TYPE__>(0x46e0c0)));
        const unsigned side = eRand__10GameObject(self) & 1u;
        animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(
            animation, w10l::from_word(w10l::word(w10l::from_word(w10l::word(o, 36)) + side * 8, 92)), 0.3f, 2054);
        w10l::call_entry(o, 192);
        break;
    }
    case 14:
        animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(animation, header(140), 0.1f, 3);
        animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(animation, header(108), 0.1f, 0.1f, 2056);
        break;
    case 15:
        animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(animation, header(140), 0.1f, 3);
        animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(animation, header(116), 0.1f, 0.1f, 2056);
        break;
    default:
        break;
    }
    w10l::half(o, 572) = static_cast<short>(state);
}
}
