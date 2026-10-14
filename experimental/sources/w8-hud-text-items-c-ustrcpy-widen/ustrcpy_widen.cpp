// Observed ustrcpy__FPUsPc at 0x00191868, 56 bytes.
// It widens bytes from the second argument into 16-bit units at the first
// until the first zero byte, then stores a zero unit. The stored value is the
// byte zero-extended (the loop reloads it with an unsigned byte load; only the
// zero test uses the signed load, which cannot change the outcome).
extern "C" void ustrcpy__FPUsPc(unsigned short* dst, char* src) {
    while (*src != 0) {
        *dst = (unsigned char)*src;
        src++;
        dst++;
    }
    *dst = 0;
}
