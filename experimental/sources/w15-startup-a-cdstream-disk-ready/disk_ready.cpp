// Observed: VICdStreamDiskReady__Fv at 0x01000a90 is 120 bytes and returns an int.
// If the pending word (0x011e2640) is nonzero it stores 2 in the status word (0x011e2644)
// and returns -1 without touching the request block. Otherwise it stores 5 in word 0 of
// the request block at 0x011e25c0, calls FlushIop__Fi(0) (0x01000b68), and if that is
// negative stores 1 in the status word and returns -1. Otherwise it copies word 0 of the
// reply block at 0x011e2600 into the status word and returns 0 when that word is zero,
// -1 when it is not.
// Inferred: command 5 asks the IOP whether the disc is ready; CDSync loops while the result
// is negative, so a nonzero reply word means "not ready".
// Unknown: the reply word's other values and the extent of the request and reply blocks
// (64 bytes each only from the gaps between the local symbols).
extern "C" {
extern int norrath_cdstream_pending;
extern int norrath_cdstream_status;
extern int norrath_cdstream_send[];
extern int norrath_cdstream_recv[];
int FlushIop__Fi(int argument);

int VICdStreamDiskReady__Fv() {
    if (norrath_cdstream_pending != 0) {
        norrath_cdstream_status = 2;
        return -1;
    }
    norrath_cdstream_send[0] = 5;
    if (FlushIop__Fi(0) < 0) {
        norrath_cdstream_status = 1;
        return -1;
    }
    const int reply = norrath_cdstream_recv[0];
    norrath_cdstream_status = reply;
    return reply == 0 ? 0 : -1;
}
}
