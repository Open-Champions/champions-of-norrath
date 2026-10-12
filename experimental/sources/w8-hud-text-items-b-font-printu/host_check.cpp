static int calls;
static int sx, sy, scolor;
static const unsigned short* stext;

extern "C" {
void _fontPrintu__FiiPUsi(int x, int y, const unsigned short* text, int color) {
    calls += 1;
    sx = x;
    sy = y;
    stext = text;
    scolor = color;
}
void fontPrintu__FiiPUsi(int x, int y, const unsigned short* text, int color);
}

int main() {
    static const unsigned short text[3] = {72, 105, 0};
    fontPrintu__FiiPUsi(-5, 300, text, 0x80ffffff);
    if (calls != 1) return 1;
    if (sx != -5 || sy != 300 || stext != text || scolor != static_cast<int>(0x80ffffff)) return 2;
    return 0;
}
