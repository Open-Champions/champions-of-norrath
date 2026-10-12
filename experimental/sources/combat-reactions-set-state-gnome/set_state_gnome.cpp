#include "../combat-reactions-bytes/bytes.hpp"
// Observed setState__14GnomeNavigatori at 0x2021b0, 564 bytes.
// The argument is kept in a stack word. setStateProcessing is called with the
// object and the address of that word; a nonzero result returns without
// storing anything. Otherwise the word, which the call may have changed,
// selects a path through a jump table of eleven words at 0x46f470; words at or
// above 11 take the store path. Every path ends by storing the low 16 bits of
// the word at object+572.
// The word at object+36 is a table pointer; the animation state is at
// object+288. Every transition rate is the single 0x3dcccccc (0.1).
// 0: a transition to the header at table+28 is added, flags 3; eRand modulo
//    120 plus 60 plus the word at +808 is stored at +256.
// 1: resetRouting runs; getSmartRandomDestination returns a 12-byte point for
//    the object and range 200, stored at object+272. When the 64-bit value at
//    object+160 has any of bits 0x400000 and 0x800000, a transition to the
//    header at table+52 is added, otherwise to table+84, both with flags 2051
//    (0x803). eRand modulo 120 plus 300 plus the word at +808 is stored at
//    +256 and animGlobalSync runs on the animation state.
// 3: resetRouting runs, a transition to the header at table+52 is added with
//    flags 2051 and animGlobalSync runs.
// 5: a transition to table+28 with flags 6.
// 9: a transition to table+28 with flags 3.
// 10: a transition to table+28 with flags 6, then virtual slot byte 192 is
//    called with this adjusted by that slot's signed halfword and no other
//    argument; the vtable names that slot death__8Creature in the SmallSpider
//    table, and the GnomeNavigator table is not read here.
// 6 and 8: the word becomes the signed halfword already at object+572, so the
//    state does not change. 2, 4, 7 and values from 11 up: only the store.
// Inferred: norrath_actor_random is eRand and norrath_animation_add_transition
// is animAddTransitionTo. The point result of getSmartRandomDestination comes
// back through a hidden first pointer. Unknown: what the states mean, what the
// bits at +160 mean, and what the fields at +256 and +272 are.
extern "C" int setStateProcessing__8CreaturePi(void* self, int* state);
extern "C" unsigned norrath_actor_random(void* object);
extern "C" void norrath_animation_add_transition(void* animation_state, void* animation_header, float rate, int flags);
extern "C" void animGlobalSync__FP14AnimationState(void* animation_state);
extern "C" void resetRouting__8Creature(void* self);
extern "C" void getSmartRandomDestination__8Creaturei(void* result, void* self, int range);

extern "C" void setState__14GnomeNavigatori(void* self, int state) {
    using namespace combat_reactions;
    const Address base = address(self);
    unsigned char* animation = static_cast<unsigned char*>(self) + 288u;
    const float rate = single_from_bits(0x3dccccccu);
    int word = state;
    if (setStateProcessing__8CreaturePi(self, &word) != 0) return;
    switch (static_cast<unsigned>(word)) {
    case 0u: {
        const Address table = address(pointer(read_word(base + 36u)));
        norrath_animation_add_transition(animation, pointer(read_word(table + 28u)), rate, 3);
        const unsigned wait = norrath_actor_random(self) % 120u;
        write_word(base + 256u, read_word(base + 808u) + wait + 60u);
        break;
    }
    case 1u: {
        resetRouting__8Creature(self);
        unsigned char point[12];
        getSmartRandomDestination__8Creaturei(point, self, 200);
        for (unsigned i = 0u; i < 12u; ++i) bytes(base + 272u)[i] = point[i];
        const Address table = address(pointer(read_word(base + 36u)));
        const unsigned offset = (read_word(base + 160u) & 0xc00000u) != 0u ? 52u : 84u;
        norrath_animation_add_transition(animation, pointer(read_word(table + offset)), rate, 2051);
        const unsigned wait = norrath_actor_random(self) % 120u;
        write_word(base + 256u, read_word(base + 808u) + wait + 300u);
        animGlobalSync__FP14AnimationState(animation);
        break;
    }
    case 3u: {
        resetRouting__8Creature(self);
        const Address table = address(pointer(read_word(base + 36u)));
        norrath_animation_add_transition(animation, pointer(read_word(table + 52u)), rate, 2051);
        animGlobalSync__FP14AnimationState(animation);
        break;
    }
    case 5u: {
        const Address table = address(pointer(read_word(base + 36u)));
        norrath_animation_add_transition(animation, pointer(read_word(table + 28u)), rate, 6);
        break;
    }
    case 9u: {
        const Address table = address(pointer(read_word(base + 36u)));
        norrath_animation_add_transition(animation, pointer(read_word(table + 28u)), rate, 3);
        break;
    }
    case 10u: {
        const Address table = address(pointer(read_word(base + 36u)));
        norrath_animation_add_transition(animation, pointer(read_word(table + 28u)), rate, 6);
        void* adjusted = nullptr;
        using SlotCall = void (*)(void*);
        SlotCall call = slot_function<SlotCall>(self, 192u, &adjusted);
        call(adjusted);
        break;
    }
    case 6u:
    case 8u:
        word = read_i16(base + 572u);
        break;
    default:
        break;
    }
    write_u16(base + 572u, static_cast<unsigned>(word));
}
