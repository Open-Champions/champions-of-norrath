#include "../w9-rtti-vtables-entities-c-bytes/bytes.hpp"
// Observed setPassableState__8HackDoorb at 0x226548, 48 bytes. Calls
// setPassableState__8AutoPropb with the same object and flag, then sets bit 1
// of the flag word at object+12. Nothing is returned.
// Inferred: HackDoor extends the AutoProp behaviour. Unknown: what bit 1 means.
extern "C" void setPassableState__8AutoPropb(void* self, int passable);

extern "C" void setPassableState__8HackDoorb(void* self, int passable) {
    using namespace rtti_c;
    setPassableState__8AutoPropb(self, passable);
    const Address flags = address(self) + 12u;
    write_word(flags, read_word(flags) | 1u);
}
