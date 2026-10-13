// Observed _$_11LavaMonster at 0x0030f2e0, 100 bytes (weak).
// Stores the LavaMonster table pointer (the .data object at 0x44b810, _vt$11LavaMonster) at object+72, calls the Creature destructor (_$_8Creature) on the
// object with flag 0, then calls blockFree__FPv on the object when bit 0 of the incoming flags is set.
// The free is a tail call in the image.
// Inferred: this is the LavaMonster destructor from its symbol name. Nothing here ran on the EE.
extern "C" {
extern unsigned char norrath_ctors_lava_wraith_view_44b810[];
void creature_destructor(void* self, int flags) __asm__("_$_8Creature");
void blockFree__FPv(void* block);
}

extern "C" void lava_monster_destructor(void* self, int flags) __asm__("_$_11LavaMonster");
extern "C" void lava_monster_destructor(void* self, int flags) {
    unsigned char* object = static_cast<unsigned char*>(self);
    *reinterpret_cast<unsigned*>(object + 72) = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(norrath_ctors_lava_wraith_view_44b810));
    creature_destructor(object, 0);
    if ((flags & 1) != 0) blockFree__FPv(object);
}
