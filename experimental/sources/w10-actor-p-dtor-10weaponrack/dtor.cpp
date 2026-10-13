// Observed _$_10WeaponRack at 0x00312658, 100 bytes (weak). Stores the 10WeaponRack table pointer
// (_vt$10WeaponRack) at object+72, calls the GameObject destructor (_$_10GameObject) with
// flag 0, then calls blockFree__FPv on the object when bit 0 of the incoming flags is set. Other
// flag bits do not free. The free is a tail call in the image.
// Inferred: this is the 10WeaponRack destructor from its symbol name. Nothing here ran on the EE.
extern "C" {
extern unsigned char vt_10WeaponRack[] __asm__("_vt$10WeaponRack");
void base_destructor(void* self, int flags) __asm__("_$_10GameObject");

void blockFree__FPv(void* block);
}

extern "C" void destructor_10WeaponRack(void* self, int flags) __asm__("_$_10WeaponRack");
extern "C" void destructor_10WeaponRack(void* self, int flags) {
    unsigned char* object = static_cast<unsigned char*>(self);
    *reinterpret_cast<unsigned*>(object + 72) = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(vt_10WeaponRack));
    base_destructor(object, 0);
    if ((flags & 1) != 0) blockFree__FPv(object);
}
