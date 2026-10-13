// Observed: _$_19VIFlatFileStructure at 0x01005070, 84 bytes. With (this, flags) it calls _$_19VIFlatFileDirectory(this, 2)
// (the base-class destructor with the constant 2 as its flag word), then when bit 0 of flags is set tail-calls
// __builtin_delete(this). Nothing is returned.
// Inferred: VIFlatFileStructure derives from VIFlatFileDirectory (the constructor installs that class's table address
// and the destructor calls its destructor); bit 0 of flags is the delete-after flag.
// Unknown: whether the original class has members of its own to destroy (none are touched here).
extern "C" {
void _$_19VIFlatFileDirectory(void* self, int flags);
void __builtin_delete(void* pointer);

void _$_19VIFlatFileStructure(void* self, int flags) {
    _$_19VIFlatFileDirectory(self, 2);
    if ((flags & 1) != 0) __builtin_delete(self);
}
}
