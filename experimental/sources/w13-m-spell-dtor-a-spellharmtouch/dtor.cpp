// Deleting destructor of SpellHarmTouch (image label _$_14SpellHarmTouch).
//
// Observed in the private image at 0x00317840 (100 bytes): stores the Spell table at
// object+20 (not object+72), releases the list head at object+4, and releases the object
// when bit 0 of the incoming flags is set. It calls no base destructor.
// Inferred: the Spell base sits at object+4 and has no destructor call of its own here.
// Unknown: why the Spell table word lives at +20 while the list head lives at +4.
#include "../w13-m-spell-dtor-a-common/dtor_common.hpp"

extern "C" {
extern unsigned char table_Spell[] __asm__("_vt$5Spell");
}

extern "C" void destructor_SpellHarmTouch(void* self, int flags) __asm__("_$_14SpellHarmTouch");
extern "C" void destructor_SpellHarmTouch(void* self, int flags) {
    unsigned char* object = static_cast<unsigned char*>(self);
    const bool release = (flags & 1) != 0;
    store_table(object, 20, table_Spell);
    listDestroy__FP8ListHead(object + 4);
    if (release) blockFree__FPv(object);
}
