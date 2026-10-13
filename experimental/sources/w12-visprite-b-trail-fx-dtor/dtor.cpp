// Observed: _$_9VITrailFx at 0x01027278 is 108 bytes in the pinned SLUS-20565 image. With (this,
// flags) it calls Clear__9VITrailFx(this), then calls _$_7VILList(this+40, 2), then calls
// _$_7VILList(this+28, 2), and when bit 0 of flags is set tail-calls __builtin_delete(this).
// Nothing is returned.
// Inferred: the second argument 2 given to the list destructors is the compiler convention for
// "destroy members, do not delete". Nothing here ran on the EE.
extern "C" {
void Clear__9VITrailFx(void* self);
void _$_7VILList(void* self, int flags);
void __builtin_delete(void* pointer);

void fn_9VITrailFx(void* self, int flags) __asm__("_$_9VITrailFx");
void fn_9VITrailFx(void* self, int flags) {
    unsigned char* bytes = static_cast<unsigned char*>(self);
    Clear__9VITrailFx(self);
    _$_7VILList(bytes + 40, 2);
    _$_7VILList(bytes + 28, 2);
    if ((flags & 1) != 0) __builtin_delete(self);
}
}
