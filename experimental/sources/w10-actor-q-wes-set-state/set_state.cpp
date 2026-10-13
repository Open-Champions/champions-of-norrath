#include "../w10-actor-q-common/q.hpp"
// Observed setState__14WoodElfSoldieri at 0x00248690, 2472 bytes.
// The requested state goes to a stack word whose address is passed to
// setStateProcessing__8CreaturePi(object, &state); a nonzero result returns at once. Otherwise bit 0x200
// of the 64-bit word at +160 is set for state 13 and cleared for every other state, and the state
// (unsigned, 16 and above only store) selects through the jump table at 0x004745f0. Below, A is the
// animation state at +288, m is the pointer stored at +36, R(n) is the animation header word at m+n, V is the
// word at +964 (re-read at each test, values 0 to 3), T(n, r, f) is
// animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(A, R(n), r, f) and S(n, r, f) is
// animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(A, R(n), r, r, f). Rates are 0.3 (0x3e999999)
// where "slow" is written and 0.1 (0x3dcccccc) where "fast" is written. W is eRand__10GameObject, U
// the word at +808, and Y the float at +176.
//   0: +256 = U + W%120 + 120; when V is 1 and bNextToEdge__14WoodElfSoldier(object) is nonzero, +256 =
//      U + W%60 (a second roll). Then by V: 0 T(148) 1 T(172) 2 T(244) 3 T(252), slow, flags 3.
//   9: the same V choice as the end of state 0, without the +256 writes.
//   1: resetRouting__8Creature; getSmartRandomDestination__8Creaturei(&p, object, 300) fills a Point3 that is
//      copied to +272; Y = 100.0. By V: 0 T(164) 1 T(212) 2 T(164) 3 (Y = 150.0 first) T(268), slow, flags 3.
//      animGlobalSync__FP14AnimationState(A); +256 = U + W%120 + 300.
//   2: resetRouting; Y = 100.0. By V: 0 T(164) 1 T(212) 2 T(164) 3 T(300), slow, flags 3; animGlobalSync.
//   7: resetRouting; Y = 150.0. By V: 0 T(156) 1 T(188) 2 T(156) 3 T(268), slow, flags 3; +256 = U + 120 is
//      written just before animGlobalSync.
//   3: resetRouting. By V (each followed by the Y write): 0 T(156) then Y = 150.0; 1 T(212) then Y = 100.0;
//      2 T(156) then Y = 150.0; 3 T(268) then Y = 150.0; slow, flags 3; animGlobalSync.
//   5: by V: 0 T(148) 1 T(172) 2 T(244) 3 T(260), fast, flags 3.
//   13: T(236), slow, flags 3; +256 = U + 60.
//   6: by V: 0 T(148) then S(140), 2 T(244) then S(220), 3 T(260) then S(284); fast, T flags 3, S flags 8;
//      +256 = W%10.
//   15: by V: 1 T(172) then S(196), 3 T(260) then S(196); fast; +256 = W%10 + 10.
//   14: V 1: T(172) then S(204), fast; +256 = W%60 + 60.
//   8: with the halfword at +572 equal to 9 the state becomes 9; with it equal to 6 and bit 0x8 of the
//      halfword at +294 set the state becomes 6; otherwise with V equal to 2, S(228) fast flags 16;
//      otherwise the state becomes the current signed halfword at +572 again.
//   10: when commanderValid__14WoodElfSoldier(object) is nonzero, leaveCommander__14WoodElfSoldier(object);
//      when bit 0x2 of the word at +12 is clear, dropItem__14WoodElfSoldieri(object, 1); T(308) slow
//      flags 6; dropTreasure__8CreatureG6Point3iffffffPc(object, copy of the Point3 at +16, the signed
//      halfword at +56, 36.0, 2.0, 1.6 (0x3fcccccc), 0.6 (0x3f199999), 1.6, 0.25, null); then the virtual
//      function at byte offset 192 of the table at +72, called on the adjusted object.
//   4, 11, 12 and 16 or more: only the store.
// Every path that does not return early stores the low 16 bits of the state at +572.
// Unknown: the state names and the meaning of V and of the header offsets.
// Nothing here ran on the EE.
extern "C" {
int setStateProcessing__8CreaturePi(void* self, int* state);
void animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(void* state, void* header, float rate, int flags);
void animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(void* state, void* header, float a, float b, int flags);
void animGlobalSync__FP14AnimationState(void* state);
int eRand__10GameObject(void* object);
void resetRouting__8Creature(void* self);
void getSmartRandomDestination__8Creaturei(float* result, void* self, int range);
int bNextToEdge__14WoodElfSoldier(void* self);
int commanderValid__14WoodElfSoldier(void* self);
void leaveCommander__14WoodElfSoldier(void* self);
void dropItem__14WoodElfSoldieri(void* self, int kind);
void dropTreasure__8CreatureG6Point3iffffffPc(void* self, const float* point, int heading, float f12, float f13, float f14,
                                              float f15, float f16, float f17, char* name);
}

namespace {
using namespace w10q;
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
// state 0 and 9 share this choice by V
__attribute__((always_inline)) inline void choose_stand(void* self) {
    if (word(self, 964) == 0u) transition(self, 148, slow, 3);
    else if (word(self, 964) == 1u) transition(self, 172, slow, 3);
    else if (word(self, 964) == 2u) transition(self, 244, slow, 3);
    else if (word(self, 964) == 3u) transition(self, 252, slow, 3);
}
}  // namespace

extern "C" void setState__14WoodElfSoldieri(void* self, int requested) {
    int state = requested;
    if (setStateProcessing__8CreaturePi(self, &state) != 0) return;
    Double& flags64 = *reinterpret_cast<Double*>(bytes(self) + 160);
    if (state == 13) flags64 = flags64 | 0x200ull;
    else flags64 = flags64 & ~0x200ull;
    switch (static_cast<unsigned>(state)) {
    case 0u: {
        word(self, 256) = word(self, 808) + roll(self) % 120u + 120u;
        if (word(self, 964) == 1u && bNextToEdge__14WoodElfSoldier(self) != 0) word(self, 256) = word(self, 808) + roll(self) % 60u;
        choose_stand(self);
        break;
    }
    case 9u:
        choose_stand(self);
        break;
    case 1u: {
        resetRouting__8Creature(self);
        float destination[3];
        getSmartRandomDestination__8Creaturei(destination, self, 300);
        float copy[3] = {destination[0], destination[1], destination[2]};
        real(self, 272) = copy[0];
        real(self, 276) = copy[1];
        real(self, 280) = copy[2];
        real(self, 176) = 100.0f;
        if (word(self, 964) == 0u) transition(self, 164, slow, 3);
        if (word(self, 964) == 1u) transition(self, 212, slow, 3);
        if (word(self, 964) == 2u) transition(self, 164, slow, 3);
        if (word(self, 964) == 3u) {
            real(self, 176) = 150.0f;
            transition(self, 268, slow, 3);
        }
        animGlobalSync__FP14AnimationState(bytes(self) + 288);
        word(self, 256) = word(self, 808) + roll(self) % 120u + 300u;
        break;
    }
    case 2u:
        resetRouting__8Creature(self);
        real(self, 176) = 100.0f;
        if (word(self, 964) == 0u) transition(self, 164, slow, 3);
        if (word(self, 964) == 1u) transition(self, 212, slow, 3);
        if (word(self, 964) == 2u) transition(self, 164, slow, 3);
        if (word(self, 964) == 3u) transition(self, 300, slow, 3);
        animGlobalSync__FP14AnimationState(bytes(self) + 288);
        break;
    case 7u:
        resetRouting__8Creature(self);
        real(self, 176) = 150.0f;
        if (word(self, 964) == 0u) transition(self, 156, slow, 3);
        if (word(self, 964) == 1u) transition(self, 188, slow, 3);
        if (word(self, 964) == 2u) transition(self, 156, slow, 3);
        if (word(self, 964) == 3u) transition(self, 268, slow, 3);
        word(self, 256) = word(self, 808) + 120u;
        animGlobalSync__FP14AnimationState(bytes(self) + 288);
        break;
    case 3u:
        resetRouting__8Creature(self);
        if (word(self, 964) == 0u) {
            transition(self, 156, slow, 3);
            real(self, 176) = 150.0f;
        }
        if (word(self, 964) == 1u) {
            transition(self, 212, slow, 3);
            real(self, 176) = 100.0f;
        }
        if (word(self, 964) == 2u) {
            transition(self, 156, slow, 3);
            real(self, 176) = 150.0f;
        }
        if (word(self, 964) == 3u) {
            transition(self, 268, slow, 3);
            real(self, 176) = 150.0f;
        }
        animGlobalSync__FP14AnimationState(bytes(self) + 288);
        break;
    case 5u:
        if (word(self, 964) == 0u) transition(self, 148, fast, 3);
        if (word(self, 964) == 2u) transition(self, 244, fast, 3);
        if (word(self, 964) == 3u) transition(self, 260, fast, 3);
        if (word(self, 964) == 1u) transition(self, 172, fast, 3);
        break;
    case 13u:
        transition(self, 236, slow, 3);
        word(self, 256) = word(self, 808) + 60u;
        break;
    case 6u:
        if (word(self, 964) == 0u) {
            transition(self, 148, fast, 3);
            one_shot(self, 140, fast, 8);
        }
        if (word(self, 964) == 2u) {
            transition(self, 244, fast, 3);
            one_shot(self, 220, fast, 8);
        }
        if (word(self, 964) == 3u) {
            transition(self, 260, fast, 3);
            one_shot(self, 284, fast, 8);
        }
        word(self, 256) = roll(self) % 10u;
        break;
    case 15u:
        if (word(self, 964) == 1u) {
            transition(self, 172, fast, 3);
            one_shot(self, 196, fast, 8);
        }
        if (word(self, 964) == 3u) {
            transition(self, 260, fast, 3);
            one_shot(self, 196, fast, 8);
        }
        word(self, 256) = roll(self) % 10u + 10u;
        break;
    case 14u:
        if (word(self, 964) == 1u) {
            transition(self, 172, fast, 3);
            one_shot(self, 204, fast, 8);
        }
        word(self, 256) = roll(self) % 60u + 60u;
        break;
    case 8u: {
        const int current = shalf(self, 572);
        if (current == 9) {
            state = 9;
        } else if (current == 6 && (half(self, 294) & 0x8u) != 0u) {
            state = 6;
        } else if (word(self, 964) == 2u) {
            one_shot(self, 228, fast, 16);
        } else {
            state = current;
        }
        break;
    }
    case 10u: {
        if (commanderValid__14WoodElfSoldier(self) != 0) leaveCommander__14WoodElfSoldier(self);
        if ((word(self, 12) & 0x2u) == 0u) dropItem__14WoodElfSoldieri(self, 1);
        transition(self, 308, slow, 6);
        float point[3] = {real(self, 16), real(self, 20), real(self, 24)};
        dropTreasure__8CreatureG6Point3iffffffPc(self, point, shalf(self, 56), 36.0f, 2.0f, __builtin_bit_cast(float, 0x3fccccccu),
                                                 __builtin_bit_cast(float, 0x3f199999u), __builtin_bit_cast(float, 0x3fccccccu), 0.25f, nullptr);
        typedef void (*SlotCall)(void*);
        unsigned char* table = link(self, 72) + 192;
        SlotCall call = reinterpret_cast<SlotCall>(from_word(word(table, 4)));
        call(bytes(self) + shalf(table, 0));
        break;
    }
    default:
        break;
    }
    half(self, 572) = static_cast<unsigned short>(state);
}
