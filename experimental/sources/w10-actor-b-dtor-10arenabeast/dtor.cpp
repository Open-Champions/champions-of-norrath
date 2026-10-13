#include "../w10-actor-b-common/fields.hpp"
// Observed _$_10Arenabeast at 0x315bb8, 100 bytes. In order:
//   1. store the 10Arenabeast vtable (0x44f6a8) at object+72;
//   2. the 8Creature destructor on the object with flag 0;
//   3. when bit 0 of the incoming flag is set, blockFree on the object (tail call).
// Other flag bits do not free, and the incoming flag is not passed on.
// Inferred: the slot at +72 is the vtable word, as in the neighbouring destructors.
extern "C" {
void base_destructor(void* object, int flags) asm("_$_8Creature");
void norrath_block_free(void* object);
extern const unsigned char norrath_ctors_ant_lantern_view_44f6a8[];
}

extern "C" void dtor_10Arenabeast(void* object, int flags) asm("_$_10Arenabeast");
extern "C" void dtor_10Arenabeast(void* object, int flags) {
    using namespace actor_b;
    field<Word>(object, 72) = rtti_c::address_bits(norrath_ctors_ant_lantern_view_44f6a8);
    base_destructor(object, 0);
    if ((flags & 1) != 0) norrath_block_free(object);
}
