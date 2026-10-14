// Observed _$_5Torch at 0x0030fef0, 116 bytes (weak). Stores the 5Torch table pointer
// (_vt$5Torch) at object+72, calls the Light destructor (_$_5Light) with flag 2 on the member at
// object+76, calls the GameObject destructor (_$_10GameObject) with flag 0 on the object, then
// calls blockFree__FPv on the object when bit 0 of the incoming flags is set. Other flag bits do
// not free. The free is a tail call in the image.
// Inferred: this is the 5Torch destructor from its symbol name. Nothing here ran on the EE.
extern "C" {
extern unsigned char vt_5Torch[] __asm__("_vt$5Torch");
void light_destructor(void* light, int flags) __asm__("_$_5Light");
void gameobject_destructor(void* self, int flags) __asm__("_$_10GameObject");
void blockFree__FPv(void* block);
}

extern "C" void destructor_5Torch(void* self, int flags) __asm__("_$_5Torch");
extern "C" void destructor_5Torch(void* self, int flags) {
    unsigned char* object = static_cast<unsigned char*>(self);
    *reinterpret_cast<unsigned*>(object + 72) = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(vt_5Torch));
    light_destructor(object + 76, 2);
    gameobject_destructor(object, 0);
    if ((flags & 1) != 0) blockFree__FPv(object);
}
