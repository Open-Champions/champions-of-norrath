// Observed _$_10VILauncher at 0x0106c260, 44 bytes. It returns.
// Bit 0 of the flag selects __builtin_delete. The object bytes are left
// unchanged. What __builtin_delete releases is unknown.
extern "C" {
struct VILauncher;
void __builtin_delete(void* pointer);

void _$_10VILauncher(VILauncher* object, int flags) {
    if ((flags & 1) != 0) __builtin_delete(object);
}
}
