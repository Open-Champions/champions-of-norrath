#include "../vslots-b-bytes/bytes.hpp"
// Observed _$_8Froglock at 0x0030ef28, 100 bytes (weak definition).
// Stores the _vt$8Froglock vtable (0x44bcc0) at object+72, calls the Creature destructor with flag 0,
// then blockFree on the object when bit 0 of the incoming flags is set (tail call). Other bits do not free.
extern "C" {
extern const unsigned char norrath_ctors_ant_lantern_view_44bcc0[];
void _$_8Creature(void* object, int flags);
void blockFree__FPv(void* object);
}
extern "C" void _$_8Froglock(void* self, int flags) {
    using namespace vslots_b;
    write_word(address(self) + 72u, address_bits(norrath_ctors_ant_lantern_view_44bcc0));
    _$_8Creature(self, 0);
    if ((flags & 1) != 0) blockFree__FPv(self);
}
