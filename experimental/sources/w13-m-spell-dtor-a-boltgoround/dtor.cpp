// Deleting destructor of BoltGoRound (image label _$_11BoltGoRound).
//
// Observed in the private image at 0x00313ea8 (100 bytes): stores the BoltGoRound table at
// object+72, calls the GameObject destructor with flag 0, and releases the object when bit 0
// of the incoming flags is set. It releases no members.
// Inferred: the class adds no member storage of its own beyond the base.
// Unknown: whether the table is the class's own or a shared table.
#include "../w13-m-spell-dtor-a-common/dtor_common.hpp"

extern "C" {
extern unsigned char table_BoltGoRound[] __asm__("_vt$11BoltGoRound");
}

extern "C" void destructor_BoltGoRound(void* self, int flags) __asm__("_$_11BoltGoRound");
extern "C" void destructor_BoltGoRound(void* self, int flags) {
    unsigned char* object = static_cast<unsigned char*>(self);
    const bool release = (flags & 1) != 0;
    store_table(object, 72, table_BoltGoRound);
    gameObjectDestructor(object, 0);
    if (release) blockFree__FPv(object);
}
