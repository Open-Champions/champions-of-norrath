// Observed: _$_19VISpellEffectDriver at 0x01182bb8 is 92 bytes in the pinned SLUS-20565 image.
// With (this, flags) it calls Destroy__19VISpellEffectDriver(this), then calls
// _$_7VILList(this+72, 2), and when bit 0 of flags is set tail-calls __builtin_delete(this).
// Nothing is returned.
// Unknown: what Destroy does and the rest of the object. Nothing here ran on the EE.
extern "C" {
void Destroy__19VISpellEffectDriver(void* self);
void _$_7VILList(void* self, int flags);
void __builtin_delete(void* pointer);

void fn_19VISpellEffectDriver(void* self, int flags) __asm__("_$_19VISpellEffectDriver");
void fn_19VISpellEffectDriver(void* self, int flags) {
    unsigned char* bytes = static_cast<unsigned char*>(self);
    Destroy__19VISpellEffectDriver(self);
    _$_7VILList(bytes + 72, 2);
    if ((flags & 1) != 0) __builtin_delete(self);
}
}
