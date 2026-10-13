// Observed: _$_t8VIVector1Z18VISceneDisplayElem at 0x012246a8 is 116 bytes and returns nothing. If the word at
// offset 8 is nonzero it calls DestroyPool__t8VIVector1Z18VISceneDisplayElem(this), then, if that word is still
// nonzero, passes it to __builtin_vec_delete. When bit 0 of the second argument
// is set it tail-calls __builtin_delete(this).
// Inferred: this is the destructor of the VIVector instance named by the symbol; the word at
// offset 8 is its element storage pointer. Neither callee is defined here.
// Unknown: the element type's size and the storage owner's allocation path.
extern "C" {
void DestroyPool__t8VIVector1Z18VISceneDisplayElem(void* object);
void __builtin_vec_delete(void* pointer);
void __builtin_delete(void* pointer);

void _$_t8VIVector1Z18VISceneDisplayElem(void* object, int flags) {
    void** storage = reinterpret_cast<void**>(static_cast<char*>(object) + 8);
    if (*storage != nullptr) {
        DestroyPool__t8VIVector1Z18VISceneDisplayElem(object);
        if (*storage != nullptr) __builtin_vec_delete(*storage);
    }
    if ((flags & 1) != 0) __builtin_delete(object);
}
}
