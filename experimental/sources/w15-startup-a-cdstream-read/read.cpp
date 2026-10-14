// Observed: VICdStreamRead__FUiUiPvi at 0x01000958 is 168 bytes and returns an int.
// Arguments (a, b, buffer, d). If the pending word (0x011e2640) is nonzero it stores 2 in
// the status word (0x011e2644) and returns -1 with no other store. Otherwise it stores the
// buffer pointer in last_buffer (0x0118e544), fills the request block at 0x011e25c0 with
// words 4, a, b, d, buffer (in that order), calls FlushIop__Fi(0) (0x01000b68) and, if
// that is negative, stores 1 in the status word and returns -1 (pending stays clear).
// Otherwise it stores 0 in the status word, 1 in the pending word and returns 0.
// Inferred: command 4 is a sector read request (CDSync passes sector, count, buffer and
// mode 2 for a retry); the pending word is set only after the IOP accepted the request.
// Unknown: the IOP-side meaning of the words, who reads last_buffer, and the real extent
// of the request block.
extern "C" {
extern int norrath_cdstream_pending;
extern int norrath_cdstream_status;
extern int norrath_cdstream_send[];
extern void* last_buffer;
int FlushIop__Fi(int argument);

int VICdStreamRead__FUiUiPvi(unsigned a, unsigned b, void* buffer, int d) {
    if (norrath_cdstream_pending != 0) {
        norrath_cdstream_status = 2;
        return -1;
    }
    last_buffer = buffer;
    norrath_cdstream_send[0] = 4;
    norrath_cdstream_send[1] = static_cast<int>(a);
    norrath_cdstream_send[2] = static_cast<int>(b);
    norrath_cdstream_send[3] = d;
    norrath_cdstream_send[4] = static_cast<int>(reinterpret_cast<__UINTPTR_TYPE__>(buffer));
    if (FlushIop__Fi(0) < 0) {
        norrath_cdstream_status = 1;
        return -1;
    }
    norrath_cdstream_status = 0;
    norrath_cdstream_pending = 1;
    return 0;
}
}
