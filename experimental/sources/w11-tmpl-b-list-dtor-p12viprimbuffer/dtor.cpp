// Observed: _$_t6VIList1ZP12VIPrimBuffer at 0x01210fb8 is 84 bytes and returns nothing. With (this, flags) it
// calls Clear__t6VIList1ZP12VIPrimBuffer(this), then when bit 0 of flags is set tail-calls __builtin_delete(this).
// Inferred: flags bit 0 is the delete-after flag of the compiler's destructor convention;
// only the mangled template name and this call shape are evidence for the container type.
extern "C" {
void Clear__t6VIList1ZP12VIPrimBuffer(void* self);
void __builtin_delete(void* pointer);

void _$_t6VIList1ZP12VIPrimBuffer(void* self, int flags) {
    Clear__t6VIList1ZP12VIPrimBuffer(self);
    if ((flags & 1) != 0) __builtin_delete(self);
}
}
