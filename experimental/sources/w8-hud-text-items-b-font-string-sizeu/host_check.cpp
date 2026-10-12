static void* seen_font;
static const unsigned short* seen_text;
static int seen_count;
static int calls;

extern "C" {
int _fontStringSizeu__FP4FontPUsi(void* font, const unsigned short* text, int count) {
    calls += 1;
    seen_font = font;
    seen_text = text;
    seen_count = count;
    return 0x1234 + count;
}
int fontStringSizeu__FP4FontPUsi(void* font, const unsigned short* text, int count);
}

int main() {
    static const unsigned short text[2] = {65, 0};
    int font_marker;
    int r = fontStringSizeu__FP4FontPUsi(&font_marker, text, -9);
    if (calls != 1) return 1;
    if (seen_font != &font_marker || seen_text != text || seen_count != -9) return 2;
    if (r != 0x1234 - 9) return 3;
    return 0;
}
