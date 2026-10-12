extern "C" void ustrcpy__FPUsPc(unsigned short* dst, char* src);

int main() {
    for (int len = 0; len < 20; len++) {
        char src[32];
        unsigned short dst[32];
        for (int i = 0; i < 32; i++) {
            // Includes bytes with the high bit set, which must not sign-extend.
            src[i] = (char)(0x70 + i * 5);
            if (src[i] == 0) src[i] = 1;
            dst[i] = 0xdead;
        }
        src[len] = 0;
        ustrcpy__FPUsPc(dst, src);
        for (int i = 0; i < len; i++) if (dst[i] != (unsigned char)src[i]) return 1;
        if (dst[len] != 0) return 2;
        for (int i = len + 1; i < 32; i++) if (dst[i] != 0xdead) return 3;
    }
    char hi[2] = {(char)0xe9, 0};
    unsigned short out[3] = {7, 7, 7};
    ustrcpy__FPUsPc(out, hi);
    if (out[0] != 0x00e9 || out[1] != 0 || out[2] != 7) return 4;
    return 0;
}
