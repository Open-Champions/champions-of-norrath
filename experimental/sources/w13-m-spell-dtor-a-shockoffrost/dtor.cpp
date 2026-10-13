// Deleting destructor of ShockOfFrost (image label _$_12ShockOfFrost).
//
// Observed in the private image at 0x002bc978 (168 bytes): stores the ShockOfFrost table at
// object+72, calls lightDeactivate on the embedded light at object+88, calls the Light
// destructor on it with flag 2, releases the list head at object+84, stores the Projectile
// table at object+72, releases the list head at object+76, calls the GameObject destructor
// with flag 0, and releases the object when bit 0 of the incoming flags is set.
// Inferred: the embedded light belongs to the spell and is deactivated before it is destroyed.
// Unknown: the meaning of the list heads.
#include "../w13-m-spell-dtor-a-common/dtor_common.hpp"

extern "C" {
extern unsigned char table_ShockOfFrost[] __asm__("_vt$12ShockOfFrost");
extern unsigned char table_Projectile[] __asm__("_vt$10Projectile");
}

extern "C" void destructor_ShockOfFrost(void* self, int flags) __asm__("_$_12ShockOfFrost");
extern "C" void destructor_ShockOfFrost(void* self, int flags) {
    unsigned char* object = static_cast<unsigned char*>(self);
    const bool release = (flags & 1) != 0;
    store_table(object, 72, table_ShockOfFrost);
    lightDeactivate__FP5Light(object + 88);
    lightDestructor(object + 88, 2);
    listDestroy__FP8ListHead(object + 84);
    store_table(object, 72, table_Projectile);
    listDestroy__FP8ListHead(object + 76);
    gameObjectDestructor(object, 0);
    if (release) blockFree__FPv(object);
}
