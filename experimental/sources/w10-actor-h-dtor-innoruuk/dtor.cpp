// Observed _$_8Innoruuk at 0x00314708, 124 bytes (weak).
// Stores the Innoruuk table pointer (_vt$8Innoruuk) at object+72, calls the Light destructor on the
// member at object+1016 with flag 2, listDestroy__FP8ListHead on object+988, then the Creature
// destructor (_$_8Creature) on the object with flag 0, then calls blockFree__FPv on the object when
// bit 0 of the incoming flags is set. The free is a tail call in the image.
// Inferred: this is the Innoruuk destructor from its symbol name. Nothing here ran on the EE.
extern "C" {
extern unsigned char vt_Innoruuk[] __asm__("_vt$8Innoruuk");
void light_destructor(void* self, int flags) __asm__("_$_5Light");
void list_destroy(void* head) __asm__("listDestroy__FP8ListHead");
void creature_destructor(void* self, int flags) __asm__("_$_8Creature");
void blockFree__FPv(void* block);
}

extern "C" void innoruuk_destructor(void* self, int flags) __asm__("_$_8Innoruuk");
extern "C" void innoruuk_destructor(void* self, int flags) {
    unsigned char* object = static_cast<unsigned char*>(self);
    *reinterpret_cast<unsigned*>(object + 72) = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(vt_Innoruuk));
    light_destructor(object + 1016, 2);
    list_destroy(object + 988);
    creature_destructor(object, 0);
    if ((flags & 1) != 0) blockFree__FPv(object);
}
