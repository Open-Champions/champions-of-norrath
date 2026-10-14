// Observed kdm_rpccall_end at 0x002e9068, 48 bytes (a file-local routine in the original). The
// completion function that the keyboard/mouse reader thread passes to sceSifCallRpc. Reads the word at
// GLibData + 56: when it is zero, releases the semaphore whose id is the word at GLibData + 40 (the
// one the thread waits on after each call) with iSignalSema, the interrupt-context form, as its last
// action (a jump, no result used); when it is non-zero, does nothing. The argument is not read.
// Inferred only: the word at +56 is a "do not signal" switch; nothing else about it is known.
// Nothing was executed on the EE.
extern "C" {
extern unsigned char GLibData[] __attribute__((aligned(4)));
int iSignalSema(int semaphore);

void kdm_rpccall_end(void*) {
    if (*reinterpret_cast<int*>(GLibData + 56) == 0) iSignalSema(*reinterpret_cast<int*>(GLibData + 40));
}
}
