static void* size_font;
static const unsigned short* size_text;
static int size_count;
static int width_to_return;
static int calls;
static int px, py, pcount;
static const unsigned short* ptext;

extern "C" {
void* curFont;
int _fontStringSizeu__FP4FontPUsi(void* font, const unsigned short* text, int count) {
    size_font = font;
    size_text = text;
    size_count = count;
    return width_to_return;
}
void _fontPrintu__FiiPUsi(int x, int y, const unsigned short* text, int count) {
    calls += 1;
    px = x;
    py = y;
    ptext = text;
    pcount = count;
}
void fontPrintCenteredu__FiiPUsi(int x, int y, const unsigned short* text, int count);
}

static int run(int x, int width, const unsigned short* text) {
    width_to_return = width;
    fontPrintCenteredu__FiiPUsi(x, 8, text, 9);
    return px;
}

int main() {
    static const unsigned short text[3] = {72, 105, 0};
    int marker;
    curFont = &marker;
    if (run(320, 100, text) != 270) return 1;
    if (calls != 1 || size_font != &marker || size_text != text || size_count != 9) return 2;
    if (ptext != text || py != 8 || pcount != 9) return 3;
    if (run(320, 101, text) != 270) return 4;
    if (run(320, -3, text) != 322) return 5;
    return 0;
}
