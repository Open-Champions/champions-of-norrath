// Observed: _$_t7VIArray1Zi at 0x0120fa00 is 116 bytes and returns nothing. If the word at
// offset 4 is nonzero it calls DestroyPool__t7VIArray1Zi(this), then, if that word is still
// nonzero, passes it to __builtin_vec_delete. When bit 0 of the second argument
// is set it tail-calls __builtin_delete(this).
// Inferred: this is the destructor of the VIArray instance named by the symbol; the word at
// offset 4 is its element storage pointer. Neither callee is defined here.
// Unknown: the element type's size and the storage owner's allocation path.
extern "C" {
void DestroyPool__t7VIArray1Zi(void* object);
void __builtin_vec_delete(void* pointer);
void __builtin_delete(void* pointer);

void _$_t7VIArray1Zi(void* object, int flags) {
    void** storage = reinterpret_cast<void**>(static_cast<char*>(object) + 4);
    if (*storage != nullptr) {
        DestroyPool__t7VIArray1Zi(object);
        if (*storage != nullptr) __builtin_vec_delete(*storage);
    }
    if ((flags & 1) != 0) __builtin_delete(object);
}
}
