// Observed: _$_t7VIArray1Z14VIResourceElem at 0x012067d8 is 116 bytes and returns nothing. With (this, flags) it reads the word at
// this+4; when that is nonzero it calls DestroyPool__t7VIArray1Z14VIResourceElem(this), re-reads the word at this+4 and, if
// still nonzero, calls __builtin_vec_delete with it. Then when bit 0 of flags is set it tail-calls
// __builtin_delete(this).
// Inferred: the word at this+4 is the storage pointer of the container (the delete[] call on it says
// so); flags bit 0 is the delete-after flag, as in the other container destructors of this family.
// Unknown: the remaining fields of the container and what the DestroyPool routine does to the word.
extern "C" {
void DestroyPool__t7VIArray1Z14VIResourceElem(void* self);
void __builtin_vec_delete(void* pointer);
void __builtin_delete(void* pointer);

void _$_t7VIArray1Z14VIResourceElem(void* self, int flags) {
    unsigned* words = static_cast<unsigned*>(self);
    if (words[1] != 0) {
        DestroyPool__t7VIArray1Z14VIResourceElem(self);
        if (words[1] != 0) __builtin_vec_delete(reinterpret_cast<void*>(static_cast<unsigned long>(words[1])));
    }
    if ((flags & 1) != 0) __builtin_delete(self);
}
}
