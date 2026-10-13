#include "../w11-actor-c-common/c.hpp"
// Observed setState__6Wraithi (first written in w10-actor-q, checked here) at 0x00204c50, 928 bytes.
// The requested state goes to a stack word whose address is passed to
// setStateProcessing__8CreaturePi(object, &state); a nonzero result returns at once. Otherwise the state
// (unsigned, 16 and above only store) selects through the jump table at 0x0046f880. Below, A is the
// animation state at +288, m is the pointer stored at +36, R(n) is the header word at m+n, T(n, r, f) is
// animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(A, R(n), r, f), S(n, r, f) is
// animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(A, R(n), r, r, f), W is eRand__10GameObject,
// U the word at +808, and C the halfword at +572 (the current state). Rates are 0.3 (0x3e999999) for
// "slow" and 0.1 (0x3dcccccc) for "fast".
//   0: listDestroy__FP8ListHead on the head at +576 and clear that word; +256 = U + W%120 (unsigned);
//      T(76, slow, 3).
//   1: resetRouting__8Creature; getSmartRandomDestination__8Creaturei(&p, object, 300) fills a Point3 that is
//      copied to +272; the float at +176 = 140.0 (0x430c0000); T(60, slow, 3); animGlobalSync.
//   3: resetRouting; +176 = 140.0; unless C is 3, 6 or 13: T(60, slow, 3) and animGlobalSync.
//   5: T(84, fast, 3).        6: S(28, fast, 8).        15: S(44, fast, 8).
//   13: S(44, fast, 8); +964 = U + W%120 + 300.
//   14: T(84, fast, 3); +256 = U + W%60 + 120.
//   9: T(76, slow, 3).
//   8: with C equal to 9, or with C equal to 6 or 13 and bit 0x8 of the halfword at +294 set, the state
//      becomes C; otherwise S(68, fast, 16).
//   10: item::dropTreasure (dropTreasure__4itemG6Point3iffffffPci) with a copy of the Point3 at +16, the
//      signed halfword at +56, 36.0, 2.0, 2.8 (0x40333333), 0.0, 3.0, 0.25, no name and 0; listDestroy on
//      the head at +576 and clear it; T(36, slow, 6); the word at +12 |= 0x4000; then the virtual function
//      at byte offset 192 of the table at +72, called on the adjusted object.
//   2, 4, 7, 11, 12 and 16 or more: only the store.
// Every path that does not return early stores the low 16 bits of the state at +572.
// Unknown: the state names and the meaning of +964. Nothing here ran on the EE.
extern "C" {
int setStateProcessing__8CreaturePi(void* self, int* state);
void animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(void* state, void* header, float rate, int flags);
void animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(void* state, void* header, float a, float b, int flags);
void animGlobalSync__FP14AnimationState(void* state);
int eRand__10GameObject(void* object);
void resetRouting__8Creature(void* self);
void getSmartRandomDestination__8Creaturei(float* result, void* self, int range);
void listDestroy__FP8ListHead(void* head);
void dropTreasure__4itemG6Point3iffffffPci(const float* point, int heading, float f12, float f13, float f14, float f15,
                                           float f16, float f17, char* name, int value);
}

namespace {
using namespace w11c;
const float slow = __builtin_bit_cast(float, 0x3e999999u);
const float fast = __builtin_bit_cast(float, 0x3dccccccu);
__attribute__((always_inline)) inline void* header(void* self, unsigned offset) { return link(link(self, 36), offset); }
__attribute__((always_inline)) inline void transition(void* self, unsigned offset, float rate, int flags) {
    animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(bytes(self) + 288, header(self, offset), rate, flags);
}
__attribute__((always_inline)) inline void one_shot(void* self, unsigned offset, float rate, int flags) {
    animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(bytes(self) + 288, header(self, offset), rate, rate, flags);
}
__attribute__((always_inline)) inline unsigned roll(void* self) { return static_cast<unsigned>(eRand__10GameObject(self)); }
__attribute__((always_inline)) inline void clear_list(void* self) {
    listDestroy__FP8ListHead(bytes(self) + 576);
    word(self, 576) = 0u;
}
}  // namespace

extern "C" void setState__6Wraithi(void* self, int requested) {
    int state = requested;
    if (setStateProcessing__8CreaturePi(self, &state) != 0) return;
    switch (static_cast<unsigned>(state)) {
    case 0u:
        clear_list(self);
        word(self, 256) = word(self, 808) + roll(self) % 120u;
        transition(self, 76, slow, 3);
        break;
    case 1u: {
        resetRouting__8Creature(self);
        float destination[3];
        getSmartRandomDestination__8Creaturei(destination, self, 300);
        float copy[3] = {destination[0], destination[1], destination[2]};
        real(self, 272) = copy[0];
        real(self, 276) = copy[1];
        real(self, 280) = copy[2];
        real(self, 176) = 140.0f;
        transition(self, 60, slow, 3);
        animGlobalSync__FP14AnimationState(bytes(self) + 288);
        break;
    }
    case 3u: {
        resetRouting__8Creature(self);
        real(self, 176) = 140.0f;
        const int current = shalf(self, 572);
        if (current == 3 || current == 6 || current == 13) break;
        transition(self, 60, slow, 3);
        animGlobalSync__FP14AnimationState(bytes(self) + 288);
        break;
    }
    case 5u:
        transition(self, 84, fast, 3);
        break;
    case 6u:
        one_shot(self, 28, fast, 8);
        break;
    case 15u:
        one_shot(self, 44, fast, 8);
        break;
    case 13u:
        one_shot(self, 44, fast, 8);
        word(self, 964) = word(self, 808) + roll(self) % 120u + 300u;
        break;
    case 14u:
        transition(self, 84, fast, 3);
        word(self, 256) = word(self, 808) + roll(self) % 60u + 120u;
        break;
    case 9u:
        transition(self, 76, slow, 3);
        break;
    case 8u: {
        const int current = shalf(self, 572);
        if (current == 9 || ((current == 6 || current == 13) && (half(self, 294) & 0x8u) != 0u)) state = current;
        else one_shot(self, 68, fast, 16);
        break;
    }
    case 10u: {
        float point[3] = {real(self, 16), real(self, 20), real(self, 24)};
        dropTreasure__4itemG6Point3iffffffPci(point, shalf(self, 56), 36.0f, 2.0f, __builtin_bit_cast(float, 0x40333333u), 0.0f, 3.0f, 0.25f, nullptr, 0);
        clear_list(self);
        transition(self, 36, slow, 6);
        word(self, 12) = word(self, 12) | 0x4000u;
        call_slot(self, 192);
        break;
    }
    default:
        break;
    }
    half(self, 572) = static_cast<unsigned short>(state);
}
