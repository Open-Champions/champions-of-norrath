#include "../combat-reactions-bytes/bytes.hpp"
// Observed setState__7Nataslai at 0x233800, 116 bytes.
// A jump table of eleven words at 0x472350 selects the entry for arguments
// 0 through 10; any other argument takes the plain path.
// Observed table: argument 0 adds a transition, arguments 2, 4 and 7 are plain,
// and arguments 1, 3, 5, 6, 8, 9 and 10 reload the halfword at object+572 and
// store it back, so those arguments leave the state unchanged.
// Argument 0: the word at object+36 is a table pointer; the word at its +28 is
// an animation header. A transition to it is added to the animation state at
// object+288 with rate 0.3 (bit pattern 0x3e999999, in .lit4) and flags 3.
// The low 16 bits of the argument are then stored at object+572.
// Plain path: the low 16 bits of the argument are stored at object+572.
// Inferred: norrath_animation_add_transition is animAddTransitionTo.
// Unknown: what the states mean.
extern "C" void norrath_animation_add_transition(void* animation_state, void* animation_header, float rate, int flags);

extern "C" void setState__7Nataslai(void* self, int state) {
    using namespace combat_reactions;
    const Address base = address(self);
    const unsigned selector = static_cast<unsigned>(state);
    if (selector < 11u) {
        if (selector == 0u) {
            const Address table = address(pointer(read_word(base + 36u)));
            norrath_animation_add_transition(static_cast<unsigned char*>(self) + 288u,
                                             pointer(read_word(table + 28u)),
                                             single_from_bits(0x3e999999u), 3);
        } else if (selector != 2u && selector != 4u && selector != 7u) {
            return;
        }
    }
    write_u16(base + 572u, static_cast<unsigned>(state));
}
