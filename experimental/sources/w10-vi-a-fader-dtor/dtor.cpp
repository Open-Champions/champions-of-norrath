// Observed: _$_7VIFader at 0x0117cc30 is 84 bytes and returns nothing. It
// calls Free__7VIFader(this) first. Bit 0 of the second argument then selects
// __builtin_delete(this); the other bits are ignored.
extern "C" {
void Free__7VIFader(void* object);
void __builtin_delete(void* pointer);

void _$_7VIFader(void* object, int flags) {
    Free__7VIFader(object);
    if ((flags & 1) != 0) __builtin_delete(object);
}
}
