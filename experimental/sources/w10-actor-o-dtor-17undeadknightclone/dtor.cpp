// Observed _$_17UndeadKnightClone at 0x00316158, 116 bytes (weak). Stores the 17UndeadKnightClone table pointer
// (_vt$17UndeadKnightClone at 0x44fb28) at object+72, then calls the Light destructor (_$_5Light) with flag 2 on the member at object+2496, then calls the Creature destructor (_$_8Creature) with
// flag 0 on the object, then calls blockFree__FPv on the object when bit 0 of the incoming flags
// is set. Other flag bits do not free. The free is a tail call in the image.
// Inferred: this is the 17UndeadKnightClone destructor from its symbol name. Nothing here ran on the EE.
extern "C" {
extern unsigned char vt_UndeadKnightClone[] __asm__("_vt$17UndeadKnightClone");
void creature_destructor(void* self, int flags) __asm__("_$_8Creature");
void blockFree__FPv(void* block);
void light_destructor(void* light, int flags) __asm__("_$_5Light");
}

extern "C" void destructor_UndeadKnightClone(void* self, int flags) __asm__("_$_17UndeadKnightClone");
extern "C" void destructor_UndeadKnightClone(void* self, int flags) {
    unsigned char* object = static_cast<unsigned char*>(self);
    *reinterpret_cast<unsigned*>(object + 72) = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(vt_UndeadKnightClone));
    light_destructor(object + 2496, 2);
    creature_destructor(object, 0);
    if ((flags & 1) != 0) blockFree__FPv(object);
}
