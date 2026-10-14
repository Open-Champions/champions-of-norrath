// Observed _$_22SpellPlanarSteelEffect at 0x002c9988, 144 bytes (strong).
// In order it stores the table _vt$22SpellPlanarSteelEffect at object+20, calls lightDeactivate__FP5Light on object+48, calls the Light destructor (_$_5Light) on object+48 with flag 2, stores the table _vt$5Spell at object+20, then calls listDestroy__FP8ListHead on object+4.
// Finally, when bit 0 of the incoming flags is set, it calls blockFree__FPv on the object (a tail call in the image).
// Other flag bits do not free. Inferred: this is the destructor of the class from its symbol name; the table name stored
// is the observed one even where it is a base class's. Nothing here ran on the EE.
extern "C" {
extern unsigned char vt_22SpellPlanarSteelEffect[] __asm__("_vt$22SpellPlanarSteelEffect");
extern unsigned char vt_5Spell[] __asm__("_vt$5Spell");
void light_destructor(void* self, int flags) __asm__("_$_5Light");
void lightDeactivate__FP5Light(void* light);
void listDestroy__FP8ListHead(void* head);
void blockFree__FPv(void* block);
}

static void store_table(unsigned char* object, int offset, const unsigned char* table) {
    *reinterpret_cast<unsigned*>(object + offset) = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(table));
}

extern "C" void destructor_22SpellPlanarSteelEffect(void* self, int flags) __asm__("_$_22SpellPlanarSteelEffect");
extern "C" void destructor_22SpellPlanarSteelEffect(void* self, int flags) {
    unsigned char* object = static_cast<unsigned char*>(self);
    store_table(object, 20, vt_22SpellPlanarSteelEffect);
    lightDeactivate__FP5Light(object + 48);
    light_destructor(object + 48, 2);
    store_table(object, 20, vt_5Spell);
    listDestroy__FP8ListHead(object + 4);
    if ((flags & 1) != 0) blockFree__FPv(object);
}
