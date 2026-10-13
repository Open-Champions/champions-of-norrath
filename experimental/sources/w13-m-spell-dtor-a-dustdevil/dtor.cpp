// Deleting destructor of DustDevil (image label _$_9DustDevil).
//
// Observed in the private image at 0x0023c0e0 (108 bytes): stores the DustDevil table at
// object+72, passes the int at object+84 to P_DeleteParticleSystem, calls the GameObject
// destructor with flag 0, and releases the object when bit 0 of the incoming flags is set.
// Inferred: the int at +84 is a particle-system handle owned by the object.
// Unknown: whether the handle is ever zero or reset by the caller.
#include "../w13-m-spell-dtor-a-common/dtor_common.hpp"

extern "C" {
extern unsigned char table_DustDevil[] __asm__("_vt$9DustDevil");
}

extern "C" void destructor_DustDevil(void* self, int flags) __asm__("_$_9DustDevil");
extern "C" void destructor_DustDevil(void* self, int flags) {
    unsigned char* object = static_cast<unsigned char*>(self);
    const bool release = (flags & 1) != 0;
    store_table(object, 72, table_DustDevil);
    P_DeleteParticleSystem__Fi(*reinterpret_cast<int*>(object + 84));
    gameObjectDestructor(object, 0);
    if (release) blockFree__FPv(object);
}
