// Observed ustrcpy__FPcPUs at 0x001918a0, 56 bytes.
// It narrows 16-bit units from the second argument to bytes at the first
// until the first zero unit, keeping only the low byte of each unit, then
// stores a zero byte. A unit such as 0x4100 whose low byte is zero but which
// is nonzero as a whole is copied as a zero byte and does not stop the copy.
extern "C" void ustrcpy__FPcPUs(char* dst, unsigned short* src) {
    while (*src != 0) {
        *dst = (char)(*src & 0xff);
        src++;
        dst++;
    }
    *dst = 0;
}
