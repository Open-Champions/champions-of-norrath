// Observed: _$_t6VIPool1ZP16VIStaticLighting at 0x01224620 is 116 bytes and returns nothing. With (this, flags)
// it reads the word at this+8; when that is nonzero it calls DestroyPool__t6VIPool1ZP16VIStaticLighting(this), re-reads
// the word at this+8 and, if still nonzero, calls __builtin_vec_delete with it. Then when bit 0 of
// flags is set it tail-calls __builtin_delete(this).
// Inferred: the word at this+8 is the storage pointer of the container (the compiler's
// delete[] call on it says so); flags bit 0 is the delete-after flag.
// Unknown: the remaining fields of the container and what DestroyPool__t6VIPool1ZP16VIStaticLighting does to the word.
struct Container {
    int words_before[2];
    void* storage;
};

extern "C" {
void DestroyPool__t6VIPool1ZP16VIStaticLighting(Container* self);
void __builtin_vec_delete(void* pointer);
void __builtin_delete(void* pointer);

void _$_t6VIPool1ZP16VIStaticLighting(Container* self, int flags) {
    if (self->storage != 0) {
        DestroyPool__t6VIPool1ZP16VIStaticLighting(self);
        if (self->storage != 0) __builtin_vec_delete(self->storage);
    }
    if ((flags & 1) != 0) __builtin_delete(self);
}
}
