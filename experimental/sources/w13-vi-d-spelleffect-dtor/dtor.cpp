// Observed: _$_13VISpellEffect at 0x010007b8 is 108 bytes in the pinned SLUS-20565 image and returns nothing. With (this, flags) it
// calls Destroy__13VISpellEffect(this), then Destroy__16VISpellEventList on this+36, on this+24 and on this+12 in that order, and
// then, when bit 0 of flags is set, tail-calls __builtin_delete(this).
// Inferred: three VISpellEventList members sit at +12, +24 and +36 and are destroyed in reverse order of declaration; flags bit 0
// is the delete-after flag as in the other destructors of the engine.
// Unknown: the remaining fields and the callee bodies (all imports). Nothing here ran on the EE.
extern "C" {
void Destroy__13VISpellEffect(void* self);
void Destroy__16VISpellEventList(void* list);
void __builtin_delete(void* pointer);

void _$_13VISpellEffect(void* self, int flags) {
    unsigned char* bytes = static_cast<unsigned char*>(self);
    Destroy__13VISpellEffect(self);
    Destroy__16VISpellEventList(bytes + 36);
    Destroy__16VISpellEventList(bytes + 24);
    Destroy__16VISpellEventList(bytes + 12);
    if ((flags & 1) != 0) __builtin_delete(self);
}
}
