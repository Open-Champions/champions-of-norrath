// Observed Exit at 0x002ddce8, 40 bytes. Calls TerminateLibrary (0x002ddc28) with no argument,
// then hands the original status argument on to _Exit (0x002d4d00) as a tail call. The status
// is kept across the first call in a saved register. Nothing is returned from here: whatever
// _Exit leaves in the result register is the caller's result.
// Unknown: what _Exit does with the status (it is a bare kernel-service stub).
// Nothing was executed on the EE.
extern "C" {
void TerminateLibrary();
void _Exit(int status);

void Exit(int status) {
    TerminateLibrary();
    _Exit(status);
}
}
