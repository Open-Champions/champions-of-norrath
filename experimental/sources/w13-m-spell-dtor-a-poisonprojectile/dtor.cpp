// Deleting destructor of PoisonProjectile (image label _$_16PoisonProjectile).
//
// Observed in the private image at 0x00314ad8 (132 bytes): stores the PoisonProjectile table
// at object+72, calls the AnimationState destructor on the member at object+100 with flag 2,
// stores the Projectile table at object+72, releases the list head at object+76, calls the
// GameObject destructor with flag 0, and releases the object when bit 0 of the incoming flags
// is set.
// Inferred: the Projectile table is restored before the base destructor, as in DiseaseBolt.
// Unknown: the meaning of the list head.
#include "../w13-m-spell-dtor-a-common/dtor_common.hpp"

extern "C" {
extern unsigned char table_PoisonProjectile[] __asm__("_vt$16PoisonProjectile");
extern unsigned char table_Projectile[] __asm__("_vt$10Projectile");
}

extern "C" void destructor_PoisonProjectile(void* self, int flags) __asm__("_$_16PoisonProjectile");
extern "C" void destructor_PoisonProjectile(void* self, int flags) {
    unsigned char* object = static_cast<unsigned char*>(self);
    const bool release = (flags & 1) != 0;
    store_table(object, 72, table_PoisonProjectile);
    animationStateDestructor(object + 100, 2);
    store_table(object, 72, table_Projectile);
    listDestroy__FP8ListHead(object + 76);
    gameObjectDestructor(object, 0);
    if (release) blockFree__FPv(object);
}
