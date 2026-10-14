// Observed: sceLibnetWaitGetAddress at 0x002e62e0, 52 bytes, pinned SLUS_205.65. Six arguments (a0-a3, t0, t1). When bit 1
// (value 2) of the sixth argument is set it returns the result of WaitGetAddressAutoUpIf at 0x002e6148 with the first five
// arguments unchanged; otherwise the result of WaitGetAddressNoAutoUpIf at 0x002e61e0 with the same five. Both callees take five
// arguments (the sixth is never read by either).
// Inferred: the flag chooses between waiting with and without an automatic interface bring-up, from the callee names.
// Unknown: both callees' bodies (they are local functions of the library and are left as imports), and the other bits of the flag.
extern "C" {
int WaitGetAddressAutoUpIf(void* object, void* buffer, int* state, int third, int fourth);
int WaitGetAddressNoAutoUpIf(void* object, void* buffer, int* state, int third, int fourth);

int sceLibnetWaitGetAddress(void* object, void* buffer, int* state, int third, int fourth, unsigned flags) {
    if ((flags & 2) != 0) return WaitGetAddressAutoUpIf(object, buffer, state, third, fourth);
    return WaitGetAddressNoAutoUpIf(object, buffer, state, third, fourth);
}
}
