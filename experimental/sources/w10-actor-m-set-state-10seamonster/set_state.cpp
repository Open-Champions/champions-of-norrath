#include "../w10-actor-m-common/access.hpp"
// Observed setState__10SeaMonsteri at 0x1f8770, 1008 bytes. Arguments: the object and the requested state;
// nothing is returned. The requested state is copied to a stack word whose address goes to
// setStateProcessing__8CreaturePi(object, &state); a nonzero result returns at once without storing. Otherwise
// the (possibly changed) word picks a case below (unsigned, jump table at 0x46eb50, 16 entries; 16 and above
// only store) and the low 16 bits of the word are stored at object+572 at the end. The animation state is at
// object+288 and the word at object+36 points to a table T of animation headers. R is a roll from
// eRand__10GameObject(object). "tr" is animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(anim, T[o],
// rate, flags) and "shot" is animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(anim, T[o], rate, rate, flags).
//   0: +256 = word(+808) + R%120 (unsigned) is stored before the call; tr(T60, 0.3, 3).
//   1: resetRouting(object); getSmartRandomDestination (range 200) copied to +272; +256 = word(+808) + R%120 + 300
//      stored before the call; tr(T124, 0.3, 2051); animGlobalSync(anim).
//   3: resetRouting(object); tr(T92, 0.1, 2051); animGlobalSync(anim).
//   5: tr(T68, 0.1, 3).            9: tr(T60, 0.3, 3).
//   13: tr(T68, 0.1, 3); shot(T108, 0.1, 2056).     14: tr(T68, 0.1, 3); shot(T116, 0.1, 2056).
//   6: tr(T68, 0.1, 3); shot(T[44 + 8*(R&1)], 0.5, 8) with the first roll; +256 = R2%12 with a second roll.
//   15: tr(T68, 0.1, 3); shot(T36, 0.5, 8); +256 = R%20.
//   8: when the current signed halfword at +572 is 6 and bit 3 of the halfword at +294 is set, or the current
//      halfword is 9, the state word becomes the current halfword (no change); otherwise shot(T84, 0.1, 16).
//   10: dropTreasure__8CreatureG6Point3iffffffPc(object, copy of the 12 bytes at +16, signed halfword at +56,
//      36.0, 2.0, 1.8, 0.6, 2.0, 0.25, null text); tr(T76, 0.3, 6); then table entry 192 (slot 24) runs on the
//      adjusted object.
//   2, 4, 7, 11, 12 and 16 or more: only the store.
// Unknown: the state names, the fields at +256 (a countdown or time), +272 (a destination), +294 and the units.
extern "C" {
int setStateProcessing__8CreaturePi(void* self, int* state);
int eRand__10GameObject(void* object);
void animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(void* state, void* header, float rate, int flags);
void animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(void* state, void* header, float a, float b, int flags);
void animGlobalSync__FP14AnimationState(void* state);
void resetRouting__8Creature(void* self);
void getSmartRandomDestination__8Creaturei(Point3* result, void* self, int range);
void dropTreasure__8CreatureG6Point3iffffffPc(void* self, Point3 where, int angle, float a, float b, float c, float d, float e, float f, char* text);
}

namespace {
inline void* header(unsigned char* o, unsigned offset) {
    return w10m::from_word(w10m::word(w10m::from_word(w10m::word(o, 36)), offset));
}
inline void transition(unsigned char* o, unsigned offset, unsigned rate_bits, int flags) {
    animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(o + 288, header(o, offset), w10m::bits(rate_bits), flags);
}
inline void one_shot(unsigned char* o, unsigned offset, unsigned rate_bits, int flags) {
    const float rate = w10m::bits(rate_bits);
    animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(o + 288, header(o, offset), rate, rate, flags);
}
const unsigned kThreeTenths = 0x3e999999u;
const unsigned kOneTenth = 0x3dccccccu;
}  // namespace

extern "C" void setState__10SeaMonsteri(void* self, int requested) {
    unsigned char* o = static_cast<unsigned char*>(self);
    int state = requested;
    if (setStateProcessing__8CreaturePi(self, &state) != 0) return;
    switch (static_cast<unsigned>(state)) {
    case 0u: {
        const unsigned roll = static_cast<unsigned>(eRand__10GameObject(self)) % 120u;
        w10m::word(o, 256) = w10m::word(o, 808) + roll;
        transition(o, 60, kThreeTenths, 3);
        break;
    }
    case 1u: {
        resetRouting__8Creature(self);
        Point3 destination;
        getSmartRandomDestination__8Creaturei(&destination, self, 200);
        *reinterpret_cast<Point3*>(o + 272) = destination;
        const unsigned roll = static_cast<unsigned>(eRand__10GameObject(self)) % 120u;
        w10m::word(o, 256) = w10m::word(o, 808) + roll + 300u;
        transition(o, 124, kThreeTenths, 2051);
        animGlobalSync__FP14AnimationState(o + 288);
        break;
    }
    case 3u:
        resetRouting__8Creature(self);
        transition(o, 92, kOneTenth, 2051);
        animGlobalSync__FP14AnimationState(o + 288);
        break;
    case 5u:
        transition(o, 68, kOneTenth, 3);
        break;
    case 6u: {
        transition(o, 68, kOneTenth, 3);
        const unsigned pick = static_cast<unsigned>(eRand__10GameObject(self)) & 1u;
        one_shot(o, 44u + (pick << 3), 0x3f000000u, 8);
        w10m::word(o, 256) = static_cast<unsigned>(eRand__10GameObject(self)) % 12u;
        break;
    }
    case 8u: {
        const int current = w10m::half(o, 572);
        if ((current == 6 && (static_cast<unsigned short>(w10m::half(o, 294)) & 8u) != 0) || current == 9) {
            state = current;
        } else {
            one_shot(o, 84, kOneTenth, 16);
        }
        break;
    }
    case 9u:
        transition(o, 60, kThreeTenths, 3);
        break;
    case 10u: {
        dropTreasure__8CreatureG6Point3iffffffPc(self, *reinterpret_cast<Point3*>(o + 16), w10m::half(o, 56), 36.0f, 2.0f, w10m::bits(0x3fe66666u),
                                                 w10m::bits(0x3f199999u), 2.0f, 0.25f, nullptr);
        transition(o, 76, kThreeTenths, 6);
        w10m::call_entry(o, 192);
        break;
    }
    case 13u:
        transition(o, 68, kOneTenth, 3);
        one_shot(o, 108, kOneTenth, 2056);
        break;
    case 14u:
        transition(o, 68, kOneTenth, 3);
        one_shot(o, 116, kOneTenth, 2056);
        break;
    case 15u: {
        transition(o, 68, kOneTenth, 3);
        one_shot(o, 36, 0x3f000000u, 8);
        w10m::word(o, 256) = static_cast<unsigned>(eRand__10GameObject(self)) % 20u;
        break;
    }
    default:
        break;
    }
    w10m::half(o, 572) = static_cast<short>(state);
}
