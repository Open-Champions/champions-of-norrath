// Observed: _$_6VIFile at 0x010cae60 is 84 bytes and returns nothing. It calls
// Close__6VIFile(this), then if bit 0 of the second argument is set tail-calls
// __builtin_delete(this).
extern "C" {
void Close__6VIFile(void* file);
void __builtin_delete(void* pointer);

void _$_6VIFile(void* file, int flags) {
    Close__6VIFile(file);
    if ((flags & 1) != 0) __builtin_delete(file);
}
}
