extern "C" void ustrcpy__FPcPUs(char* dst, unsigned short* src);

int main() {
    for (int len = 0; len < 20; len++) {
        unsigned short src[32];
        char dst[32];
        for (int i = 0; i < 32; i++) {
            src[i] = (unsigned short)(0x2300u + 0x31 + i);
            dst[i] = 0x55;
        }
        src[len] = 0;
        ustrcpy__FPcPUs(dst, src);
        for (int i = 0; i < len; i++) if ((unsigned char)dst[i] != (src[i] & 0xff)) return 1;
        if (dst[len] != 0) return 2;
        for (int i = len + 1; i < 32; i++) if (dst[i] != 0x55) return 3;
    }
    unsigned short src[3] = {0x4100, 0x0141, 0};
    char dst[4] = {9, 9, 9, 9};
    ustrcpy__FPcPUs(dst, src);
    if (dst[0] != 0 || dst[1] != 0x41 || dst[2] != 0 || dst[3] != 9) return 4;
    return 0;
}
