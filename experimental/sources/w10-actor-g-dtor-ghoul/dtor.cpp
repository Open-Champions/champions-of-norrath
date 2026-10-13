// Observed _$_5Ghoul at 0x0030f0a8, 100 bytes (weak).
// Stores the Ghoul table pointer (_vt$5Ghoul) at object+72, calls the Creature destructor
// (_$_8Creature) with flag 0, then calls blockFree__FPv on the object when bit 0 of the incoming
// flags is set. Other flag bits do not free. The free is a tail call in the image.
// Inferred: this is the Ghoul destructor from its symbol name. Nothing here ran on the EE.
extern "C" {
extern unsigned char vt_Ghoul[] __asm__("_vt$5Ghoul");
void creature_destructor(void* self, int flags) __asm__("_$_8Creature");
void blockFree__FPv(void* block);
}

extern "C" void ghoul_destructor(void* self, int flags) __asm__("_$_5Ghoul");
extern "C" void ghoul_destructor(void* self, int flags) {
    unsigned char* object = static_cast<unsigned char*>(self);
    *reinterpret_cast<unsigned*>(object + 72) = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(vt_Ghoul));
    creature_destructor(object, 0);
    if ((flags & 1) != 0) blockFree__FPv(object);
}
