// Deleting destructor of PoisonRain (image label _$_10PoisonRain).
//
// Observed in the private image at 0x0031a078 (108 bytes): stores the PoisonRain table at
// object+72, releases the list head at object+100, calls the GameObject destructor with
// flag 0, and releases the object when bit 0 of the incoming flags is set.
// Inferred: the list head is a member of the derived class.
// Unknown: the meaning of the list head and the stored table.
#include "../w13-m-spell-dtor-a-common/dtor_common.hpp"

extern "C" {
extern unsigned char table_PoisonRain[] __asm__("_vt$10PoisonRain");
}

extern "C" void destructor_PoisonRain(void* self, int flags) __asm__("_$_10PoisonRain");
extern "C" void destructor_PoisonRain(void* self, int flags) {
    unsigned char* object = static_cast<unsigned char*>(self);
    const bool release = (flags & 1) != 0;
    store_table(object, 72, table_PoisonRain);
    listDestroy__FP8ListHead(object + 100);
    gameObjectDestructor(object, 0);
    if (release) blockFree__FPv(object);
}
