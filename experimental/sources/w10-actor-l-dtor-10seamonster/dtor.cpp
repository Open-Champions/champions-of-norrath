// Observed _$_10SeaMonster at 0x00314570, 100 bytes (weak). Stores the 10SeaMonster table pointer
// (the data object at 0x44e2b0, _vt$10SeaMonster) at object+72, then calls the destructor
// _$_8Creature on object+0 with flag 0; finally calls blockFree__FPv on the object when bit 0 of the
// incoming flags is set (a tail call in the image). Inferred: this is the 10SeaMonster destructor
// from its symbol name. Nothing here ran on the EE.
extern "C" {
extern unsigned char norrath_ctors_lava_wraith_view_44e2b0[];
void creature_destructor(void* self, int flags) __asm__("_$_8Creature");
void blockFree__FPv(void* block);
}

extern "C" void destructor_10seamonster(void* self, int flags) __asm__("_$_10SeaMonster");
extern "C" void destructor_10seamonster(void* self, int flags) {
    unsigned char* object = static_cast<unsigned char*>(self);
    *reinterpret_cast<unsigned*>(object + 72) = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(norrath_ctors_lava_wraith_view_44e2b0));
    creature_destructor(object, 0);
    if ((flags & 1) != 0) blockFree__FPv(object);
}
