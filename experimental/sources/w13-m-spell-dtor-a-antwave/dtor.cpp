// Deleting destructor of AntWave (image label _$_7AntWave).
//
// Observed in the private image at 0x00208cd0 (144 bytes): stores the AntWave table at
// object+72; if the word at object+968 is nonzero, releases the list head at object+968; then
// releases the list head at object+968 again unconditionally; calls the Creature destructor
// with flag 0; and releases the object when bit 0 of the incoming flags is set.
// The second release of the same head is what the image does; it is kept as observed.
// Unknown: why the head is released twice, and what the word at +968 holds (a head pointer
// or a count).
#include "../w13-m-spell-dtor-a-common/dtor_common.hpp"

extern "C" {
extern unsigned char table_AntWave[] __asm__("_vt$7AntWave");
}

extern "C" void destructor_AntWave(void* self, int flags) __asm__("_$_7AntWave");
extern "C" void destructor_AntWave(void* self, int flags) {
    unsigned char* object = static_cast<unsigned char*>(self);
    const bool release = (flags & 1) != 0;
    store_table(object, 72, table_AntWave);
    if (*reinterpret_cast<unsigned*>(object + 968) != 0) listDestroy__FP8ListHead(object + 968);
    listDestroy__FP8ListHead(object + 968);
    creatureDestructor(object, 0);
    if (release) blockFree__FPv(object);
}
