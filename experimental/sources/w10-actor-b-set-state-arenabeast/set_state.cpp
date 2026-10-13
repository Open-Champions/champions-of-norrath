#include "../w10-actor-b-common/fields.hpp"
// Observed setState__10Arenabeasti at 0x2364c8, 1156 bytes. The state is passed to
// setStateProcessing(object, &state) first; when that returns nonzero nothing else happens.
// Otherwise a switch on the (possibly changed) state, an unsigned compare against 24 gating
// a table at 0x4727a0, runs one of the blocks below and the halfword at +572 is then set to
// the state. "Headers" are the animation-header words of the record whose address is the word at
// +36; "anim" is the AnimationState at object+288; all one-shots pass the same single twice.
//  0:  +256 = word at +808 + eRand % 120 (unsigned); transition to header +140 (rate 0.3,
//      flags 3).
//  1:  resetRouting; getSmartRandomDestination(200) copied to the point at +272; +256 = word
//      at +808 + eRand % 120 + 300; transition to header +164 (rate 0.3, flags 2051);
//      animGlobalSync(anim).
//  3:  resetRouting; unless the current state (signed halfword at +572) is 3, 6, 18, 14 or 15,
//      a transition to header +164 (rate 0.5, flags 2051); then animGlobalSync(anim).
//  5, 9: transition to header +140 (rate 0.1, flags 3).
//  6, 14, 15, 18: one-shot (0.5, flags 8) of header +60, +68, +44, +52 respectively; then
//      +960 = state and +256 = eRand % 20.
//  13, 16, 17: one-shot (0.5, flags 8) of header +92, +100, +84; then as above.
//  8:  the state is replaced by the current state (signed halfword at +572).
//  10: transition to header +108 (rate 0.3, flags 6), then the virtual entry at byte offset
//      192 of the table at +72 is called with the object adjusted by its halfword.
//  19: one-shot (0.1, flags 16) of header +148.
//  20: one-shot (0.1, flags 16) of header +156; +964 = word at +808 + eRand % 300 + 300.
//  21, 22, 23: one-shot (0.5, flags 2056) of header +76, +28, +36; then +960 = state,
//      +968 = 0, +972 = 0, +256 = eRand % 20.
//  2, 4, 7, 11, 12 and states of 24 or more: only the halfword store.
// Inferred: the 0.3/0.1 singles are blend rates (the bit patterns are 0x3e999999 and
// 0x3dcccccc, one ulp under the nearest values). Unknown: the names of the states, of +256 /
// +960 / +964 / +968 / +972 and the meaning of slot 192.
extern "C" {
int setStateProcessing__8CreaturePi(void* self, int* state);
unsigned eRand__10GameObject(void* self);
void animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(void* state, void* header, float rate, int flags);
void animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(void* state, void* header, float a, float b, int flags);
void animGlobalSync__FP14AnimationState(void* state);
void resetRouting__8Creature(void* self);
void getSmartRandomDestination__8Creaturei(float* result, void* self, int range);
}

namespace {
using namespace actor_b;

__attribute__((always_inline)) inline void* header(void* self, unsigned offset) {
    return link(link(self, 36), offset);
}
__attribute__((always_inline)) inline void* anim(void* self) { return static_cast<unsigned char*>(self) + 288; }
__attribute__((always_inline)) inline void one_shot(void* self, unsigned offset, float rate, int flags) {
    animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(anim(self), header(self, offset), rate, rate, flags);
}
__attribute__((always_inline)) inline void transition(void* self, unsigned offset, float rate, int flags) {
    animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(anim(self), header(self, offset), rate, flags);
}
}

extern "C" void setState__10Arenabeasti(void* self, int state) {
    using namespace rtti_c;
    if (setStateProcessing__8CreaturePi(self, &state) != 0) return;
    switch (state) {
    case 0:
        field<Word>(self, 256) = field<Word>(self, 808) + eRand__10GameObject(self) % 120u;
        transition(self, 140, single_from_bits(0x3e999999u), 3);
        break;
    case 1: {
        resetRouting__8Creature(self);
        float destination[3] __attribute__((may_alias));
        getSmartRandomDestination__8Creaturei(destination, self, 200);
        const Word* bits = reinterpret_cast<const Word*>(destination);
        field<Word>(self, 272) = bits[0];
        field<Word>(self, 276) = bits[1];
        field<Word>(self, 280) = bits[2];
        field<Word>(self, 256) = field<Word>(self, 808) + eRand__10GameObject(self) % 120u + 300u;
        transition(self, 164, single_from_bits(0x3e999999u), 2051);
        animGlobalSync__FP14AnimationState(anim(self));
        break;
    }
    case 3: {
        resetRouting__8Creature(self);
        const int current = field<SHalf>(self, 572);
        if (current != 3 && current != 6 && current != 18 && current != 14 && current != 15) transition(self, 164, 0.5f, 2051);
        animGlobalSync__FP14AnimationState(anim(self));
        break;
    }
    case 5:
    case 9:
        transition(self, 140, single_from_bits(0x3dccccccu), 3);
        break;
    case 6:
    case 14:
    case 15:
    case 18:
        one_shot(self, state == 15 ? 44 : state == 14 ? 68 : state == 18 ? 52 : 60, 0.5f, 8);
        field<Word>(self, 960) = static_cast<unsigned>(state);
        field<Word>(self, 256) = eRand__10GameObject(self) % 20u;
        break;
    case 13:
    case 16:
    case 17:
        one_shot(self, state == 16 ? 100 : state == 17 ? 84 : 92, 0.5f, 8);
        field<Word>(self, 960) = static_cast<unsigned>(state);
        field<Word>(self, 256) = eRand__10GameObject(self) % 20u;
        break;
    case 8:
        state = field<SHalf>(self, 572);
        break;
    case 10: {
        transition(self, 108, single_from_bits(0x3e999999u), 6);
        void* adjusted;
        slot_function<void (*)(void*)>(self, 192, &adjusted)(adjusted);
        break;
    }
    case 19:
        one_shot(self, 148, single_from_bits(0x3dccccccu), 16);
        break;
    case 20:
        one_shot(self, 156, single_from_bits(0x3dccccccu), 16);
        field<Word>(self, 964) = field<Word>(self, 808) + eRand__10GameObject(self) % 300u + 300u;
        break;
    case 21:
    case 22:
    case 23:
        one_shot(self, state == 22 ? 28 : state == 23 ? 36 : 76, 0.5f, 2056);
        field<Word>(self, 960) = static_cast<unsigned>(state);
        field<Word>(self, 968) = 0;
        field<Word>(self, 972) = 0;
        field<Word>(self, 256) = eRand__10GameObject(self) % 20u;
        break;
    default:
        break;
    }
    field<Half>(self, 572) = static_cast<unsigned short>(state);
}
