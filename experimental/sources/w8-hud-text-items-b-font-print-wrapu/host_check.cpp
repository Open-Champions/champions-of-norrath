#include <cstring>

extern "C" {
int fontLastPrintedPosition;
int restrictChars;
void* curFont;
signed char language;
int fontPrintWrapu__FiiPUsiiii(int x, int y, const unsigned short* text, int width, int y_min, int y_max, int line_step);
}

struct Print {
    int x, y, count;
    const unsigned short* text;
};
static Print prints[16];
static int nprints;
static int size_calls;
static void* size_font;

extern "C" {
int _fontStringSizeu__FP4FontPUsi(void* font, const unsigned short* text, int count) {
    ++size_calls;
    size_font = font;
    int w = 0;
    for (int i = 0; i < count; ++i) w += text[i] == 'i' ? 5 : 10;   // 'i' is narrow
    return w;
}
void _fontPrintu__FiiPUsi(int x, int y, const unsigned short* text, int count) {
    Print& p = prints[nprints++];
    p.x = x; p.y = y; p.count = count; p.text = text;
}
}

static unsigned short buf[64];
static const unsigned short* set(const char* s) {
    int i = 0;
    for (; s[i]; ++i) buf[i] = static_cast<unsigned char>(s[i]);
    buf[i] = 0;
    return buf;
}

static void reset() {
    nprints = 0;
    size_calls = 0;
    language = 0;
    restrictChars = 100000;
    fontLastPrintedPosition = 77;
    static int font_marker;
    curFont = &font_marker;
}

int main() {
    // "aa bb cc", width 45: lines "aa ", "bb ", "cc".
    reset();
    const unsigned short* t = set("aa bb cc");
    int r = fontPrintWrapu__FiiPUsiiii(5, 100, t, 45, -1000, 1000, 20);
    if (r != 60 || nprints != 3) return 1;
    if (prints[0].text != t || prints[0].count != 3 || prints[0].y != 100 || prints[0].x != 5) return 2;
    if (prints[1].text != t + 3 || prints[1].count != 3 || prints[1].y != 120) return 3;
    if (prints[2].text != t + 6 || prints[2].count != 2 || prints[2].y != 140) return 4;
    if (fontLastPrintedPosition != 8 || size_font == nullptr) return 5;

    // Everything fits on one line.
    reset();
    t = set("aa bb");
    r = fontPrintWrapu__FiiPUsiiii(0, 50, t, 1000, 0, 1000, 15);
    if (r != 15 || nprints != 1 || prints[0].count != 5 || fontLastPrintedPosition != 5) return 6;

    // Newline forces a break; the newline stays with the earlier line.
    reset();
    t = set("ab\ncd");
    r = fontPrintWrapu__FiiPUsiiii(0, 10, t, 1000, 0, 1000, 20);
    if (r != 40 || nprints != 2) return 7;
    if (prints[0].count != 3 || prints[0].text != t || prints[1].text != t + 3 || prints[1].count != 2 || prints[1].y != 30) return 8;

    // restrictChars of 4: the first line prints 3, the second 1, then printing stops.
    reset();
    restrictChars = 4;
    t = set("aa bb cc");
    r = fontPrintWrapu__FiiPUsiiii(0, 100, t, 45, -1000, 1000, 20);
    if (r != 60 || nprints != 2) return 9;
    if (prints[0].count != 3 || prints[1].count != 1 || fontLastPrintedPosition != 4) return 10;

    // Vertical clip: y must be strictly between the bounds.
    reset();
    t = set("aa bb cc");
    r = fontPrintWrapu__FiiPUsiiii(0, 100, t, 45, 100, 140, 20);
    if (r != 60 || nprints != 1 || prints[0].y != 120 || prints[0].text != t + 3) return 11;
    if (fontLastPrintedPosition != 6) return 12;     // the skipped line was still counted

    // Language 5: every character is its own segment.
    reset();
    language = 5;
    t = set("abcd");
    r = fontPrintWrapu__FiiPUsiiii(0, 0, t, 25, -10, 100, 10);
    // widths 10 each: a (10), b (20), c would be 30 -> break; line "ab", then "cd".
    if (r != 20 || nprints != 2 || prints[0].count != 2 || prints[1].count != 2 || prints[1].text != t + 2) return 13;

    // A word wider than the line is cut where the width would be reached.
    reset();
    t = set("abcdef");
    r = fontPrintWrapu__FiiPUsiiii(0, 0, t, 25, -10, 100, 10);
    // segments of 2 characters (20 wide): each next one overflows, so three lines.
    if (r != 30 || nprints != 3) return 14;
    if (prints[0].count != 2 || prints[1].count != 2 || prints[2].count != 2) return 15;

    // Empty text.
    reset();
    t = set("");
    r = fontPrintWrapu__FiiPUsiiii(0, 9, t, 45, 0, 100, 20);
    if (r != 0 || nprints != 0 || fontLastPrintedPosition != 0) return 16;

    // Trailing newline: the final segment is empty and no extra line is printed.
    reset();
    t = set("ab\n");
    r = fontPrintWrapu__FiiPUsiiii(0, 10, t, 1000, 0, 1000, 20);
    if (r != 20 || nprints != 1 || prints[0].count != 3) return 17;
    return 0;
}
