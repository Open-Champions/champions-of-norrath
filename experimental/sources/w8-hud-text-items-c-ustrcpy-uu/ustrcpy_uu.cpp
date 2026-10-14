// Observed ustrcpy__FPUsT0 at 0x00191830, 52 bytes.
// It copies 16-bit units from the second argument to the first up to and
// including the first zero unit. The terminator is written after the loop.
extern "C" void ustrcpy__FPUsT0(unsigned short* dst, unsigned short* src) {
    while (*src != 0) {
        *dst = *src;
        src++;
        dst++;
    }
    *dst = 0;
}
