// Observed _$_6Sparks at 0x00310af8, 132 bytes (weak). Stores the 6Sparks table pointer (_vt$6Sparks) at object+72, destroys the Light member at object+100 (_$_5Light with flag 2), stores the 10Projectile table pointer, calls listDestroy__FP8ListHead on object+76, then the GameObject destructor with flag 0. Inferred: Sparks derives from Projectile and embeds a Light at +100 (from the table pointer swap and the member destructor).
// Bit 0 of the incoming flags decides the final blockFree__FPv call (a tail call in the image); other
// flag bits are ignored. Inferred: this is the Sparks destructor, from its symbol name only.
// Nothing here ran on the EE.
extern "C" {
extern unsigned char vt_6Sparks[] __asm__("_vt$6Sparks");
void light_destructor(void* light, int flags) __asm__("_$_5Light");
extern unsigned char vt_10Projectile[] __asm__("_vt$10Projectile");
void game_object_destructor(void* self, int flags) __asm__("_$_10GameObject");
void listDestroy__FP8ListHead(void* head);
void blockFree__FPv(void* block);
}

extern "C" void destructor_Sparks(void* self, int flags) __asm__("_$_6Sparks");
extern "C" void destructor_Sparks(void* self, int flags) {
    unsigned char* object = static_cast<unsigned char*>(self);
    *reinterpret_cast<unsigned*>(object + 72) = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(vt_6Sparks));
    light_destructor(object + 100, 2);
    *reinterpret_cast<unsigned*>(object + 72) = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(vt_10Projectile));
    listDestroy__FP8ListHead(object + 76);
    game_object_destructor(object, 0);
    if ((flags & 1) != 0) blockFree__FPv(object);
}
