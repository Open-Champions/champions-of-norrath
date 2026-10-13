// Observed: _$_13VIStringTable at 0x0101bb90 is 84 bytes and returns nothing. It
// calls Unload__13VIStringTable(this), then if bit 0 of the second argument is set
// tail-calls __builtin_delete(this).
extern "C" {
void Unload__13VIStringTable(void* table);
void __builtin_delete(void* pointer);

void _$_13VIStringTable(void* table, int flags) {
    Unload__13VIStringTable(table);
    if ((flags & 1) != 0) __builtin_delete(table);
}
}
