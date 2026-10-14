// Observed _$_10StaticFire at 0x0028f490, 132 bytes. Stores the 10StaticFire table pointer
// (_vt$10StaticFire) at object+72. When the signed byte at object+111 is non-zero it calls
// memFree__FPv on the word at object+1188. It then calls the Light destructor (_$_5Light) with
// flag 2 on the member at object+128, calls the GameObject destructor (_$_10GameObject) with flag
// 0 on the object, and finally calls blockFree__FPv on the object when bit 0 of the incoming flags
// is set. Other flag bits do not free. The free is a tail call in the image.
// Unknown: what the byte at +111 and the block at +1188 are called. Nothing here ran on the EE.
#include "../w10-actor-n-common/access.hpp"

extern "C" {
extern unsigned char vt_10StaticFire[] __asm__("_vt$10StaticFire");
void memFree__FPv(void* block);
void light_destructor(void* light, int flags) __asm__("_$_5Light");
void gameobject_destructor(void* self, int flags) __asm__("_$_10GameObject");
void blockFree__FPv(void* block);
}

extern "C" void destructor_10StaticFire(void* self, int flags) __asm__("_$_10StaticFire");
extern "C" void destructor_10StaticFire(void* self, int flags) {
    unsigned char* object = static_cast<unsigned char*>(self);
    *reinterpret_cast<unsigned*>(object + 72) = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(vt_10StaticFire));
    if (static_cast<signed char>(object[111]) != 0) memFree__FPv(w10n::from_word(w10n::word(object, 1188)));
    light_destructor(object + 128, 2);
    gameobject_destructor(object, 0);
    if ((flags & 1) != 0) blockFree__FPv(object);
}
