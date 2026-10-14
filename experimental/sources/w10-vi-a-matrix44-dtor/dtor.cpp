// Observed: _$_10VIMatrix44 at 0x0109ac20 is 44 bytes and returns nothing. It does no
// member cleanup; if bit 0 of the second argument is set it tail-calls
// __builtin_delete(this), otherwise it returns.
// Unknown: whether VIMatrix44 has members that a fuller destructor would release.
extern "C" {
void __builtin_delete(void* pointer);

void _$_10VIMatrix44(void* object, int flags) {
    if ((flags & 1) != 0) __builtin_delete(object);
}
}
