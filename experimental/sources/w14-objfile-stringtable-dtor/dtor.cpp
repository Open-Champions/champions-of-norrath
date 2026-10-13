// Observed: _$_13VIStringTable at 0x0101bb90, 84 bytes. With (this, flags) it calls Unload__13VIStringTable(this), then when bit 0
// of flags is set tail-calls __builtin_delete(this). Nothing is returned.
// Inferred: bit 0 of flags is the delete-after flag, as in the other destructors of this family.
// Unknown: what Unload does beyond the symbol name.
extern "C" {
void Unload__13VIStringTable(void* self);
void __builtin_delete(void* pointer);

void _$_13VIStringTable(void* self, int flags) {
    Unload__13VIStringTable(self);
    if ((flags & 1) != 0) __builtin_delete(self);
}
}
