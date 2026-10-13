#include "../vslots-b-bytes/bytes.hpp"
// Observed _$_9Generator at 0x0030efe0, 108 bytes (weak definition).
// Stores the _vt$9Generator vtable (0x44bbd0) at object+72, then listDestroy on the list head at
// object+984, then the Creature destructor with flag 0, then blockFree on the object when bit 0 of the
// incoming flags is set (tail call). Other bits do not free.
extern "C" {
extern const unsigned char norrath_ctors_ant_lantern_view_44bbd0[];
void listDestroy__FP8ListHead(void* head);
void _$_8Creature(void* object, int flags);
void blockFree__FPv(void* object);
}
extern "C" void _$_9Generator(void* self, int flags) {
    using namespace vslots_b;
    const Address object = address(self);
    write_word(object + 72u, address_bits(norrath_ctors_ant_lantern_view_44bbd0));
    listDestroy__FP8ListHead(bytes(object + 984u));
    _$_8Creature(self, 0);
    if ((flags & 1) != 0) blockFree__FPv(self);
}
