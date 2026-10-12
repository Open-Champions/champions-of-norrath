// Observed write at 0x002d56a8, 124 bytes. Only descriptors 1 and 2 are
// accepted; any other value returns -1. The tty word ttyinit (0x00444950,
// zero in the image) is checked first. When it is zero, sceTtyInit is called
// with the buffer pointer as its only argument; a zero result returns -1,
// otherwise ttyinit is set to 1. Then sceTtyWrite is called with the buffer
// and the count, and its result is returned. The callee argument contract
// beyond these two words is not established. sceResetttyinit clears ttyinit.
extern int ttyinit;
extern "C" int sceTtyInit(const void* buffer);
extern "C" int sceTtyWrite(const void* buffer, unsigned count);

extern "C" int write(int descriptor, const void* buffer, unsigned count) {
    if (static_cast<unsigned>(descriptor - 1) >= 2u) return -1;
    if (ttyinit == 0) {
        if (sceTtyInit(buffer) == 0) return -1;
        ttyinit = 1;
    }
    return sceTtyWrite(buffer, count);
}
