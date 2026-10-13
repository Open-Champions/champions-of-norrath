// Observed: _$_t10VIMultiMap2ZfZ14VIEmitterEntry at 0x01224ff0 is 116 bytes and returns nothing. With (this, flags) it reads the word at
// this+8; when that is nonzero it calls DestroyPool__t10VIMultiMap2ZfZ14VIEmitterEntry(this), re-reads the word at this+8 and, if
// still nonzero, calls __builtin_vec_delete with it. Then when bit 0 of flags is set it tail-calls
// __builtin_delete(this).
// Inferred: the word at this+8 is the storage pointer of the container (the delete[] call on it says
// so); flags bit 0 is the delete-after flag, as in the other container destructors of this family.
// Unknown: the remaining fields of the container and what the DestroyPool routine does to the word.
extern "C" {
void DestroyPool__t10VIMultiMap2ZfZ14VIEmitterEntry(void* self);
void __builtin_vec_delete(void* pointer);
void __builtin_delete(void* pointer);

void _$_t10VIMultiMap2ZfZ14VIEmitterEntry(void* self, int flags) {
    unsigned* words = static_cast<unsigned*>(self);
    if (words[2] != 0) {
        DestroyPool__t10VIMultiMap2ZfZ14VIEmitterEntry(self);
        if (words[2] != 0) __builtin_vec_delete(reinterpret_cast<void*>(static_cast<unsigned long>(words[2])));
    }
    if ((flags & 1) != 0) __builtin_delete(self);
}
}
