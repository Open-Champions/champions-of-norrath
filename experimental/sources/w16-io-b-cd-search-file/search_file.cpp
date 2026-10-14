// Observed: VICdSearchFile__FPCcPUiT1 at 0x01000898 is 188 bytes and returns an int.
// Arguments (name, first, second). If the pending word (0x011e2640) is nonzero it
// stores 2 in the status word (0x011e2644) and returns -1. Otherwise it stores 3
// in the first word of the request block (0x011e25c0), copies the name with
// strcpy to the block's second word (offset 4), and calls FlushIop__Fi(0). A
// negative result stores 1 in status and returns -1. Otherwise the first word of
// the reply block (0x011e2600) is stored in status; if it is nonzero -1 is
// returned. Else reply words 1 and 2 are stored through first and second and 0
// is returned. The pending word is not touched.
// Inferred: command 3 is a file lookup on the IOP side; the two outputs are the
// found file's start sector and size, since CD archive callers use them so.
// Unknown: the IOP-side meaning of the words and the request block's real extent
// (a long name would overrun it).
extern "C" {
extern int norrath_cdstream_pending;
extern int norrath_cdstream_status;
extern int norrath_cdstream_send[];
extern int norrath_cdstream_recv[];
char* strcpy(char* destination, const char* source);
int FlushIop__Fi(int argument);

int VICdSearchFile__FPCcPUiT1(const char* name, unsigned* first, unsigned* second) {
    if (norrath_cdstream_pending != 0) {
        norrath_cdstream_status = 2;
        return -1;
    }
    norrath_cdstream_send[0] = 3;
    strcpy(reinterpret_cast<char*>(&norrath_cdstream_send[1]), name);
    if (FlushIop__Fi(0) < 0) {
        norrath_cdstream_status = 1;
        return -1;
    }
    norrath_cdstream_status = norrath_cdstream_recv[0];
    if (norrath_cdstream_recv[0] != 0) return -1;
    *first = static_cast<unsigned>(norrath_cdstream_recv[1]);
    *second = static_cast<unsigned>(norrath_cdstream_recv[2]);
    return 0;
}
}
