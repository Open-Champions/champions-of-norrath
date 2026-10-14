// Observed read at 0x002d5728, 116 bytes. Only descriptor 0 is accepted;
// any other value returns -1. The tty word ttyinit (0x00444950, zero in the
// image) is checked first. When it is zero, sceTtyInit is called with the
// buffer pointer as its only argument; a zero result returns -1, otherwise
// ttyinit is set to 1. Then sceTtyRead is called with the buffer and the
// count, and its result is returned. The callee argument contract beyond
// these two words is not established.
extern int ttyinit;
extern "C" int sceTtyInit(void* buffer);
extern "C" int sceTtyRead(void* buffer, unsigned count);

extern "C" int read(int descriptor, void* buffer, unsigned count) {
    if (descriptor != 0) return -1;
    if (ttyinit == 0) {
        if (sceTtyInit(buffer) == 0) return -1;
        ttyinit = 1;
    }
    return sceTtyRead(buffer, count);
}
