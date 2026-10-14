// Host check only; not an EE build input.
extern "C" void ustrcat__FPUsPc(unsigned short*, char*);

int main() {
    for (int dlen = 0; dlen < 6; ++dlen) {
        for (int slen = 0; slen < 6; ++slen) {
            unsigned short dst[24];
            char src[12];
            for (int i = 0; i < 24; ++i) dst[i] = 0xdead;
            for (int i = 0; i < dlen; ++i) dst[i] = static_cast<unsigned short>(0x4100 + i + 1);
            dst[dlen] = 0;
            for (int i = 0; i < slen; ++i) src[i] = static_cast<char>(i == 1 ? 0xe1 : 'A' + i);
            src[slen] = 0;
            ustrcat__FPUsPc(dst, src);
            for (int i = 0; i < dlen; ++i) if (dst[i] != 0x4100 + i + 1) return 1;
            for (int i = 0; i < slen; ++i) {
                const unsigned short want = i == 1 ? 0x00e1 : static_cast<unsigned short>('A' + i);
                if (dst[dlen + i] != want) return 2;
            }
            if (dst[dlen + slen] != 0) return 3;
            for (int i = dlen + slen + 1; i < 24; ++i) if (dst[i] != 0xdead) return 4;
        }
    }
    // A destination unit with only its low byte zero is not a terminator.
    unsigned short dst[6] = {0x4100, 0, 0xdead, 0xdead, 0xdead, 0xdead};
    char src[2] = {'B', 0};
    ustrcat__FPUsPc(dst, src);
    if (dst[0] != 0x4100 || dst[1] != 'B' || dst[2] != 0 || dst[3] != 0xdead) return 5;
    return 0;
}
