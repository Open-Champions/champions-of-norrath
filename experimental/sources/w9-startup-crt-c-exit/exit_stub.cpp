// Observed _exit at 0x00100248, 8 bytes, in the startup region ahead of the rest of
// the C library. It is a tail call to Exit (0x002ddce8) with the status argument
// left in place, so Exit's behavior is whatever the caller of _exit sees. The
// callers in this build are exit and abort (status 1). Exit itself is a separate
// routine and is not defined here.
extern "C" {
void Exit(int status);

void _exit(int status) {
    Exit(status);
}
}
