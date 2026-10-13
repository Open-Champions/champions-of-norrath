#include "../vslots-b-bytes/bytes.hpp"
// Observed _$_7FireFly at 0x0030ee18, 124 bytes (weak definition).
//  1. Stores the _vt$7FireFly vtable (0x44bdb0) at object+72.
//  2. Destroys the light-sized member at object+1064 with _$_5Light and flag 2.
//  3. Stores the _vt$10FireBeetle vtable (0x44bea0) at object+72, i.e. the base-class table, and calls the
//     Creature destructor with flag 0. The FireBeetle destructor itself is not called.
//  4. blockFree on the object when bit 0 of the incoming flags is set (tail call). Other bits do not free.
// Inferred: the member at +1064 is a light, from the callee name only.
extern "C" {
extern const unsigned char norrath_ctors_ant_lantern_view_44bdb0[];
extern const unsigned char norrath_ctors_ant_lantern_view_44bea0[];
void _$_5Light(void* light, int flags);
void _$_8Creature(void* object, int flags);
void blockFree__FPv(void* object);
}
extern "C" void _$_7FireFly(void* self, int flags) {
    using namespace vslots_b;
    const Address object = address(self);
    write_word(object + 72u, address_bits(norrath_ctors_ant_lantern_view_44bdb0));
    _$_5Light(bytes(object + 1064u), 2);
    write_word(object + 72u, address_bits(norrath_ctors_ant_lantern_view_44bea0));
    _$_8Creature(self, 0);
    if ((flags & 1) != 0) blockFree__FPv(self);
}
