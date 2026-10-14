// Observed: _$_7VIRFont at 0x01135de8 is 84 bytes. It calls Clear__7VIRFont with the
// object, then, when bit 0 of the second argument is set, tail-calls
// __builtin_delete with the object. Nothing else is touched; it returns nothing.
// Unknown: whether other bits of the flags argument are ever passed.
extern "C" {
int Clear__7VIRFont(void* self);
void __builtin_delete(void* pointer);

void _$_7VIRFont(void* self, int flags) {
    Clear__7VIRFont(self);
    if (flags & 1)
        __builtin_delete(self);
}
}
