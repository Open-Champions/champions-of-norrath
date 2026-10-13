#include "../w9-rtti-vtables-entities-c-bytes/bytes.hpp"
// Observed msg_trigger__10DoorSecreti at 0x21c008, 68 bytes. When bit 0x100000
// of the flag word at object+12 is set, calls the virtual function in the slot
// at byte offset 112 of the table at object+72 with the object adjusted by
// that slot's halfword and the argument 0. Otherwise it does nothing. The
// incoming integer argument is not used and nothing is returned.
// Unknown: the meaning of flag 0x100000 and of the slot-112 argument.
extern "C" void msg_trigger__10DoorSecreti(void* self, int) {
    using namespace rtti_c;
    typedef void (*SlotCall)(void*, int);
    if ((read_word(address(self) + 12u) & 0x100000u) == 0u) return;
    void* adjusted;
    SlotCall call = slot_function<SlotCall>(self, 112u, &adjusted);
    call(adjusted, 0);
}
