static int calls;
static int sx, sy, scolor;
static unsigned short seen[16];
static int widen_calls;

extern "C" {
void ustrcpy__FPUsPc(unsigned short* dst, const char* src) {
    widen_calls += 1;
    int i = 0;
    for (; src[i]; ++i) dst[i] = static_cast<unsigned char>(src[i]);
    dst[i] = 0;
}
void fontPrintu__FiiPUsi(int x, int y, const unsigned short* text, int color) {
    calls += 1;
    sx = x;
    sy = y;
    scolor = color;
    for (int i = 0; i < 16; ++i) {
        seen[i] = text[i];
        if (!text[i]) break;
    }
}
void fontPrint__FiiPci(int x, int y, const char* text, int color);
}

int main() {
    fontPrint__FiiPci(12, -3, "Gold", 77);
    if (widen_calls != 1 || calls != 1) return 1;
    if (sx != 12 || sy != -3 || scolor != 77) return 2;
    if (seen[0] != 'G' || seen[1] != 'o' || seen[2] != 'l' || seen[3] != 'd' || seen[4] != 0) return 3;
    return 0;
}
