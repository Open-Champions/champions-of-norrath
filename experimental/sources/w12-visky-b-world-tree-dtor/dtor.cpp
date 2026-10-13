// Observed: _$_11VIWorldTree at 0x011001c8 is 108 bytes. With (this, flags) it calls
// Clear__11VIWorldTree(this), then _$_t7VIArray1Z11VIWorldLeaf(this + 8, 2), then
// _$_t7VIArray1Z11VIWorldNode(this, 2), and then, only if bit 0 of flags is set, tail-calls
// __builtin_delete(this). The constant 2 is passed to each member destructor regardless of the
// flags argument. Nothing is returned.
extern "C" {
void Clear__11VIWorldTree(void* self);
void _$_t7VIArray1Z11VIWorldLeaf(void* array, int flags);
void _$_t7VIArray1Z11VIWorldNode(void* array, int flags);
void __builtin_delete(void* pointer);

void _$_11VIWorldTree(void* self, int flags) {
    Clear__11VIWorldTree(self);
    _$_t7VIArray1Z11VIWorldLeaf(static_cast<char*>(self) + 8, 2);
    _$_t7VIArray1Z11VIWorldNode(self, 2);
    if (flags & 1) __builtin_delete(self);
}
}
