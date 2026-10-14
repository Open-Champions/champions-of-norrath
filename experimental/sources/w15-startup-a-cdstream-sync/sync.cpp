// Observed: VICdStreamSync__Fi at 0x01000b08 is 76 bytes and returns an int. With a
// zero argument it repeats RotateThreadReadyQueue(1) while the pending word at
// 0x011e2640 (local sPending, imported as norrath_cdstream_pending) is nonzero, re-reading
// it after every call, and returns 0. With a nonzero argument it makes no call and
// returns 1 when the pending word is nonzero, else 0.
// Inferred: zero means "wait for the outstanding request", nonzero means "poll"; the
// result convention (0 = finished, 1 = busy) matches how CDSync passes it on.
// Unknown: who clears the pending word (an IOP reply path, not part of this unit).
extern "C" {
extern volatile int norrath_cdstream_pending;
int RotateThreadReadyQueue(int priority);

int VICdStreamSync__Fi(int mode) {
    if (mode == 0) {
        while (norrath_cdstream_pending != 0) RotateThreadReadyQueue(1);
        return 0;
    }
    return norrath_cdstream_pending != 0;
}
}
