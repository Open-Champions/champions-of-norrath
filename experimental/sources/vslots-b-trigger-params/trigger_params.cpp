#include "../vslots-b-bytes/bytes.hpp"
// Observed msg_trigger__13TriggerParamsi at 0x1d1a60, 44 bytes.
// A zero word at object+76 returns. Otherwise that word is a pointer.
// A nonzero argument stores 0 at the target+100. A zero argument stores -1.
// The argument value itself is not stored.
extern "C" void msg_trigger__13TriggerParamsi(void* self, int value) {
    using namespace vslots_b;
    const unsigned bits = read_word(address(self) + 76u);
    if (bits == 0u) return;
    const Address target = address(pointer(bits));
    write_word(target + 100u, value != 0 ? 0u : 0xffffffffu);
}
