// Observed _$_7VIWorld at 0x010fe400, 116 bytes. With the world as this and flags as the second
// argument: _$_t7VIArray1Z13VIWorldRegion(this + 80, 2); _$_t7VIArray1Z6VIZone(this + 72, 2);
// _$_11VIWorldTree(this + 52, 2); then, only if bit 0 of flags is set, __builtin_delete(this) as a
// tail call. Nothing is returned. The constant 2 is passed to each member destructor regardless
// of the flags argument.
extern "C" {
void _$_t7VIArray1Z13VIWorldRegion(void* array, int flags);
void _$_t7VIArray1Z6VIZone(void* array, int flags);
void _$_11VIWorldTree(void* tree, int flags);
void __builtin_delete(void* pointer);

void _$_7VIWorld(void* self, int flags) {
    char* base = static_cast<char*>(self);
    _$_t7VIArray1Z13VIWorldRegion(base + 80, 2);
    _$_t7VIArray1Z6VIZone(base + 72, 2);
    _$_11VIWorldTree(base + 52, 2);
    if (flags & 1) __builtin_delete(self);
}
}
