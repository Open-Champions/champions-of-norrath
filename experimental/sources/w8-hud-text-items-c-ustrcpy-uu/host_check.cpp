extern "C" void ustrcpy__FPUsT0(unsigned short* dst, unsigned short* src);

int main() {
    for (int len = 0; len < 20; len++) {
        unsigned short src[32], dst[32];
        for (int i = 0; i < 32; i++) {
            src[i] = (unsigned short)(0x1100u + i * 3u + 1u);
            dst[i] = 0xdead;
        }
        src[len] = 0;
        ustrcpy__FPUsT0(dst, src);
        for (int i = 0; i < len; i++) if (dst[i] != src[i]) return 1;
        if (dst[len] != 0) return 2;
        // Nothing past the terminator is written.
        for (int i = len + 1; i < 32; i++) if (dst[i] != 0xdead) return 3;
    }
    // A unit with only the low byte zero is not a terminator.
    unsigned short src[3] = {0x4100, 0x0042, 0}, dst[4] = {1, 1, 1, 1};
    ustrcpy__FPUsT0(dst, src);
    if (dst[0] != 0x4100 || dst[1] != 0x0042 || dst[2] != 0 || dst[3] != 1) return 4;
    return 0;
}
