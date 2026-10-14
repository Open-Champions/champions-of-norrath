// Observed drawNamePad__Fv at 0x0027fb78, 2312 bytes, 448-byte frame. Draws the name entry screen for `curPlayer`.
// It stores 10 into `nameGridW` first. flareAlpha (the file-local float at 0x00486d3c; the image has three objects of that
// name) is the fade, and scaled by 79, 128, 96 and 99 it gives the alpha byte or argument of the colours below.
//  1. With mediumFont, font colour (alpha(79) << 24) | 0x00585e5d: one centered glyph per grid cell i of the nameGridCount
//     cells, from fontGrid[i], at x = 61 + 20 * (i % nameGridW) + (29 when `language` is nonzero), y = 109 + 20 * (i /
//     nameGridW). The cell equal to nameGridPos is drawn first with the throb colour (throbR | throbG << 8 | throbB << 16 |
//     throbA << 24) and the normal colour is set again after it.
//  2. Below the grid, at y = 109 + 20 * (cellsDrawn / nameGridW): "selectSetNameSpace" centered at x = 163 + offset and
//     "selectSetNameDone" at x = 233 + offset, where offset is 29 when `language` is nonzero, each throb coloured when
//     nameGridPos is -2 or -1 respectively; the normal colour is restored after each, and once more unconditionally after the
//     first.
//  3. Three help lines at x = 81, y = 149 + 20 * (cellsDrawn / nameGridW) + 20 * k, with fontColor(normal colour,
//     (int)(flareAlpha * 128), 0): "selectSetNameShift", "selectSetNameDelete" and "selectSetNameLetter", each formatted
//     with usprintf and gameButtonToIcon(8, 128 or 64) as a float.
//  4. A rectangle whose centre and width are blended from nameGridSrcX/Y/W to nameGridDstX/Y/W with
//     evalHermite(otherHermiteTime, 0, 1, 0.5, 0.5), rounded half away from zero: (x - w / 2, y - 9, x - w / 2 + w, y + 11)
//     with rectangleDraw argument 6 and colour (alpha(96) << 24) | 0x001e3053.
//  5. With smallFont and colour (alpha(99) << 24) | 0x00316864: "selectSetName" wrapped at x 41, y 49 with a width of 219
//     (289 when `language` is nonzero) and the limits -100, 580, 17.
//  6. With mediumFont: the current name (playerSelectInfo + 41 * curPlayer) is copied, "_" is appended when bit 0x20 of
//     frameNm is set, and it is printed at x = origin - overflow, y = 83, where origin = 41 + 9 (41 + 4 when `language` is
//     nonzero) and overflow is the name's fontStringSize minus 169, at least 0. The scissor is set to 0..640 by 0..448 with
//     its left edge at origin around the print and reset afterwards.
// Nothing is returned.
// Inferred: this is the draw half of the name pad; the colours are the unit-local palette of the selection screens.
// Unknown: what the fontStart/fontColor numeric arguments (6, -1, 0) mean, the exact meaning of the 79, 96, 99 and 128
// scales, and the origin of the rectangle numbers.
typedef unsigned short UString;

#if defined(__mips__)
#define LIBRARY_SYMBOL(name) __asm__(#name)
#else
#define LIBRARY_SYMBOL(name)
#endif

extern "C" {
extern int curPlayer;
extern int nameGridPos;
extern int nameGridW;
extern int nameGridCount;
extern float flareAlpha __asm__("norrath_player_select_view_486d3c");
extern float otherHermiteTime;
extern float nameGridSrcX, nameGridSrcY, nameGridSrcW, nameGridDstX, nameGridDstY, nameGridDstW;
extern int throbR, throbG, throbB, throbA;
extern signed char language;
extern int frameNm;
extern unsigned mediumFont;
extern unsigned smallFont;
extern unsigned char fontGrid[];
extern unsigned char playerSelectInfo[];

void fontStart__FP4FontiUii(void* font, int a, unsigned color, int b);
void fontEnd__Fv();
void fontColor__FUiii(unsigned color, int a, int b);
void fontPrintCenteredu__FiiPUsi(int x, int y, UString* text, int width);
void fontPrintu__FiiPUsi(int x, int y, UString* text, int width);
int fontPrintWrapu__FiiPUsiiii(int x, int y, UString* text, int a, int b, int c, int d);
void fontPrint__FiiPci(int x, int y, char* text, int width);
int fontStringSize__FP4FontPci(void* font, char* text, int width);
void fontScissor__Fiiii(int a, int b, int c, int d);
UString* textGetString__FPc(const char* key);
void usprintf__FPUsT0ffffffff(UString* destination, UString* format, float a, float b, float c, float d, float e, float f, float g, float h);
int gameButtonToIcon__Fi(int button);
float evalHermite__Ffffff(float t, float a, float b, float c, float d);
void rectangleDraw__FiiiiiUi(int x0, int y0, int x1, int y1, int fifth, unsigned color);
char* copyText(char* destination, const char* source) LIBRARY_SYMBOL(strcpy);
char* appendText(char* destination, const char* source) LIBRARY_SYMBOL(strcat);
}

namespace {
inline unsigned alphaOf(float scale) { return static_cast<unsigned>(flareAlpha * scale); }
inline unsigned normalColor() { return (alphaOf(79.0f) << 24) | 0x00585e5du; }
inline unsigned throbColor() {
    return static_cast<unsigned>(throbR) | (static_cast<unsigned>(throbA) << 24) |
           ((static_cast<unsigned>(throbB) << 16) | (static_cast<unsigned>(throbG) << 8));
}
inline int roundAway(float value) { return static_cast<int>(value >= 0.0f ? value + 0.5f : value - 0.5f); }
inline void* font(unsigned word) { return reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(word)); }
}

extern "C" void drawNamePad__Fv() {
    nameGridW = 10;
    fontStart__FP4FontiUii(font(mediumFont), 6, normalColor(), -1);
    int cells = 0;
    for (; cells < nameGridCount; ++cells) {
        UString text[2];
        text[0] = fontGrid[cells];
        text[1] = 0;
        if (cells == nameGridPos) fontColor__FUiii(throbColor(), -1, 0);
        fontPrintCenteredu__FiiPUsi(61 + 20 * (cells % nameGridW) + (language != 0 ? 29 : 0), 109 + 20 * (cells / nameGridW), text, 1000000);
        if (cells == nameGridPos) fontColor__FUiii(normalColor(), -1, 0);
    }
    const int offset = language != 0 ? 29 : 0;
    int y = 109 + 20 * (cells / nameGridW);
    if (nameGridPos == -2) fontColor__FUiii(throbColor(), -1, 0);
    fontPrintCenteredu__FiiPUsi(163 + offset, y, textGetString__FPc("selectSetNameSpace"), 1000000);
    if (nameGridPos == -2) fontColor__FUiii(normalColor(), -1, 0);
    fontColor__FUiii(normalColor(), -1, 0);
    if (nameGridPos == -1) fontColor__FUiii(throbColor(), -1, 0);
    fontPrintCenteredu__FiiPUsi(233 + offset, y, textGetString__FPc("selectSetNameDone"), 1000000);
    if (nameGridPos == -1) fontColor__FUiii(normalColor(), -1, 0);
    y = 149 + 20 * (cells / nameGridW);
    fontColor__FUiii(normalColor(), static_cast<int>(flareAlpha * 128.0f), 0);
    static const char* const keys[3] = {"selectSetNameShift", "selectSetNameDelete", "selectSetNameLetter"};
    static const int buttons[3] = {8, 128, 64};
    for (int k = 0; k < 3; ++k) {
        UString text[256];
        UString* format = textGetString__FPc(keys[k]);
        const int icon = gameButtonToIcon__Fi(buttons[k]);
        usprintf__FPUsT0ffffffff(text, format, static_cast<float>(icon), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
        fontPrintu__FiiPUsi(81, y, text, 1000000);
        y += 20;
    }
    fontEnd__Fv();

    const float t = evalHermite__Ffffff(otherHermiteTime, 0.0f, 1.0f, 0.5f, 0.5f);
    const int centerX = roundAway(nameGridSrcX + (nameGridDstX - nameGridSrcX) * t);
    const int centerY = roundAway(nameGridSrcY + (nameGridDstY - nameGridSrcY) * t);
    const int width = roundAway(nameGridSrcW + (nameGridDstW - nameGridSrcW) * t);
    const int left = centerX - width / 2;
    rectangleDraw__FiiiiiUi(left, centerY - 9, left + width, centerY + 11, 6, (alphaOf(96.0f) << 24) | 0x001e3053u);

    fontStart__FP4FontiUii(font(smallFont), 6, (alphaOf(99.0f) << 24) | 0x00316864u, -1);
    UString* title = textGetString__FPc("selectSetName");
    fontPrintWrapu__FiiPUsiiii(41, 49, title, (language != 0 ? 334 : 269) - (language != 0 ? 4 : 9) - 41, -100, 580, 17);
    fontEnd__Fv();

    fontStart__FP4FontiUii(font(mediumFont), 6, normalColor(), -1);
    char* const name = reinterpret_cast<char*>(playerSelectInfo + 41 * curPlayer);
    const int origin = (language != 0 ? 4 : 9) + 41;
    int overflow = fontStringSize__FP4FontPci(font(mediumFont), name, 100000) - 169;
    if (overflow < 0) overflow = 0;
    fontScissor__Fiiii(origin, 0, 640, 448);
    char shown[128];
    copyText(shown, name);
    if (frameNm & 0x20) appendText(shown, "_");
    fontPrint__FiiPci(origin - overflow, 83, shown, 100000);
    fontScissor__Fiiii(0, 0, 640, 448);
    fontEnd__Fv();
}
