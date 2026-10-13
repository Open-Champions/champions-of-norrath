// Deleting destructor of MultiFireProjectile (image label _$_19MultiFireProjectile).
//
// Observed in the private image at 0x00317fc8 (108 bytes): stores the Projectile table at
// object+72 (the class's own table is not stored here), releases the list head at object+76,
// calls the GameObject destructor with flag 0, and releases the object when bit 0 of the
// incoming flags is set.
// Inferred: the class has no destructor-time table of its own; the base Projectile table
// stands for the whole object while the base destructors run.
// Unknown: the meaning of the list head.
#include "../w13-m-spell-dtor-a-common/dtor_common.hpp"

extern "C" {
extern unsigned char table_Projectile[] __asm__("_vt$10Projectile");
}

extern "C" void destructor_MultiFireProjectile(void* self, int flags) __asm__("_$_19MultiFireProjectile");
extern "C" void destructor_MultiFireProjectile(void* self, int flags) {
    unsigned char* object = static_cast<unsigned char*>(self);
    const bool release = (flags & 1) != 0;
    store_table(object, 72, table_Projectile);
    listDestroy__FP8ListHead(object + 76);
    gameObjectDestructor(object, 0);
    if (release) blockFree__FPv(object);
}
