// Observed _$_6PhProp at 0x0030dce0, 100 bytes (weak). Stores the 4Prop table pointer (_vt$4Prop) at object+72, calls the GameObject destructor (_$_10GameObject) with flag 0.
// Bit 0 of the incoming flags decides the final blockFree__FPv call (a tail call in the image); other
// flag bits are ignored. Inferred: this is the PhProp destructor, from its symbol name only.
// Nothing here ran on the EE.
extern "C" {
extern unsigned char vt_4Prop[] __asm__("_vt$4Prop");
void game_object_destructor(void* self, int flags) __asm__("_$_10GameObject");
void blockFree__FPv(void* block);
}

extern "C" void destructor_PhProp(void* self, int flags) __asm__("_$_6PhProp");
extern "C" void destructor_PhProp(void* self, int flags) {
    unsigned char* object = static_cast<unsigned char*>(self);
    *reinterpret_cast<unsigned*>(object + 72) = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(vt_4Prop));
    game_object_destructor(object, 0);
    if ((flags & 1) != 0) blockFree__FPv(object);
}
