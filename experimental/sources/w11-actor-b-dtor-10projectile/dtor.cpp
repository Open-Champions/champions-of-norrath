// Observed _$_10Projectile at 0x0030d658, 108 bytes (weak). Stores the 10Projectile table pointer (_vt$10Projectile) at object+72, calls listDestroy__FP8ListHead on object+76, then the GameObject destructor with flag 0.
// Bit 0 of the incoming flags decides the final blockFree__FPv call (a tail call in the image); other
// flag bits are ignored. Inferred: this is the Projectile destructor, from its symbol name only.
// Nothing here ran on the EE.
extern "C" {
extern unsigned char vt_10Projectile[] __asm__("_vt$10Projectile");
void game_object_destructor(void* self, int flags) __asm__("_$_10GameObject");
void listDestroy__FP8ListHead(void* head);
void blockFree__FPv(void* block);
}

extern "C" void destructor_Projectile(void* self, int flags) __asm__("_$_10Projectile");
extern "C" void destructor_Projectile(void* self, int flags) {
    unsigned char* object = static_cast<unsigned char*>(self);
    *reinterpret_cast<unsigned*>(object + 72) = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(vt_10Projectile));
    listDestroy__FP8ListHead(object + 76);
    game_object_destructor(object, 0);
    if ((flags & 1) != 0) blockFree__FPv(object);
}
