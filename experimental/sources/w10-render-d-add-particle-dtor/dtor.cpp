// Observed _$_11AddParticle at 0x00315050, 116 bytes: the AddParticle destructor, taking the object and a
// flag word. In order: the AddParticle vtable (_vt$11AddParticle, 0x0044eb08) is stored at object+72; the
// Light destructor (_$_5Light, 0x0013eac8) runs on the embedded light record at object+256 with flag 2;
// the GameObject destructor (_$_10GameObject, 0x00141658) runs on the object with flag 0; and when bit 0
// of the incoming flag word is set the object is released with blockFree__FPv (0x001447d8, a tail call).
// Other flag bits do nothing. Nothing is returned.
extern "C" {
void light_destructor(void* light, int flags) __asm__("_$_5Light");
void game_object_destructor(void* object, int flags) __asm__("_$_10GameObject");
void blockFree__FPv(void* block);
extern unsigned char add_particle_vtable[] __asm__("_vt$11AddParticle");
}

extern "C" void add_particle_destructor(void* self, int flags) __asm__("_$_11AddParticle");
extern "C" void add_particle_destructor(void* self, int flags) {
    unsigned char* object = static_cast<unsigned char*>(self);
    *reinterpret_cast<unsigned*>(object + 72) = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(add_particle_vtable));
    light_destructor(object + 256, 2);
    game_object_destructor(object, 0);
    if (flags & 1) blockFree__FPv(object);
}
