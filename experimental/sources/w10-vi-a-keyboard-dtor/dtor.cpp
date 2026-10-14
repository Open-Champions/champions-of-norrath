// Observed: _$_10VIKeyboard at 0x002cbf00 is 44 bytes and returns nothing. It does no
// member cleanup; if bit 0 of the second argument is set it tail-calls
// __builtin_delete(this), otherwise it returns.
// Unknown: whether VIKeyboard has members that a fuller destructor would release.
extern "C" {
void __builtin_delete(void* pointer);

void _$_10VIKeyboard(void* object, int flags) {
    if ((flags & 1) != 0) __builtin_delete(object);
}
}
