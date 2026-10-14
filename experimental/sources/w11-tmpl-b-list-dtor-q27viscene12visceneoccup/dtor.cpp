// Observed: _$_t6VIList1ZQ27VIScene12VISceneOccup at 0x012243e0 is 84 bytes and returns nothing. With (this, flags) it
// calls Clear__t6VIList1ZQ27VIScene12VISceneOccup(this), then when bit 0 of flags is set tail-calls __builtin_delete(this).
// Inferred: flags bit 0 is the delete-after flag of the compiler's destructor convention;
// only the mangled template name and this call shape are evidence for the container type.
extern "C" {
void Clear__t6VIList1ZQ27VIScene12VISceneOccup(void* self);
void __builtin_delete(void* pointer);

void _$_t6VIList1ZQ27VIScene12VISceneOccup(void* self, int flags) {
    Clear__t6VIList1ZQ27VIScene12VISceneOccup(self);
    if ((flags & 1) != 0) __builtin_delete(self);
}
}
