// Observed _$_13FireBombletJr at 0x003121b8, 100 bytes (weak symbol in the private executable).
// Destructor of the fire-bomblet projectile class (derived from GameObject). In order it stores the table
// _vt$13FireBombletJr at object+72, calls the GameObject base destructor on the object with a flags value of
// zero, and then, when bit 0 of the incoming flags is set, calls blockFree__FPv on the object (a tail call in the
// image). Other flag bits do not free.
// Inferred: a derived destructor of the projectile family, following the shape of the spell destructors in this
// package. Unknown: the layout of the object from +0 to +71 and the owner of the flag convention beyond bit 0.
// Nothing here ran on the EE.
extern "C" {
extern unsigned char vt_FireBombletJr[] __asm__("_vt$13FireBombletJr");
void destructor_GameObject(void* self, int flags) __asm__("_$_10GameObject");
void blockFree__FPv(void* block);
}

extern "C" void destructor_FireBombletJr(void* self, int flags) __asm__("_$_13FireBombletJr");
extern "C" void destructor_FireBombletJr(void* self, int flags) {
    unsigned char* object = static_cast<unsigned char*>(self);
    *reinterpret_cast<unsigned*>(object + 72) = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(vt_FireBombletJr));
    destructor_GameObject(object, 0);
    if ((flags & 1) != 0) blockFree__FPv(object);
}
