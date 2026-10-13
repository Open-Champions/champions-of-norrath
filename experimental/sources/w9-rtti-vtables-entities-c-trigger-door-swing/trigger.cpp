#include "../w9-rtti-vtables-entities-c-bytes/bytes.hpp"
// Observed msg_trigger__9DoorSwingi at 0x21bf50, 52 bytes. Calls the virtual
// function in the slot at byte offset 112 of the table at object+72, with the
// object adjusted by that slot's halfword and the argument -10. The incoming
// integer argument is not used and nothing is returned.
// Inferred: slot 112 takes an integer and is the same entry DoorSecret calls
// with zero. Unknown: what the argument -10 selects.
extern "C" void msg_trigger__9DoorSwingi(void* self, int) {
    using namespace rtti_c;
    typedef void (*SlotCall)(void*, int);
    void* adjusted;
    SlotCall call = slot_function<SlotCall>(self, 112u, &adjusted);
    call(adjusted, -10);
}
