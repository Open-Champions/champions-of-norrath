#include "../vslots-b-bytes/bytes.hpp"
// Observed stun__8Creaturei at 0x112be0, 80 bytes.
// When the signed halfword at object+572 is 10, nothing is written.
// Otherwise the word at object+808 plus the argument is stored at object+804,
// and virtual slot byte 184 is called with this adjusted by that slot's
// signed halfword and with integer 9. The creature vtable names that slot
// setState__8Creaturei. Unknown: which state 9 and 10 select.
extern "C" void stun__8Creaturei(void* self, int duration) {
    using namespace vslots_b;
    const Address base = address(self);
    if (read_i16(base + 572u) == 10) return;
    write_word(base + 804u, read_word(base + 808u) + static_cast<unsigned>(duration));
    void* adjusted = nullptr;
    using StateCall = void (*)(void*, int);
    StateCall call = slot_function<StateCall>(self, 184u, &adjusted);
    call(adjusted, 9);
}
