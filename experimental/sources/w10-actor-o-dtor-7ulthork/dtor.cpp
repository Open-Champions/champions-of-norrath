// Observed _$_7Ulthork at 0x00314340, 100 bytes (weak). Stores the 7Ulthork table pointer
// (_vt$7Ulthork at 0x44dfe0) at object+72, calls the Creature destructor (_$_8Creature) with
// flag 0 on the object, then calls blockFree__FPv on the object when bit 0 of the incoming flags
// is set. Other flag bits do not free. The free is a tail call in the image.
// Inferred: this is the 7Ulthork destructor from its symbol name. Nothing here ran on the EE.
extern "C" {
extern unsigned char vt_Ulthork[] __asm__("_vt$7Ulthork");
void creature_destructor(void* self, int flags) __asm__("_$_8Creature");
void blockFree__FPv(void* block);
}

extern "C" void destructor_Ulthork(void* self, int flags) __asm__("_$_7Ulthork");
extern "C" void destructor_Ulthork(void* self, int flags) {
    unsigned char* object = static_cast<unsigned char*>(self);
    *reinterpret_cast<unsigned*>(object + 72) = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(vt_Ulthork));
    creature_destructor(object, 0);
    if ((flags & 1) != 0) blockFree__FPv(object);
}
