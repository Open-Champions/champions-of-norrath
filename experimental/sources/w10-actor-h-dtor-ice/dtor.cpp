// Observed _$_3Ice at 0x0030e068, 100 bytes (weak).
// Stores the Ice table pointer (_vt$3Ice) at object+72, calls the GameObject destructor
// (_$_10GameObject) with flag 0, then calls blockFree__FPv on the object when bit 0 of the incoming
// flags is set. Other flag bits do not free. The free is a tail call in the image.
// Inferred: this is the Ice destructor from its symbol name. Nothing here ran on the EE.
extern "C" {
extern unsigned char vt_Ice[] __asm__("_vt$3Ice");
void gameobject_destructor(void* self, int flags) __asm__("_$_10GameObject");
void blockFree__FPv(void* block);
}

extern "C" void ice_destructor(void* self, int flags) __asm__("_$_3Ice");
extern "C" void ice_destructor(void* self, int flags) {
    unsigned char* object = static_cast<unsigned char*>(self);
    *reinterpret_cast<unsigned*>(object + 72) = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(vt_Ice));
    gameobject_destructor(object, 0);
    if ((flags & 1) != 0) blockFree__FPv(object);
}
