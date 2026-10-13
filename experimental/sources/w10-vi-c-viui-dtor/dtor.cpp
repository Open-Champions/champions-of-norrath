// Observed: _$_4VIUI at 0x0102f210 is 116 bytes. With (this, flags) it runs _$_13VIStringTable
// with second argument 2 on this+32, this+16 and this, in that order, and when bit 0 of flags is
// set tail-calls __builtin_delete(this). Nothing is returned. The font member at this+56 gets
// no call here.
// Unknown: whether __6VIFont has a destructor that the original class simply does not need.
extern "C" {
void _$_13VIStringTable(void* self, int flags);
void __builtin_delete(void* pointer);

void dtor_4VIUI(void* self, int flags) asm("_$_4VIUI");
void dtor_4VIUI(void* self, int flags) {
    unsigned char* bytes = static_cast<unsigned char*>(self);
    _$_13VIStringTable(bytes + 32, 2);
    _$_13VIStringTable(bytes + 16, 2);
    _$_13VIStringTable(self, 2);
    if ((flags & 1) != 0) __builtin_delete(self);
}
}
