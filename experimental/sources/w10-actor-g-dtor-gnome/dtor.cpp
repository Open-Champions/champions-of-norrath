// Observed _$_5Gnome at 0x0030f160, 108 bytes (weak).
// Stores the Gnome table pointer (_vt$5Gnome) at object+72, runs listDestroy__FP8ListHead on the
// list head embedded at object+968, calls the Creature destructor
// (_$_8Creature) with flag 0, then calls blockFree__FPv on the object when bit 0 of the incoming
// flags is set. Other flag bits do not free. The free is a tail call in the image.
// Inferred: this is the Gnome destructor from its symbol name, and +968 is a list head. Nothing here ran on the EE.
extern "C" {
extern unsigned char vt_Gnome[] __asm__("_vt$5Gnome");
void listDestroy__FP8ListHead(void* head);
void creature_destructor(void* self, int flags) __asm__("_$_8Creature");
void blockFree__FPv(void* block);
}

extern "C" void gnome_destructor(void* self, int flags) __asm__("_$_5Gnome");
extern "C" void gnome_destructor(void* self, int flags) {
    unsigned char* object = static_cast<unsigned char*>(self);
    *reinterpret_cast<unsigned*>(object + 72) = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(vt_Gnome));
    listDestroy__FP8ListHead(object + 968);
    creature_destructor(object, 0);
    if ((flags & 1) != 0) blockFree__FPv(object);
}
