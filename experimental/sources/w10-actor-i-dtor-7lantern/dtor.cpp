// Observed _$_7Lantern at 0x003102a8, 116 bytes (weak).
// Stores the Lantern table pointer (the .data object at 0x44a8a0, _vt$7Lantern) at object+72, calls the
// Light destructor on the member at object+76 with flag 2, calls the GameObject destructor
// (_$_10GameObject) on the object with flag 0, then calls blockFree__FPv on the object when bit 0 of
// the incoming flags is set. The free is a tail call in the image.
// Inferred: this is the Lantern destructor from its symbol name. Nothing here ran on the EE.
extern "C" {
extern unsigned char norrath_ctors_ant_lantern_view_44a8a0[];
void light_destructor(void* self, int flags) __asm__("_$_5Light");
void gameobject_destructor(void* self, int flags) __asm__("_$_10GameObject");
void blockFree__FPv(void* block);
}

extern "C" void lantern_destructor(void* self, int flags) __asm__("_$_7Lantern");
extern "C" void lantern_destructor(void* self, int flags) {
    unsigned char* object = static_cast<unsigned char*>(self);
    *reinterpret_cast<unsigned*>(object + 72) = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(norrath_ctors_ant_lantern_view_44a8a0));
    light_destructor(object + 76, 2);
    gameobject_destructor(object, 0);
    if ((flags & 1) != 0) blockFree__FPv(object);
}
