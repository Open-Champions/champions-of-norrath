// Observed: _$_t6VIPool1ZP20VIParticleDefinition at 0x01208f28, 116 bytes; returns nothing. Arguments are the object and a flags word.
//  1. When the word at +8 is nonzero: call DestroyPool__t6VIPool1ZP20VIParticleDefinition(object); then if that word is still nonzero pass it to __builtin_vec_delete.
//  2. When bit 0 of the flags is set, tail-call __builtin_delete(object).
// Inferred: the word at +8 is the element storage of this instance and bit 0 means free the object afterwards.
// The template argument comes only from the mangled name.
extern "C" {
void DestroyPool__t6VIPool1ZP20VIParticleDefinition(void* object);
void __builtin_vec_delete(void* pointer);
void __builtin_delete(void* pointer);

void _$_t6VIPool1ZP20VIParticleDefinition(void* object, int flags) {
    unsigned* words = static_cast<unsigned*>(object);
    if (words[2] != 0) {
        DestroyPool__t6VIPool1ZP20VIParticleDefinition(object);
        if (words[2] != 0) __builtin_vec_delete(reinterpret_cast<void*>(static_cast<unsigned long>(words[2])));
    }
    if ((flags & 1) != 0) __builtin_delete(object);
}
}
