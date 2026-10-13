// Observed: _$_12VICollBuffer at 0x0108b8f8, 84 bytes, returns nothing. It calls
// Clear__12VICollBuffer(this) and then, if bit 0 of the second argument is set,
// tail-calls __builtin_delete(this).
// Unknown: the member layout of the collision buffer.
extern "C" {
void Clear__12VICollBuffer(void* object);
void __builtin_delete(void* pointer);

void _$_12VICollBuffer(void* object, int flags) {
    Clear__12VICollBuffer(object);
    if ((flags & 1) != 0) __builtin_delete(object);
}
}
