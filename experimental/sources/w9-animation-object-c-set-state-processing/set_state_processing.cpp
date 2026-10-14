#include "../w9-animation-object-c-common/mem.hpp"
// Observed setStateProcessing__8CreaturePi at 0x001129f0, 108 bytes. Takes the creature and
// a pointer to a state word and returns 0 or 1.
//   - If the signed byte at creature+570 is nonzero and *state is 1: returns 1, nothing
//     is stored.
//   - Otherwise, if *state is 3: calls bExitStatePersueIdle__8Creature(creature); when it
//     returns zero, stores 5 into *state. Returns 0 in either case.
//   - Any other state: returns 0, nothing stored.
// Inferred: callers use a result of 1 as "the state change was consumed here"; the
// numbers 1, 3 and 5 are creature state values and +570 is some creature flag byte.
// Unknown: the state names and what the byte at +570 means.
extern "C" int bExitStatePersueIdle__8Creature(unsigned char* self);

extern "C" int setStateProcessing__8CreaturePi(unsigned char* self, int* state) {
    using namespace norrath_aoc;
    if (load_signed_byte(self + 570) != 0 && *state == 1) return 1;
    if (*state == 3 && bExitStatePersueIdle__8Creature(self) == 0) *state = 5;
    return 0;
}
