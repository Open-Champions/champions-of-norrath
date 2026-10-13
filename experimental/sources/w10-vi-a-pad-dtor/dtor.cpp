// Observed: _$_5VIPad at 0x010ff0b0 is 84 bytes and returns nothing. It calls
// Close__5VIPad(this), then if bit 0 of the second argument is set
// tail-calls __builtin_delete(this).
extern "C" {
void Close__5VIPad(void* object);
void __builtin_delete(void* pointer);

void _$_5VIPad(void* object, int flags) {
    Close__5VIPad(object);
    if ((flags & 1) != 0) __builtin_delete(object);
}
}
