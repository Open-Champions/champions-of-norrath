// Deleting destructor of HateSoul (image label _$_8HateSoul).
//
// Observed in the private image at 0x003147d8 (132 bytes): stores the HateSoul table at
// object+72, calls the Light destructor on the embedded light at object+100 with flag 2,
// stores the Projectile table at object+72, releases the list head at object+76, calls the
// GameObject destructor with flag 0, and releases the object when bit 0 of the incoming flags
// is set.
// Inferred: the Projectile table is restored before the base destructor, as in DiseaseBolt.
// Unknown: the meaning of the list head.
#include "../w13-m-spell-dtor-a-common/dtor_common.hpp"

extern "C" {
extern unsigned char table_HateSoul[] __asm__("_vt$8HateSoul");
extern unsigned char table_Projectile[] __asm__("_vt$10Projectile");
}

extern "C" void destructor_HateSoul(void* self, int flags) __asm__("_$_8HateSoul");
extern "C" void destructor_HateSoul(void* self, int flags) {
    unsigned char* object = static_cast<unsigned char*>(self);
    const bool release = (flags & 1) != 0;
    store_table(object, 72, table_HateSoul);
    lightDestructor(object + 100, 2);
    store_table(object, 72, table_Projectile);
    listDestroy__FP8ListHead(object + 76);
    gameObjectDestructor(object, 0);
    if (release) blockFree__FPv(object);
}
