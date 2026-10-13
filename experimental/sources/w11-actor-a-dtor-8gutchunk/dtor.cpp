// Observed _$_8GutChunk at 0x0030fba8, 100 bytes (weak). Stores the table pointer _vt$4Prop (0x44d018, the table
// of the base class Prop, not a 8GutChunk table) at object+72, calls the GameObject destructor
// (_$_10GameObject) on the object with flag 0, and calls blockFree__FPv on the object when bit 0 of the
// incoming flags is set (a tail call in the image). Other flag bits do not free.
// Inferred: this is the 8GutChunk destructor from its symbol name; that the table stored is a base class's is
// read from the symbol name of the address alone. Nothing here ran on the EE.
extern "C" {
extern unsigned char vt_Prop[] __asm__("_vt$4Prop");
void gameobject_destructor(void* self, int flags) __asm__("_$_10GameObject");
void blockFree__FPv(void* block);
}

extern "C" void destructor_GutChunk(void* self, int flags) __asm__("_$_8GutChunk");
extern "C" void destructor_GutChunk(void* self, int flags) {
    unsigned char* object = static_cast<unsigned char*>(self);
    *reinterpret_cast<unsigned*>(object + 72) = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(vt_Prop));
    gameobject_destructor(object, 0);
    if ((flags & 1) != 0) blockFree__FPv(object);
}
