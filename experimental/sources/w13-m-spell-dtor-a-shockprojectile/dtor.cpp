// Deleting destructor of ShockProjectile (image label _$_15ShockProjectile).
//
// Observed in the private image at 0x0028d978 (140 bytes): stores the ShockProjectile table at
// object+72, releases the list head at object+404, calls the Light destructor on the embedded
// light at object+332 with flag 2, releases the list heads at object+84 and object+80, calls
// the GameObject destructor with flag 0, and releases the object when bit 0 of the incoming
// flags is set.
// Unknown: the meaning of the list heads at +404, +84 and +80.
#include "../w13-m-spell-dtor-a-common/dtor_common.hpp"

extern "C" {
extern unsigned char table_ShockProjectile[] __asm__("_vt$15ShockProjectile");
}

extern "C" void destructor_ShockProjectile(void* self, int flags) __asm__("_$_15ShockProjectile");
extern "C" void destructor_ShockProjectile(void* self, int flags) {
    unsigned char* object = static_cast<unsigned char*>(self);
    const bool release = (flags & 1) != 0;
    store_table(object, 72, table_ShockProjectile);
    listDestroy__FP8ListHead(object + 404);
    lightDestructor(object + 332, 2);
    listDestroy__FP8ListHead(object + 84);
    listDestroy__FP8ListHead(object + 80);
    gameObjectDestructor(object, 0);
    if (release) blockFree__FPv(object);
}
