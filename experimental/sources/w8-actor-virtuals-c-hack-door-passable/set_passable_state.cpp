#include "../w8-actor-virtuals-c-bytes/bytes.hpp"
// Observed setPassableState__8HackDoorb at 0x226548, 48 bytes. The object and the
// bool go unchanged to setPassableState__8AutoPropb (defined by another source
// in this package), then bit 0x1 of the word at object+12 is set. No value is
// returned. Unknown: the meaning of that flag bit.
extern "C" void setPassableState__8AutoPropb(void* self, int passable);
extern "C" void setPassableState__8HackDoorb(void* self, int passable) {
    setPassableState__8AutoPropb(self, passable);
    w8c::write_word(w8c::address(self) + 12u, w8c::read_word(w8c::address(self) + 12u) | 1u);
}
