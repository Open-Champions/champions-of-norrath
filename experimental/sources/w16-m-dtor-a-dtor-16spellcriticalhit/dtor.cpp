// Observed _$_16SpellCriticalHit at 0x00318f00, 124 bytes (weak symbol in the private executable).
// In order it stores the table _vt$16SpellCriticalHit at object+20, calls the Light destructor _$_5Light on object+28 with flag 2, stores the table _vt$5Spell at object+20, and calls listDestroy__FP8ListHead on object+4; then, when bit 0 of the incoming flags is set, it calls blockFree__FPv on the object
// (a tail call in the image). Other flag bits do not free.
// Inferred: destructor of the class named by the symbol; the table names are the observed ones, including base
// tables stored after the derived one. Unknown: the meaning of the list heads and the flag-bit uses beyond bit 0.
// Nothing here ran on the EE.
extern "C" {
extern unsigned char vt_16SpellCriticalHit[] __asm__("_vt$16SpellCriticalHit");
extern unsigned char vt_5Spell[] __asm__("_vt$5Spell");
void light_destructor(void* self, int flags) __asm__("_$_5Light");
void listDestroy__FP8ListHead(void* head);
void blockFree__FPv(void* block);
}

static void store_table(unsigned char* object, int offset, const unsigned char* table) {
    *reinterpret_cast<unsigned*>(object + offset) = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(table));
}

extern "C" void destructor_16SpellCriticalHit(void* self, int flags) __asm__("_$_16SpellCriticalHit");
extern "C" void destructor_16SpellCriticalHit(void* self, int flags) {
    unsigned char* object = static_cast<unsigned char*>(self);
    const bool release = (flags & 1) != 0;
    store_table(object, 20, vt_16SpellCriticalHit);
    light_destructor(object + 28, 2);
    store_table(object, 20, vt_5Spell);
    listDestroy__FP8ListHead(object + 4);
    if (release) blockFree__FPv(object);
}
