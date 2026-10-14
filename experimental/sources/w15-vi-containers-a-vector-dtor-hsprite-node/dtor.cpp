// Observed: _$_t8VIVector1Z13VIHSpriteNode at 0x01203e58, 116 bytes, returns nothing. With (this, flags) it reads the word at
// this+8; when that is nonzero it calls DestroyPool__t8VIVector1Z13VIHSpriteNode(this), re-reads the word at this+8 and, if
// still nonzero, calls __builtin_vec_delete with it. Then when bit 0 of flags is set it tail-calls
// __builtin_delete(this).
// Inferred: the word at this+8 is the element storage pointer of the container (the delete[] call on it
// says so); flags bit 0 is the delete-after flag, as in the other container destructors of this family.
// Unknown: the remaining fields of the container and what DestroyPool does to the storage word.
extern "C" {
int DestroyPool__t8VIVector1Z13VIHSpriteNode(void* self);
void __builtin_vec_delete(void* pointer);
void __builtin_delete(void* pointer);

void _$_t8VIVector1Z13VIHSpriteNode(void* self, int flags) {
    unsigned* words = static_cast<unsigned*>(self);
    if (words[2] != 0) {
        DestroyPool__t8VIVector1Z13VIHSpriteNode(self);
        if (words[2] != 0) __builtin_vec_delete(reinterpret_cast<void*>(static_cast<unsigned long>(words[2])));
    }
    if ((flags & 1) != 0) __builtin_delete(self);
}
}
