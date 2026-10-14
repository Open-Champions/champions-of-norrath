// Observed _$_19BlindingLightEffect at 0x00319c68, 100 bytes (weak symbol in the private executable).
// In order it stores the table _vt$19BlindingLightEffect at object+72, and calls the GameObject destructor _$_10GameObject with flag 0; then, when bit 0 of the incoming flags is set, it calls blockFree__FPv on the object
// (a tail call in the image). Other flag bits do not free.
// Inferred: destructor of the class named by the symbol; the table names are the observed ones, including base
// tables stored after the derived one. Unknown: the meaning of the list heads and the flag-bit uses beyond bit 0.
// Nothing here ran on the EE.
extern "C" {
extern unsigned char vt_19BlindingLightEffect[] __asm__("_vt$19BlindingLightEffect");
void game_object_destructor(void* self, int flags) __asm__("_$_10GameObject");
void blockFree__FPv(void* block);
}

static void store_table(unsigned char* object, int offset, const unsigned char* table) {
    *reinterpret_cast<unsigned*>(object + offset) = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(table));
}

extern "C" void destructor_19BlindingLightEffect(void* self, int flags) __asm__("_$_19BlindingLightEffect");
extern "C" void destructor_19BlindingLightEffect(void* self, int flags) {
    unsigned char* object = static_cast<unsigned char*>(self);
    const bool release = (flags & 1) != 0;
    store_table(object, 72, vt_19BlindingLightEffect);
    game_object_destructor(object, 0);
    if (release) blockFree__FPv(object);
}
