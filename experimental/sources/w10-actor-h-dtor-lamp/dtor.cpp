// Observed _$_4Lamp at 0x00310370, 116 bytes (weak).
// Stores the Lamp table pointer (_vt$4Lamp) at object+72, calls the Light destructor on the member
// at object+76 with flag 2, calls the GameObject destructor (_$_10GameObject) on the object with flag 0,
// then calls blockFree__FPv on the object when bit 0 of the incoming flags is set. The free is a tail
// call in the image.
// Inferred: this is the Lamp destructor from its symbol name. Nothing here ran on the EE.
extern "C" {
extern unsigned char vt_Lamp[] __asm__("_vt$4Lamp");
void light_destructor(void* self, int flags) __asm__("_$_5Light");
void gameobject_destructor(void* self, int flags) __asm__("_$_10GameObject");
void blockFree__FPv(void* block);
}

extern "C" void lamp_destructor(void* self, int flags) __asm__("_$_4Lamp");
extern "C" void lamp_destructor(void* self, int flags) {
    unsigned char* object = static_cast<unsigned char*>(self);
    *reinterpret_cast<unsigned*>(object + 72) = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(vt_Lamp));
    light_destructor(object + 76, 2);
    gameobject_destructor(object, 0);
    if ((flags & 1) != 0) blockFree__FPv(object);
}
