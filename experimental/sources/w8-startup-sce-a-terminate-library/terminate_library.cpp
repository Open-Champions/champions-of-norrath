/* Observed TerminateLibrary at 0x002ddc28, 8 bytes. It is a single jump to
 * InitTLB (0x002dfd68) with no frame, so the caller returns from InitTLB. It
 * reads no argument. What InitTLB leaves in the result register is not
 * described here. Compiled output is not claimed to match the original.
 */
extern "C" void InitTLB();

extern "C" void TerminateLibrary() {
    InitTLB();
}
