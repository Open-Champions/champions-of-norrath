// Observed: _$_7VIActor at 0x010659a8 is 84 bytes in the pinned SLUS-20565 image. With (this,
// flags) it calls _$_10VIMatrix44(this, 2), and when bit 0 of flags is set tail-calls
// __builtin_delete(this). Nothing is returned.
// Unknown: the remaining members of VIActor; only the matrix at the start of the object is
// destroyed here. Nothing here ran on the EE.
extern "C" {
void _$_10VIMatrix44(void* self, int flags);
void __builtin_delete(void* pointer);

void fn_7VIActor(void* self, int flags) __asm__("_$_7VIActor");
void fn_7VIActor(void* self, int flags) {
    _$_10VIMatrix44(self, 2);
    if ((flags & 1) != 0) __builtin_delete(self);
}
}
