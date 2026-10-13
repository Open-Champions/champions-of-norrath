// Observed __builtin_vec_delete at 0x002fee10, 28 bytes (weak), pinned SLUS_205.65. Passes its first
// argument to __builtin_delete (0x002fedf0) and returns; any further arguments are ignored,
// so no element destructor loop exists in this version. Nothing is returned.
extern "C" {
void __builtin_delete(void* pointer);

__attribute__((weak)) void __builtin_vec_delete(void* pointer) {
    __builtin_delete(pointer);
}
}
