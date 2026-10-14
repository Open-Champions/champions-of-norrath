// Observed: _$_10VIZoneRoom at 0x0111f338, 108 bytes, returns nothing. With (this, flags) it calls
// Clear__10VIZoneRoom(this), then _$_t7VIArray1Zi(this+68, 2), then _$_t7VIArray1Zi(this+60, 2). When bit 0 of
// flags is set it then tail-calls __builtin_delete(this).
// Inferred: the destructor of the zone room: the two arrays at +60 and +68 are members that are destroyed
// in reverse order of declaration (+68 first), and flags bit 0 is the delete-after flag;// argument 2 to the member destructors is the compiler's not-in-charge-of-delete value.
// Unknown: what the two arrays hold (their element type is int by the symbol name only).
extern "C" {
int Clear__10VIZoneRoom(unsigned char* room);
void _$_t7VIArray1Zi(void* array, int flags);
void __builtin_delete(void* pointer);

void _$_10VIZoneRoom(unsigned char* self, int flags) {
    Clear__10VIZoneRoom(self);
    _$_t7VIArray1Zi(self + 68, 2);
    _$_t7VIArray1Zi(self + 60, 2);
    if ((flags & 1) != 0) __builtin_delete(self);
}
}
