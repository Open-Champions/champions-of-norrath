// Observed _$_10CloudGiant at 0x00313f60, 100 bytes (weak).
// Stores the CloudGiant table pointer (_vt$10CloudGiant) at object+72, calls the Creature destructor
// (_$_8Creature) with flag 0, then calls blockFree__FPv on the object when bit 0 of the incoming
// flags is set. Other flag bits do not free. The free is a tail call in the image.
// Inferred: this is the CloudGiant destructor from its symbol name. Nothing here ran on the EE.
extern "C" {
extern unsigned char vt_10CloudGiant[] __asm__("_vt$10CloudGiant");
void creature_destructor(void* self, int flags) __asm__("_$_8Creature");
void blockFree__FPv(void* block);
}

extern "C" void cloudgiant_destructor(void* self, int flags) __asm__("_$_10CloudGiant");
extern "C" void cloudgiant_destructor(void* self, int flags) {
    unsigned char* object = static_cast<unsigned char*>(self);
    *reinterpret_cast<unsigned*>(object + 72) = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(vt_10CloudGiant));
    creature_destructor(object, 0);
    if ((flags & 1) != 0) blockFree__FPv(object);
}
