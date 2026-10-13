// Observed _$_19VISpellEffectSystem at 0x01002a00, 116 bytes. With the object as this and flags
// as the second argument: _$_t6VIPool1ZP13VISpellEffect(this + 1112, 2);
// _$_7VILList(this + 112, 2); _$_7VILList(this + 100, 2); then, only if bit 0 of flags is set,
// __builtin_delete(this) as a tail call. Nothing is returned. The constant 2 is passed to each
// member destructor regardless of the flags argument.
extern "C" {
void _$_t6VIPool1ZP13VISpellEffect(void* pool, int flags);
void _$_7VILList(void* list, int flags);
void __builtin_delete(void* pointer);

void _$_19VISpellEffectSystem(void* self, int flags) {
    char* base = static_cast<char*>(self);
    _$_t6VIPool1ZP13VISpellEffect(base + 1112, 2);
    _$_7VILList(base + 112, 2);
    _$_7VILList(base + 100, 2);
    if (flags & 1) __builtin_delete(self);
}
}
