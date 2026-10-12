extern "C" void ustrcat__FPUsT0(unsigned short* dst, unsigned short* src);

int main() {
    for (int dl = 0; dl < 8; dl++) {
        for (int sl = 0; sl < 8; sl++) {
            unsigned short dst[32], src[16];
            for (int i = 0; i < 32; i++) dst[i] = (unsigned short)(0x500 + i);
            for (int i = 0; i < 16; i++) src[i] = (unsigned short)(0x9000 + i);
            dst[dl] = 0;
            src[sl] = 0;
            unsigned short want[32];
            for (int i = 0; i < 32; i++) want[i] = dst[i];
            for (int i = 0; i < sl; i++) want[dl + i] = src[i];
            want[dl + sl] = 0;
            ustrcat__FPUsT0(dst, src);
            for (int i = 0; i < 32; i++) if (dst[i] != want[i]) return 1;
        }
    }
    return 0;
}
