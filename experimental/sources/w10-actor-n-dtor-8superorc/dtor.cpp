// Observed _$_8SuperOrc at 0x0030f740, 100 bytes (weak). Stores the 8SuperOrc table pointer
// (_vt$8SuperOrc) at object+72, calls the Creature destructor (_$_8Creature) with flag 0, then
// calls blockFree__FPv on the object when bit 0 of the incoming flags is set. Other flag bits do
// not free. The free is a tail call in the image.
// Inferred: this is the 8SuperOrc destructor from its symbol name. Nothing here ran on the EE.
extern "C" {
extern unsigned char vt_8SuperOrc[] __asm__("_vt$8SuperOrc");
void creature_destructor(void* self, int flags) __asm__("_$_8Creature");
void blockFree__FPv(void* block);
}

extern "C" void destructor_8SuperOrc(void* self, int flags) __asm__("_$_8SuperOrc");
extern "C" void destructor_8SuperOrc(void* self, int flags) {
    unsigned char* object = static_cast<unsigned char*>(self);
    *reinterpret_cast<unsigned*>(object + 72) = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(vt_8SuperOrc));
    creature_destructor(object, 0);
    if ((flags & 1) != 0) blockFree__FPv(object);
}
