#include "../w9-animation-object-a-common/mem.hpp"
// Observed bExitStatePersueIdle__8Creature at 0x00112e08, 12 bytes. Returns 1 when the
// signed halfword at creature+794 is zero and 0 otherwise.
// Inferred: the halfword is a counter or timer that the pursue-to-idle exit waits on;
// the function name suggests it, the code only shows the zero test.
// Unknown: what writes the halfword and its unit.
extern "C" int bExitStatePersueIdle__8Creature(unsigned char* self) {
    return norrath_ao::load_signed_half(self + 794) == 0;
}
