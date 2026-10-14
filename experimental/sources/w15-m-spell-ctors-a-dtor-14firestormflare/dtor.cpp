// Observed _$_14FireStormFlare at 0x003179c0, 100 bytes (weak symbol in the private executable).
// Destructor of the fire-storm flare projectile class (derived from GameObject). In order it stores the table
// _vt$14FireStormFlare at object+72, calls the GameObject base destructor on the object with a flags value of
// zero, and then, when bit 0 of the incoming flags is set, calls blockFree__FPv on the object (a tail call in the
// image). Other flag bits do not free.
// Inferred: a derived destructor of the projectile family with the same shape as the fire-bomblet destructor.
// Unknown: the layout of the object from +0 to +71 and the owner of the flag convention beyond bit 0.
// Nothing here ran on the EE.
extern "C" {
extern unsigned char vt_FireStormFlare[] __asm__("_vt$14FireStormFlare");
void destructor_GameObject(void* self, int flags) __asm__("_$_10GameObject");
void blockFree__FPv(void* block);
}

extern "C" void destructor_FireStormFlare(void* self, int flags) __asm__("_$_14FireStormFlare");
extern "C" void destructor_FireStormFlare(void* self, int flags) {
    unsigned char* object = static_cast<unsigned char*>(self);
    *reinterpret_cast<unsigned*>(object + 72) = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(vt_FireStormFlare));
    destructor_GameObject(object, 0);
    if ((flags & 1) != 0) blockFree__FPv(object);
}
