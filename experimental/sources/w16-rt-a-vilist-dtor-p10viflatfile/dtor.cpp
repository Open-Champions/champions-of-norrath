// Observed: _$_t6VIList1ZP10VIFlatFile at 0x011f3b70 is 84 bytes and returns nothing. With (this, flags) it
// calls Clear__t6VIList1ZP10VIFlatFile (0x011f3d30) with this, then when bit 0 of flags is set it tail-calls
// __builtin_delete (0x002fedf0) with this. Inferred: bit 0 is the delete-after flag of the compiler's destructor
// convention; only the mangled template name and this call shape are evidence for the container type.
extern "C" {
void Clear__t6VIList1ZP10VIFlatFile(void* self);
void __builtin_delete(void* pointer);

void _$_t6VIList1ZP10VIFlatFile(void* self, int flags) {
    Clear__t6VIList1ZP10VIFlatFile(self);
    if ((flags & 1) != 0) __builtin_delete(self);
}
}
