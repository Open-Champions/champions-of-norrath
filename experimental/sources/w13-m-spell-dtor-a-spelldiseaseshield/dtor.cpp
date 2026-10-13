// Deleting destructor of SpellDiseaseShield (image label _$_18SpellDiseaseShield).
//
// Observed in the private image at 0x00316f78 (100 bytes): stores the Spell table at
// object+20, releases the list head at object+4, and releases the object when bit 0 of the
// incoming flags is set. It calls no base destructor.
// Inferred: the same Spell base layout as SpellHarmTouch.
// Unknown: the meaning of the list head.
#include "../w13-m-spell-dtor-a-common/dtor_common.hpp"

extern "C" {
extern unsigned char table_Spell[] __asm__("_vt$5Spell");
}

extern "C" void destructor_SpellDiseaseShield(void* self, int flags) __asm__("_$_18SpellDiseaseShield");
extern "C" void destructor_SpellDiseaseShield(void* self, int flags) {
    unsigned char* object = static_cast<unsigned char*>(self);
    const bool release = (flags & 1) != 0;
    store_table(object, 20, table_Spell);
    listDestroy__FP8ListHead(object + 4);
    if (release) blockFree__FPv(object);
}
