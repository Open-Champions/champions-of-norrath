#include "../vslots-b-bytes/bytes.hpp"
// Observed msg_route__8Creaturei at 0x113488, 44 bytes.
// The word at object+708 is cleared for every argument. A nonzero argument
// also clears object+716. A zero argument stores -1 at object+584 and
// object+580, then adds one to the word at object+716. Other bytes stay.
// Unknown: what those words mean to a later route step.
extern "C" void msg_route__8Creaturei(void* self, int value) {
    using namespace vslots_b;
    const Address base = address(self);
    write_word(base + 708u, 0u);
    if (value != 0) {
        write_word(base + 716u, 0u);
        return;
    }
    const unsigned next = read_word(base + 716u) + 1u;
    write_word(base + 584u, 0xffffffffu);
    write_word(base + 580u, 0xffffffffu);
    write_word(base + 716u, next);
}
