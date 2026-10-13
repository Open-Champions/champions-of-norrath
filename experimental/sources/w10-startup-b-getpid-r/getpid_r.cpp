/* Observed _getpid_r at 0x00303ce0, 20 bytes. It ignores its reentrancy-record argument and
 * tail-calls getpid, returning its result unchanged; the original saves and restores the
 * return address around the jump. _raise_r passes its result to _kill_r.
 * Unknown: what getpid returns on the target. Compiled output is not claimed to match the
 * original.
 */
extern "C" {
int getpid();

int _getpid_r(void* reent) {
    (void)reent;
    return getpid();
}
}
