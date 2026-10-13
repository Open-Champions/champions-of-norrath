// Observed: _$_19VIFlatFileStructure at 0x01005070 is 84 bytes and returns nothing.
// It calls _$_19VIFlatFileDirectory(this, 2), then if bit 0 of the second argument
// is set tail-calls __builtin_delete(this).
extern "C" {
void _$_19VIFlatFileDirectory(void* object, int flags);
void __builtin_delete(void* pointer);

void _$_19VIFlatFileStructure(void* object, int flags) {
    _$_19VIFlatFileDirectory(object, 2);
    if ((flags & 1) != 0) __builtin_delete(object);
}
}
