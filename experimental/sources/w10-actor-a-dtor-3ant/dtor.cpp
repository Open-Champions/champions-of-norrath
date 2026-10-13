// Observed _$_3Ant at 0x0030e848, 100 bytes (weak).
// Stores the Ant table pointer (_vt$3Ant) at object+72, calls the Creature destructor
// (_$_8Creature) with flag 0, then calls blockFree__FPv on the object when bit 0 of the incoming
// flags is set. Other flag bits do not free. The free is a tail call in the image.
// Inferred: this is the Ant destructor from its symbol name. Nothing here ran on the EE.
extern "C" {
extern unsigned char vt_Ant[] __asm__("_vt$3Ant");
void creature_destructor(void* self, int flags) __asm__("_$_8Creature");
void blockFree__FPv(void* block);
}

extern "C" void ant_destructor(void* self, int flags) __asm__("_$_3Ant");
extern "C" void ant_destructor(void* self, int flags) {
    unsigned char* object = static_cast<unsigned char*>(self);
    *reinterpret_cast<unsigned*>(object + 72) = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(vt_Ant));
    creature_destructor(object, 0);
    if ((flags & 1) != 0) blockFree__FPv(object);
}
