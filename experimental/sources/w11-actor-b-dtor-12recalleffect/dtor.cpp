// Observed _$_12RecallEffect at 0x003104f8, 100 bytes (weak). Stores the 12RecallEffect table pointer (_vt$12RecallEffect) at object+72, calls the GameObject destructor (_$_10GameObject) with flag 0.
// Bit 0 of the incoming flags decides the final blockFree__FPv call (a tail call in the image); other
// flag bits are ignored. Inferred: this is the RecallEffect destructor, from its symbol name only.
// Nothing here ran on the EE.
extern "C" {
extern unsigned char vt_12RecallEffect[] __asm__("_vt$12RecallEffect");
void game_object_destructor(void* self, int flags) __asm__("_$_10GameObject");
void blockFree__FPv(void* block);
}

extern "C" void destructor_RecallEffect(void* self, int flags) __asm__("_$_12RecallEffect");
extern "C" void destructor_RecallEffect(void* self, int flags) {
    unsigned char* object = static_cast<unsigned char*>(self);
    *reinterpret_cast<unsigned*>(object + 72) = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(vt_12RecallEffect));
    game_object_destructor(object, 0);
    if ((flags & 1) != 0) blockFree__FPv(object);
}
