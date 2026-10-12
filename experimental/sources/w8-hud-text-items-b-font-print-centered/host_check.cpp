static int order[8];
static int steps;
static void* size_font;
static const char* size_text;
static int size_count;
static int width_to_return;
static int px, py, pcount;
static unsigned short seen[16];

extern "C" {
void* curFont;
int _fontStringSize__FP4FontPci(void* font, const char* text, int count) {
    order[steps++] = 1;
    size_font = font;
    size_text = text;
    size_count = count;
    return width_to_return;
}
void ustrcpy__FPUsPc(unsigned short* dst, const char* src) {
    order[steps++] = 2;
    int i = 0;
    for (; src[i]; ++i) dst[i] = static_cast<unsigned char>(src[i]);
    dst[i] = 0;
}
void _fontPrintu__FiiPUsi(int x, int y, const unsigned short* text, int count) {
    order[steps++] = 3;
    px = x;
    py = y;
    pcount = count;
    for (int i = 0; i < 16; ++i) {
        seen[i] = text[i];
        if (!text[i]) break;
    }
}
void fontPrintCentered__FiiPci(int x, int y, const char* text, int count);
}

static int run(int x, int width) {
    steps = 0;
    width_to_return = width;
    fontPrintCentered__FiiPci(x, 40, "Hi", 5);
    return px;
}

int main() {
    int marker;
    curFont = &marker;
    if (run(320, 100) != 270) return 1;
    if (steps != 3 || order[0] != 1 || order[1] != 2 || order[2] != 3) return 2;
    if (size_font != &marker || size_count != 5 || size_text[0] != 'H') return 3;
    if (py != 40 || pcount != 5) return 4;
    if (seen[0] != 'H' || seen[1] != 'i' || seen[2] != 0) return 5;
    if (run(320, 101) != 270) return 6;
    if (run(320, -3) != 322) return 7;
    if (run(10, 0) != 10) return 8;
    return 0;
}
