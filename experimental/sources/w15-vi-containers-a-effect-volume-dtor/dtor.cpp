// Observed: _$_14VIEffectVolume at 0x0114a120, 44 bytes, returns nothing. With (this, flags) it tail-calls __builtin_delete(this) when
// bit 0 of flags is set and otherwise returns. It reads and writes nothing else of the object.
// Inferred: the destructor of a class whose members need no destruction and which stores no table pointer here; flags bit 0 is the
// delete-after flag of the compiler's destructor convention, as in the other destructors of the engine.
// Unknown: the class layout.
extern "C" {
void __builtin_delete(void* pointer);

void _$_14VIEffectVolume(void* self, int flags) {
    if ((flags & 1) != 0) __builtin_delete(self);
}
}
