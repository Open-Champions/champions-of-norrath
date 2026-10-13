// Deleting destructor of HateBeam (image label _$_8HateBeam).
//
// Observed in the private image at 0x001ff5a8 (124 bytes): stores the HateBeam table at
// object+72, calls the Light destructor on the embedded light at object+104 with flag 2,
// releases the list head at object+76, calls the GameObject destructor with flag 0, and
// releases the object when bit 0 of the incoming flags is set. No deactivate call is made.
// Inferred: the embedded light is a member whose destructor alone releases it.
// Unknown: the meaning of the list head; the class's relation to SkillHateBeam, whose table
// is stored by a different image routine.
#include "../w13-m-spell-dtor-a-common/dtor_common.hpp"

extern "C" {
extern unsigned char table_HateBeam[] __asm__("_vt$8HateBeam");
}

extern "C" void destructor_HateBeam(void* self, int flags) __asm__("_$_8HateBeam");
extern "C" void destructor_HateBeam(void* self, int flags) {
    unsigned char* object = static_cast<unsigned char*>(self);
    const bool release = (flags & 1) != 0;
    store_table(object, 72, table_HateBeam);
    lightDestructor(object + 104, 2);
    listDestroy__FP8ListHead(object + 76);
    gameObjectDestructor(object, 0);
    if (release) blockFree__FPv(object);
}
