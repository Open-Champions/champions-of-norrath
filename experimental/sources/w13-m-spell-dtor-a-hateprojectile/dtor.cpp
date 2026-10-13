// Deleting destructor of HateProjectile (image label _$_14HateProjectile).
//
// Observed in the private image at 0x001ff628 (140 bytes): stores the HateProjectile table at
// object+72, releases the list head at object+392, calls the Light destructor on the embedded
// light at object+324 with flag 2, releases the list heads at object+84 and object+80, calls
// the GameObject destructor with flag 0, and releases the object when bit 0 of the incoming
// flags is set. The image does not deactivate the light here, unlike ShockOfFrost.
// Inferred: the light is a member and its destructor alone releases it.
// Unknown: the meaning of the list heads.
#include "../w13-m-spell-dtor-a-common/dtor_common.hpp"

extern "C" {
extern unsigned char table_HateProjectile[] __asm__("_vt$14HateProjectile");
}

extern "C" void destructor_HateProjectile(void* self, int flags) __asm__("_$_14HateProjectile");
extern "C" void destructor_HateProjectile(void* self, int flags) {
    unsigned char* object = static_cast<unsigned char*>(self);
    const bool release = (flags & 1) != 0;
    store_table(object, 72, table_HateProjectile);
    listDestroy__FP8ListHead(object + 392);
    lightDestructor(object + 324, 2);
    listDestroy__FP8ListHead(object + 84);
    listDestroy__FP8ListHead(object + 80);
    gameObjectDestructor(object, 0);
    if (release) blockFree__FPv(object);
}
