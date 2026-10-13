// Deleting destructor of MummySnake (image label _$_10MummySnake).
//
// Observed in the private image at 0x00315e90 (116 bytes): stores the MummySnake table at
// object+72, releases the list heads at object+80 and object+76 in that order, calls the
// GameObject destructor on the object with flag 0, and releases the object when bit 0 of
// the incoming flags is set.
// Inferred: this is the destructor body of the class; the list heads are members.
// Unknown: the meaning of the two list heads and the meaning of the stored table.
#include "../w13-m-spell-dtor-a-common/dtor_common.hpp"

extern "C" {
extern unsigned char table_MummySnake[] __asm__("_vt$10MummySnake");
}

extern "C" void destructor_MummySnake(void* self, int flags) __asm__("_$_10MummySnake");
extern "C" void destructor_MummySnake(void* self, int flags) {
    unsigned char* object = static_cast<unsigned char*>(self);
    const bool release = (flags & 1) != 0;
    store_table(object, 72, table_MummySnake);
    listDestroy__FP8ListHead(object + 80);
    listDestroy__FP8ListHead(object + 76);
    gameObjectDestructor(object, 0);
    if (release) blockFree__FPv(object);
}
