// Deleting destructor of SpawnMummy (image label _$_10SpawnMummy).
//
// Observed in the private image at 0x00315d78 (140 bytes): stores the SpawnMummy table at
// object+72, releases the list heads at object+100 and object+96, stores the DustDevil table
// at object+72, passes the int at object+84 to P_DeleteParticleSystem, calls the GameObject
// destructor with flag 0, and releases the object when bit 0 of the incoming flags is set.
// Inferred: the derived class hands its particle handle back to the particle system here.
// Unknown: the meaning of the two list heads.
#include "../w13-m-spell-dtor-a-common/dtor_common.hpp"

extern "C" {
extern unsigned char table_SpawnMummy[] __asm__("_vt$10SpawnMummy");
extern unsigned char table_DustDevil[] __asm__("_vt$9DustDevil");
}

extern "C" void destructor_SpawnMummy(void* self, int flags) __asm__("_$_10SpawnMummy");
extern "C" void destructor_SpawnMummy(void* self, int flags) {
    unsigned char* object = static_cast<unsigned char*>(self);
    const bool release = (flags & 1) != 0;
    store_table(object, 72, table_SpawnMummy);
    listDestroy__FP8ListHead(object + 100);
    listDestroy__FP8ListHead(object + 96);
    store_table(object, 72, table_DustDevil);
    P_DeleteParticleSystem__Fi(*reinterpret_cast<int*>(object + 84));
    gameObjectDestructor(object, 0);
    if (release) blockFree__FPv(object);
}
