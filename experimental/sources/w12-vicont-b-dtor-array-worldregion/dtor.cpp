// Observed: _$_t7VIArray1Z13VIWorldRegion at 0x0120faf0, 116 bytes; returns nothing. Arguments are the object and a flags word.
//  1. When the word at +4 is nonzero: call DestroyPool__t7VIArray1Z13VIWorldRegion(object); then if that word is still nonzero pass it to __builtin_vec_delete.
//  2. When bit 0 of the flags is set, tail-call __builtin_delete(object).
// Inferred: the word at +4 is the element storage of this instance and bit 0 means free the object afterwards.
// The template argument comes only from the mangled name.
extern "C" {
void DestroyPool__t7VIArray1Z13VIWorldRegion(void* object);
void __builtin_vec_delete(void* pointer);
void __builtin_delete(void* pointer);

void _$_t7VIArray1Z13VIWorldRegion(void* object, int flags) {
    unsigned* words = static_cast<unsigned*>(object);
    if (words[1] != 0) {
        DestroyPool__t7VIArray1Z13VIWorldRegion(object);
        if (words[1] != 0) __builtin_vec_delete(reinterpret_cast<void*>(static_cast<unsigned long>(words[1])));
    }
    if ((flags & 1) != 0) __builtin_delete(object);
}
}
