// Deleting destructor of FrostStormSpell (image label _$_15FrostStormSpell).
//
// Observed in the private image at 0x00312a00 (124 bytes): stores the FrostStormSpell table
// at object+72, calls the Light destructor on the embedded light at object+112 with flag 2,
// releases the list head at object+108, calls the GameObject destructor with flag 0, and
// releases the object when bit 0 of the incoming flags is set. No deactivate call is made.
// Inferred: the embedded light is a member whose destructor alone releases it.
// Unknown: the meaning of the list head.
#include "../w13-m-spell-dtor-a-common/dtor_common.hpp"

extern "C" {
extern unsigned char table_FrostStormSpell[] __asm__("_vt$15FrostStormSpell");
}

extern "C" void destructor_FrostStormSpell(void* self, int flags) __asm__("_$_15FrostStormSpell");
extern "C" void destructor_FrostStormSpell(void* self, int flags) {
    unsigned char* object = static_cast<unsigned char*>(self);
    const bool release = (flags & 1) != 0;
    store_table(object, 72, table_FrostStormSpell);
    lightDestructor(object + 112, 2);
    listDestroy__FP8ListHead(object + 108);
    gameObjectDestructor(object, 0);
    if (release) blockFree__FPv(object);
}
