// Observed _$_19SpellHornOmenEffect at 0x002ca358, 116 bytes (strong).
// In order it stores the table _vt$19SpellHornOmenEffect at object+20, calls P_DeleteParticleSystem__Fi with the int at object+28, stores the table _vt$5Spell at object+20, then calls listDestroy__FP8ListHead on object+4.
// Finally, when bit 0 of the incoming flags is set, it calls blockFree__FPv on the object (a tail call in the image).
// Other flag bits do not free. Inferred: this is the destructor of the class from its symbol name; the table name stored
// is the observed one even where it is a base class's. Nothing here ran on the EE.
extern "C" {
extern unsigned char vt_19SpellHornOmenEffect[] __asm__("_vt$19SpellHornOmenEffect");
extern unsigned char vt_5Spell[] __asm__("_vt$5Spell");
void listDestroy__FP8ListHead(void* head);
void P_DeleteParticleSystem__Fi(int id);
void blockFree__FPv(void* block);
}

static void store_table(unsigned char* object, int offset, const unsigned char* table) {
    *reinterpret_cast<unsigned*>(object + offset) = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(table));
}

extern "C" void destructor_19SpellHornOmenEffect(void* self, int flags) __asm__("_$_19SpellHornOmenEffect");
extern "C" void destructor_19SpellHornOmenEffect(void* self, int flags) {
    unsigned char* object = static_cast<unsigned char*>(self);
    store_table(object, 20, vt_19SpellHornOmenEffect);
    P_DeleteParticleSystem__Fi(*reinterpret_cast<int*>(object + 28));
    store_table(object, 20, vt_5Spell);
    listDestroy__FP8ListHead(object + 4);
    if ((flags & 1) != 0) blockFree__FPv(object);
}
