#include "../combat-reactions-bytes/bytes.hpp"
// Observed setState__7Mermaidi at 0x1f7f08, 344 bytes.
// A jump table of eleven words at 0x46e9c0 selects the entry for arguments 0
// through 10; any other argument only stores the state. Observed table:
// argument 0 and 1 have their own entries, 5 and 8 share one, 10 has one, and
// 2, 3, 4, 6, 7 and 9 go straight to the store. Every path ends by storing the
// low 16 bits of the argument at object+572.
// The word at object+36 is a table pointer whose words at +84 and +92 are
// animation headers. The animation state is at object+288. The transition
// rate is the single 0x3e999999 (three .lit4 words hold the same pattern).
// Argument 0: eRand on the object, taken modulo 120 unsigned, is added to the
//   word at object+808 and stored at object+256; a transition to the header at
//   +92 is added with rate 0.3 and flags 3.
// Argument 1: resetRouting runs with the object; getSmartRandomDestination
//   returns a 12-byte point for the object and range 200, copied to object+272.
//   Then eRand modulo 120 plus the word at +808 plus 300 is stored at object+256
//   and a transition to the header at +84 is added with rate 0.3 and flags
//   2051 (0x803).
// Arguments 5 and 8: a one-shot of the header at +92 is added with both
//   singles 0x3dcccccc (0.1) and flags 16.
// Argument 10: a transition to the header at +92 is added with rate 0.3, flags 6.
// Inferred: norrath_actor_random is eRand (the +68 generator) and
// norrath_animation_add_transition is animAddTransitionTo. The point result of
// getSmartRandomDestination comes back through a hidden first pointer, with the
// object and range in the next two registers.
// Unknown: what the states mean and what the words at +256, +272 and +808 are.
extern "C" unsigned norrath_actor_random(void* object);
extern "C" void norrath_animation_add_transition(void* animation_state, void* animation_header, float rate, int flags);
extern "C" void resetRouting__8Creature(void* self);
extern "C" void getSmartRandomDestination__8Creaturei(void* result, void* self, int range);
extern "C" void animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(void* animation_state, void* animation_header,
                                                                     float first, float second, int flags);

extern "C" void setState__7Mermaidi(void* self, int state) {
    using namespace combat_reactions;
    const Address base = address(self);
    unsigned char* animation = static_cast<unsigned char*>(self) + 288u;
    switch (static_cast<unsigned>(state)) {
    case 0u: {
        const unsigned wait = norrath_actor_random(self) % 120u;
        const Address table = address(pointer(read_word(base + 36u)));
        write_word(base + 256u, read_word(base + 808u) + wait);
        norrath_animation_add_transition(animation, pointer(read_word(table + 92u)), single_from_bits(0x3e999999u), 3);
        break;
    }
    case 1u: {
        resetRouting__8Creature(self);
        unsigned char point[12];
        getSmartRandomDestination__8Creaturei(point, self, 200);
        for (unsigned i = 0u; i < 12u; ++i) bytes(base + 272u)[i] = point[i];
        const unsigned wait = norrath_actor_random(self) % 120u;
        const Address table = address(pointer(read_word(base + 36u)));
        write_word(base + 256u, read_word(base + 808u) + wait + 300u);
        norrath_animation_add_transition(animation, pointer(read_word(table + 84u)), single_from_bits(0x3e999999u), 2051);
        break;
    }
    case 5u:
    case 8u: {
        const Address table = address(pointer(read_word(base + 36u)));
        animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(animation, pointer(read_word(table + 92u)),
                                                              single_from_bits(0x3dccccccu), single_from_bits(0x3dccccccu), 16);
        break;
    }
    case 10u: {
        const Address table = address(pointer(read_word(base + 36u)));
        norrath_animation_add_transition(animation, pointer(read_word(table + 92u)), single_from_bits(0x3e999999u), 6);
        break;
    }
    default:
        break;
    }
    write_u16(base + 572u, static_cast<unsigned>(state));
}
