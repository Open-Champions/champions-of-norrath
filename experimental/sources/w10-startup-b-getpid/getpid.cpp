/* Observed getpid at 0x002d58b0, 8 bytes. It returns 1 and touches nothing else; the
 * target has one process. _getpid_r tail-calls it. Compiled output is not claimed to
 * match the original.
 */
extern "C" int getpid() {
    return 1;
}
