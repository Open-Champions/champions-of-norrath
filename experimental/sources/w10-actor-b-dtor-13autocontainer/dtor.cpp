#include "../w10-actor-b-common/fields.hpp"
// Observed _$_13AutoContainer at 0x221870, 204 bytes. In order:
//   1. store the 13AutoContainer vtable (0x44ebb0) at object+72;
//   2. when the word objectKillingEveryone is zero: for each of the words at +320 and
//      +324, in that order, when it is not null, call the object it addresses through
//      its virtual table's second entry (byte offset 8: a signed halfword adjustment,
//      then the function) with the adjusted pointer and the flag 3;
//   3. store the 10AutoObject vtable (0x44ed78) at object+72;
//   4. the AnimationState destructor on object+152 with flag 2;
//   5. the GameObject destructor on the object with flag 0;
//   6. when bit 0 of the incoming flag is set, blockFree on the object (tail call).
// Other flag bits do not free, and the incoming flag is not passed on.
// Inferred: the second table entry is the destructor, so +320 and +324 hold owned
// child objects that are deleted unless the world is being torn down.
// Unknown: what the two children are.
extern "C" {
extern int objectKillingEveryone;
void animation_state_destructor(void* state, int flags) asm("_$_14AnimationState");
void game_object_destructor(void* object, int flags) asm("_$_10GameObject");
void norrath_block_free(void* object);
extern const unsigned char norrath_ctors_ant_lantern_view_44ebb0[];
extern const unsigned char norrath_rtti_entities_a_view_44ed78[];
}

namespace {
// Virtual call of the table entry at byte offset 8: a signed halfword adjustment, then the function.
__attribute__((always_inline)) inline void delete_through_table(void* child) {
    using namespace actor_b;
    void* slot = static_cast<unsigned char*>(link(child, 72)) + 8;
    typedef void (*Destroy)(void*, int);
    Destroy function = reinterpret_cast<Destroy>(link(slot, 4));
    function(static_cast<unsigned char*>(child) + field<SHalf>(slot, 0), 3);
}
}

extern "C" void dtor_13AutoContainer(void* object, int flags) asm("_$_13AutoContainer");
extern "C" void dtor_13AutoContainer(void* object, int flags) {
    using namespace actor_b;
    field<Word>(object, 72) = rtti_c::address_bits(norrath_ctors_ant_lantern_view_44ebb0);
    if (objectKillingEveryone == 0) {
        if (field<Word>(object, 320) != 0) {
            delete_through_table(link(object, 320));
        }
        if (field<Word>(object, 324) != 0) {
            delete_through_table(link(object, 324));
        }
    }
    field<Word>(object, 72) = rtti_c::address_bits(norrath_rtti_entities_a_view_44ed78);
    animation_state_destructor(static_cast<unsigned char*>(object) + 152, 2);
    game_object_destructor(object, 0);
    if ((flags & 1) != 0) norrath_block_free(object);
}
