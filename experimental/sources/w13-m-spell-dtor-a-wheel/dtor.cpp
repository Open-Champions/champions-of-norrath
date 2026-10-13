// Deleting destructor of Wheel (image label _$_5Wheel).
//
// Observed in the private image at 0x00316490 (116 bytes): stores the Wheel table at
// object+72, calls the AnimationState destructor on the member at object+84 with flag 0 (not
// 2, unlike the embedded lights), calls the GameObject destructor with flag 0, and releases
// the object when bit 0 of the incoming flags is set.
// Inferred: the AnimationState member is a plain member; flag 0 means its storage is not freed.
// Unknown: whether the Wheel table is shared with other vehicle parts.
#include "../w13-m-spell-dtor-a-common/dtor_common.hpp"

extern "C" {
extern unsigned char table_Wheel[] __asm__("_vt$5Wheel");
}

extern "C" void destructor_Wheel(void* self, int flags) __asm__("_$_5Wheel");
extern "C" void destructor_Wheel(void* self, int flags) {
    unsigned char* object = static_cast<unsigned char*>(self);
    const bool release = (flags & 1) != 0;
    store_table(object, 72, table_Wheel);
    animationStateDestructor(object + 84, 0);
    gameObjectDestructor(object, 0);
    if (release) blockFree__FPv(object);
}
