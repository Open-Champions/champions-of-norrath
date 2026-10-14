#include "../w10-actor-b-common/fields.hpp"
// Observed _$_9AutoChest at 0x314df0, 116 bytes (weak). In order:
//   1. store the 10AutoObject vtable (0x44ed78) at object+72;
//   2. the AnimationState destructor on object+152 with flag 2;
//   3. the GameObject destructor on the object with flag 0;
//   4. when bit 0 of the incoming flag is set, blockFree on the object (tail call).
// Other flag bits do not free, and the incoming flag is not passed on.
// Observed: the stored vtable is the 10AutoObject one in this class's destructor too,
// so the body matches _$_10AutoObject exactly (the two weak symbols are separate copies).
// Inferred: the slot at +72 is the vtable word, as in the neighbouring destructors.
extern "C" {
void animation_state_destructor(void* state, int flags) asm("_$_14AnimationState");
void game_object_destructor(void* object, int flags) asm("_$_10GameObject");
void norrath_block_free(void* object);
extern const unsigned char norrath_rtti_entities_a_view_44ed78[];
}

extern "C" void dtor_9AutoChest(void* object, int flags) asm("_$_9AutoChest");
extern "C" void dtor_9AutoChest(void* object, int flags) {
    using namespace actor_b;
    field<Word>(object, 72) = rtti_c::address_bits(norrath_rtti_entities_a_view_44ed78);
    animation_state_destructor(static_cast<unsigned char*>(object) + 152, 2);
    game_object_destructor(object, 0);
    if ((flags & 1) != 0) norrath_block_free(object);
}
