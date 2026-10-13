// Observed _$_11MaleDarkElf at 0x003168d0, 100 bytes (weak).
// Stores the MaleDarkElf table pointer (the .data object at 0x450440, _vt$11MaleDarkElf) at object+72, calls the Creature destructor (_$_8Creature) on the
// object with flag 0, then calls blockFree__FPv on the object when bit 0 of the incoming flags is set.
// The free is a tail call in the image.
// Inferred: this is the MaleDarkElf destructor from its symbol name. Nothing here ran on the EE.
extern "C" {
extern unsigned char norrath_ctors_lava_wraith_view_450440[];
void creature_destructor(void* self, int flags) __asm__("_$_8Creature");
void blockFree__FPv(void* block);
}

extern "C" void male_dark_elf_destructor(void* self, int flags) __asm__("_$_11MaleDarkElf");
extern "C" void male_dark_elf_destructor(void* self, int flags) {
    unsigned char* object = static_cast<unsigned char*>(self);
    *reinterpret_cast<unsigned*>(object + 72) = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(norrath_ctors_lava_wraith_view_450440));
    creature_destructor(object, 0);
    if ((flags & 1) != 0) blockFree__FPv(object);
}
