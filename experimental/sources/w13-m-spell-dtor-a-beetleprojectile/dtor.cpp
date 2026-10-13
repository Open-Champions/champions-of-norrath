// Deleting destructor of beetleProjectile (image label _$_16beetleProjectile).
//
// Observed in the private image at 0x003114f0 (108 bytes): stores the beetleProjectile table
// at object+72, releases the list head at object+452, calls the MissileWeapon destructor on
// the object with flag 0, and releases the object when bit 0 of the incoming flags is set.
// Inferred: the MissileWeapon base is the direct base of this class.
// Unknown: the meaning of the list head at +452.
#include "../w13-m-spell-dtor-a-common/dtor_common.hpp"

extern "C" {
extern unsigned char table_beetleProjectile[] __asm__("_vt$16beetleProjectile");
}

extern "C" void destructor_beetleProjectile(void* self, int flags) __asm__("_$_16beetleProjectile");
extern "C" void destructor_beetleProjectile(void* self, int flags) {
    unsigned char* object = static_cast<unsigned char*>(self);
    const bool release = (flags & 1) != 0;
    store_table(object, 72, table_beetleProjectile);
    listDestroy__FP8ListHead(object + 452);
    missileWeaponDestructor(object, 0);
    if (release) blockFree__FPv(object);
}
