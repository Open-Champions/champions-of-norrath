// Observed _$_11FireBomblet at 0x0028b7a0, 136 bytes. Stores the 11FireBomblet table pointer (_vt$11FireBomblet at
// 0x450620) at object+72, calls lightDeactivate__FP5Light on the Light member at object+88, then the Light
// destructor (_$_5Light) on it with flag 2, then the GameObject destructor (_$_10GameObject) on the object with
// flag 0, and calls blockFree__FPv on the object when bit 0 of the incoming flags is set (a tail call in the
// image). Other flag bits do not free.
// Inferred: this is the 11FireBomblet destructor from its symbol name, and the member at +88 is a Light from the
// callees' names. Nothing here ran on the EE.
extern "C" {
extern unsigned char vt_FireBomblet[] __asm__("_vt$11FireBomblet");
void lightDeactivate__FP5Light(void* light);
void light_destructor(void* light, int flags) __asm__("_$_5Light");
void gameobject_destructor(void* self, int flags) __asm__("_$_10GameObject");
void blockFree__FPv(void* block);
}

extern "C" void destructor_FireBomblet(void* self, int flags) __asm__("_$_11FireBomblet");
extern "C" void destructor_FireBomblet(void* self, int flags) {
    unsigned char* object = static_cast<unsigned char*>(self);
    *reinterpret_cast<unsigned*>(object + 72) = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(vt_FireBomblet));
    lightDeactivate__FP5Light(object + 88);
    light_destructor(object + 88, 2);
    gameobject_destructor(object, 0);
    if ((flags & 1) != 0) blockFree__FPv(object);
}
