// Deleting destructor of VIParticleAttributes (image label _$_20VIParticleAttributes).
//
// Observed in the private image at 0x010b4550 (44 bytes): it runs no member or base
// destructor and writes no field. When bit 0 of the incoming flags is set it tail-calls the
// runtime delete (the image's __builtin_delete) on the object; otherwise it returns.
// Inferred: the class has no destructor-time work of its own, and its storage is allocated
// with the runtime new rather than the block allocator, which is why it is released with the
// runtime delete instead of blockFree.
// Unknown: whether the class has other destructor-time work that the image places in a
// different routine.
#include "../w13-m-spell-dtor-a-common/dtor_common.hpp"

extern "C" void destructor_VIParticleAttributes(void* self, int flags) __asm__("_$_20VIParticleAttributes");
extern "C" void destructor_VIParticleAttributes(void* self, int flags) {
    if ((flags & 1) != 0) runtimeDelete(self);
}
