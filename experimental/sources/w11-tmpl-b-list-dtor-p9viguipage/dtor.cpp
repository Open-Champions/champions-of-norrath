// Observed: _$_t6VIList1ZP9VIGUIPage at 0x01209580 is 84 bytes and returns nothing. With (this, flags) it
// calls Clear__t6VIList1ZP9VIGUIPage(this), then when bit 0 of flags is set tail-calls __builtin_delete(this).
// Inferred: flags bit 0 is the delete-after flag of the compiler's destructor convention;
// only the mangled template name and this call shape are evidence for the container type.
extern "C" {
void Clear__t6VIList1ZP9VIGUIPage(void* self);
void __builtin_delete(void* pointer);

void _$_t6VIList1ZP9VIGUIPage(void* self, int flags) {
    Clear__t6VIList1ZP9VIGUIPage(self);
    if ((flags & 1) != 0) __builtin_delete(self);
}
}
