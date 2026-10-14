#include "../combat-reactions-bytes/bytes.hpp"
// Observed setState__3Cati at 0x1ee8a0, 460 bytes.
// A jump table of fourteen words at 0x46da30 selects the entry for arguments 0
// through 13; any other argument only stores the state. Every path ends by
// storing the low 16 bits of the (possibly replaced) state at object+572.
// The word at object+36 is a table pointer; the animation state is at
// object+288. Observed table: 0, 1, 3 and 13 have entries, 6 replaces the
// state with 3, 8 keeps the halfword already at object+572, and 2, 4, 5, 7, 9,
// 10, 11 and 12 only store.
// 0: eRand modulo 120 plus 60 plus the word at +808 is stored at +968; a second
//    eRand modulo 60 plus 120 plus the word at +808 is stored at +972; a
//    transition to the header at table+116 is added with rate 0.5 and flags 3.
// 13: the same two words with moduli 1800 and 480 and offsets 1200 and 240; a
//    transition to the header at table+132 is added with rate 0.3
//    (0x3e999999) and flags 3.
// 1 and 3: resetRouting runs; getSmartRandomDestination is called for a 12-byte
//    point, range 300, and the point is never read. The single at +176 is set
//    to 20.0 (state 1) or 40.0 (state 3). eRand modulo 300 plus 180 plus the
//    word at +808 is stored at +968. When the single at +336 is below zero it
//    becomes 0.0. The single at +340 is set to 0x3eaaaaab (one third).
// Inferred: norrath_actor_random is eRand and norrath_animation_add_transition
// is animAddTransitionTo. The point result of getSmartRandomDestination comes
// back through a hidden first pointer. Unknown: the meaning of the states and
// of the fields at +176, +336, +340, +968 and +972.
extern "C" unsigned norrath_actor_random(void* object);
extern "C" void norrath_animation_add_transition(void* animation_state, void* animation_header, float rate, int flags);
extern "C" void resetRouting__8Creature(void* self);
extern "C" void getSmartRandomDestination__8Creaturei(void* result, void* self, int range);

extern "C" void setState__3Cati(void* self, int state) {
    using namespace combat_reactions;
    const Address base = address(self);
    unsigned char* animation = static_cast<unsigned char*>(self) + 288u;
    switch (static_cast<unsigned>(state)) {
    case 0u: {
        const unsigned first = norrath_actor_random(self) % 120u;
        write_word(base + 968u, read_word(base + 808u) + first + 60u);
        const unsigned second = norrath_actor_random(self) % 60u;
        const Address table = address(pointer(read_word(base + 36u)));
        write_word(base + 972u, read_word(base + 808u) + second + 120u);
        norrath_animation_add_transition(animation, pointer(read_word(table + 116u)), single_from_bits(0x3f000000u), 3);
        break;
    }
    case 13u: {
        const unsigned first = norrath_actor_random(self) % 1800u;
        write_word(base + 968u, read_word(base + 808u) + first + 1200u);
        const unsigned second = norrath_actor_random(self) % 480u;
        const Address table = address(pointer(read_word(base + 36u)));
        write_word(base + 972u, read_word(base + 808u) + second + 240u);
        norrath_animation_add_transition(animation, pointer(read_word(table + 132u)), single_from_bits(0x3e999999u), 3);
        break;
    }
    case 1u:
    case 3u: {
        resetRouting__8Creature(self);
        unsigned char point[12];
        getSmartRandomDestination__8Creaturei(point, self, 300);
        const unsigned wait = norrath_actor_random(self) % 300u;
        write_single(base + 176u, state == 1 ? single_from_bits(0x41a00000u) : single_from_bits(0x42200000u));
        write_word(base + 968u, read_word(base + 808u) + wait + 180u);
        if (read_single(base + 336u) < 0.0f) write_single(base + 336u, 0.0f);
        write_single(base + 340u, single_from_bits(0x3eaaaaabu));
        break;
    }
    case 6u:
        write_u16(base + 572u, 3u);
        return;
    case 8u:
        return;
    default:
        break;
    }
    write_u16(base + 572u, static_cast<unsigned>(state));
}
