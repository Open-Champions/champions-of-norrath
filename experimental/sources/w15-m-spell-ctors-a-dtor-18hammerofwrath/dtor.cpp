// Observed _$_18SpellHammerOfWrath at 0x00319768, 116 bytes (weak symbol in the private executable).
// Destructor of the hammer-of-wrath spell class. In order it stores _vt$18SpellHammerOfWrath at object+20,
// calls listDestroy__FP8ListHead on object+24, stores the base table _vt$5Spell at object+20, calls
// listDestroy__FP8ListHead on object+4, and then, when bit 0 of the incoming flags is set, calls blockFree__FPv
// on the object (a tail call in the image). Other flag bits do not free.
// Inferred: a derived destructor of the spell family that owns a second list head at +24 beyond the base
// Spell layout. Unknown: the element type of the list at +24 and the reason for the base-table store between the
// two list calls. Nothing here ran on the EE.
extern "C" {
extern unsigned char vt_HammerOfWrath[] __asm__("_vt$18SpellHammerOfWrath");
extern unsigned char vt_Spell[] __asm__("_vt$5Spell");
void listDestroy__FP8ListHead(void* head);
void blockFree__FPv(void* block);
}

extern "C" void destructor_HammerOfWrath(void* self, int flags) __asm__("_$_18SpellHammerOfWrath");
extern "C" void destructor_HammerOfWrath(void* self, int flags) {
    unsigned char* object = static_cast<unsigned char*>(self);
    const int freeBlock = flags & 1;
    *reinterpret_cast<unsigned*>(object + 20) = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(vt_HammerOfWrath));
    listDestroy__FP8ListHead(object + 24);
    *reinterpret_cast<unsigned*>(object + 20) = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(vt_Spell));
    listDestroy__FP8ListHead(object + 4);
    if (freeBlock != 0) blockFree__FPv(object);
}
