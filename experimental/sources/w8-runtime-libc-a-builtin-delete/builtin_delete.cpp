// Observed __builtin_delete at 0x002fedf0, 32 bytes (a weak symbol). A null
// pointer returns without a call; any other pointer is passed to __wrap_free at
// 0x00144ac0 and nothing is returned. The adjacent routine at 0x002fee10
// (not authored here) forwards to this one.
// Unknown: __wrap_free's behavior, which belongs to another package.
extern "C" {
void __wrap_free(void* allocation);

__attribute__((weak)) void __builtin_delete(void* allocation) {
    if (allocation != nullptr) __wrap_free(allocation);
}
}
