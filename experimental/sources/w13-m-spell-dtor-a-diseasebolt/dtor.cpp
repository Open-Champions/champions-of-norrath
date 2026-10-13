// Deleting destructor of DiseaseBolt (image label _$_11DiseaseBolt).
//
// Observed in the private image at 0x00316c78 (140 bytes): stores the DiseaseBolt table at
// object+72, releases the list heads at object+88 and object+84, stores the Projectile table
// at object+72, releases the list head at object+76, calls the GameObject destructor with
// flag 0, and releases the object when bit 0 of the incoming flags is set.
// Inferred: the Projectile table is the base-class table restored before the base destructor.
// Unknown: the meaning of the three list heads.
#include "../w13-m-spell-dtor-a-common/dtor_common.hpp"

extern "C" {
extern unsigned char table_DiseaseBolt[] __asm__("_vt$11DiseaseBolt");
extern unsigned char table_Projectile[] __asm__("_vt$10Projectile");
}

extern "C" void destructor_DiseaseBolt(void* self, int flags) __asm__("_$_11DiseaseBolt");
extern "C" void destructor_DiseaseBolt(void* self, int flags) {
    unsigned char* object = static_cast<unsigned char*>(self);
    const bool release = (flags & 1) != 0;
    store_table(object, 72, table_DiseaseBolt);
    listDestroy__FP8ListHead(object + 88);
    listDestroy__FP8ListHead(object + 84);
    store_table(object, 72, table_Projectile);
    listDestroy__FP8ListHead(object + 76);
    gameObjectDestructor(object, 0);
    if (release) blockFree__FPv(object);
}
