// Observed: _$_16VIParticleSystem at 0x010b8e38 is 116 bytes and returns nothing.
// It calls _$_t6VIPool1ZP20VIParticleDefinition with flags 2 on offset 84, then
// _$_7VILList with flags 2 on offset 64 and on offset 52, then, if bit 0 of the
// second argument is set, tail-calls __builtin_delete(this).
extern "C" {
void _$_t6VIPool1ZP20VIParticleDefinition(void* pool, int flags);
void _$_7VILList(void* list, int flags);
void __builtin_delete(void* pointer);

void _$_16VIParticleSystem(void* object, int flags) {
    unsigned char* bytes = static_cast<unsigned char*>(object);
    _$_t6VIPool1ZP20VIParticleDefinition(bytes + 84, 2);
    _$_7VILList(bytes + 64, 2);
    _$_7VILList(bytes + 52, 2);
    if ((flags & 1) != 0) __builtin_delete(object);
}
}
