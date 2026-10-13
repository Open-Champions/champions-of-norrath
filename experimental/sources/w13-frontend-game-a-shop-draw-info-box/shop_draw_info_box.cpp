// Observed shopDrawInfoBox__FiP12screenStructPQ24item11descriptioniiiii at 0x001b7398, 1444 bytes,
// 1200-byte frame. Arguments: a player pn, a screen record, an item cell, mode, shade, tint, an
// unused seventh word, and width. The cell: word +0 is a description record (name pointer at +0,
// signed bytes +28 kind and +29), halfword +4 the stack count, 64-bit flags at +8, byte +33.
// Screen words: [0] int left edge, [11] int open flag, [16] float slide position.
//  1. width < 262 (signed): x = screen[0] + 164, y = 229; otherwise x = screen[0] + 163, y = 78.
//  2. The slide position screen[16] moves up by k1 when screen[11] is nonzero, else down by k2, is
//     stored, limited to [0, 1] (stored again when changed), and
//     y -= (int)((evalHermite__Ffffff(t, 0, 1, 0, 0) + k3) * 96).
//  3. A cell whose word +0 is zero ends here. Otherwise the name text getName__4itemPQ24item11description
//     i(cell, 1) is copied (ustrcpy) to a local text; while fontStringSizeu(smallFont, copy, 1000000)
//     of a fresh copy is 285 or more, the text loses one character and gets "..." in its place. (The
//     shortened text is not used afterwards.)
//  4. fontStart(smallFont, 6, 0x80000000 | shade * 0x010101, -1). The name colour is 0x805c2913 when
//     byte +33 is not 255, else 0x80004444 when flags bit 0x2000000000 is set, else unchanged. The
//     full name is printed with fontPrintWrapCenteredu(x, y, name, 216, -100, 580, 20); lines = result
//     / 20, itemInfoHeight[pn] += lines, y += 22 * lines - 22. fontEnd.
//  5. fontStart again with the same colour, fontScissor(0, y, 640, 407). price = getValue(cell) as a
//     float; with mode nonzero and the description kind not 8 and its name not containing "spear",
//     price is divided by the stack count; with mode nonzero it is then multiplied by k4. It is
//     formatted "%d" (truncated). The kind-8 offset dy is -19, else 0. Thousands separators are put in
//     the text (the code of commaString__FPc inlined: the first comma goes at len % 3, or len / 2
//     when that is zero, later ones four places on).
//  6. When byte +29 is not 2 and flags bit 0x100 is set: fontPrint(x - 64, y + dy + 64, text, 100000);
//     otherwise fontPrint(x - 74, y + dy + 43, text, 100000). With a stack count of 2 or more
//     "x %d" of it is centred at (x, y + 20) with limit 10000. fontEnd.
//  7. The icon tinyIcon[7] is drawn with textureDrawSuperColor(icon, x - 85, y + dy + 68, 6, tint
//     colour, 0, 92, 639, 447, 0, 0) in the first case above, (x - 95, y + dy + 47, ...) otherwise,
//     tint colour = 0x80000000 | tint * 0x010101. Last displayItemStats(pn, x, y + 23, cell, tint,
//     shade, lines).
// Constants (bit patterns): k1 0x3dba2e8b, k2 0x3dba2e8b, k3 0x3c23d70a, k4 0x3e4ccccc.
// Inferred: the item information box of the shop. Unknown: the roles of shade and tint, the
// seventh argument, and why the shortened name is computed but unused.
extern "C" {
extern int itemInfoHeight[];
extern void* smallFont;
extern void* tinyIcon[];
float evalHermite__Ffffff(float t, float a, float b, float c, float d);
unsigned short* getName__4itemPQ24item11descriptioni(void* cell, int flag);
void ustrcpy__FPUsT0(unsigned short* to, const unsigned short* from);
int ustrlen__FPUs(const unsigned short* text);
int fontStringSizeu__FP4FontPUsi(void* font, unsigned short* text, int limit);
void fontStart__FP4FontiUii(void* font, int size, unsigned color, int flags);
void fontColor__FUiii(unsigned color, int a, int b);
int fontPrintWrapCenteredu__FiiPUsiiii(int x, int y, unsigned short* text, int a, int b, int c, int d);
void fontEnd__Fv();
void fontScissor__Fiiii(int a, int b, int c, int d);
int getValue__4itemPQ24item11description(void* cell);
int sprintf(char* out, const char* format, ...);
__SIZE_TYPE__ strlen(const char* text);
char* strstr(const char* haystack, const char* needle);
void fontPrint__FiiPci(int x, int y, char* text, int limit);
void fontPrintCentered__FiiPci(int x, int y, char* text, int limit);
void textureDrawSuperColor__FP8_textureiiiUiiiiicc(void* texture, int x, int y, int mode, unsigned color, int a, int b, int c, int d, char e, char f);
void displayItemStats__FiiiPQ24item11descriptioniii(int player, int x, int y, void* cell, int a, int b, int c);
}

#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_base;
#endif

namespace {
// Pointer words of the image are 32-bit; a host check maps them to offsets in one arena.
inline unsigned char* at(unsigned word) {
#ifdef NORRATH_HOST_ARENA
    return norrath_host_base + word;
#else
    return reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word));
#endif
}
template <class T>
inline T load(const unsigned char* base, int offset) {
    T value;
    __builtin_memcpy(&value, base + offset, sizeof value);
    return value;
}
inline float floatFromBits(unsigned bits) {
    float value;
    __builtin_memcpy(&value, &bits, sizeof value);
    return value;
}
// The inlined commaString__FPc, written the way the machine code reads.
inline void commaText(char* text) {
    int length = static_cast<int>(strlen(text));
    const int commas = (length - 1) / 3;
    const int tail = length + commas + 1;
    int first = length;
    int offset = 0;
    for (int i = 0; i < commas; ++i) {
        const int remainder = first % 3;
        const int half = (first + static_cast<int>(static_cast<unsigned>(first) >> 31)) >> 1;
        first = remainder != 0 ? remainder : half;
        const int from = first + offset;
        if (!(tail < from)) {
            for (int j = tail; j >= from; --j) text[j] = text[j - 1];
        }
        text[first + offset] = ',';
        offset += 4;
    }
}
}

extern "C" void shopDrawInfoBox__FiP12screenStructPQ24item11descriptioniiiii(int pn, void* screenRecord, void* cellPointer, int mode, int shade,
                                                                           int tint, int unused, int width) {
    (void)unused;
    unsigned char* screen = static_cast<unsigned char*>(screenRecord);
    unsigned char* cell = static_cast<unsigned char*>(cellPointer);
    int y;
    int x;
    if (width < 262) {
        y = 229;
        x = load<int>(screen, 0) + 164;
    } else {
        y = 78;
        x = load<int>(screen, 0) + 163;
    }
    float slide;
    if (load<int>(screen, 44) != 0) slide = load<float>(screen, 64) + floatFromBits(0x3dba2e8bu);
    else slide = load<float>(screen, 64) - floatFromBits(0x3dba2e8bu);
    __builtin_memcpy(screen + 64, &slide, 4);
    float t = load<float>(screen, 64);
    if (t < 0.0f) {
        const float zero = 0.0f;
        __builtin_memcpy(screen + 64, &zero, 4);
        t = zero;
    } else if (1.0f < t) {
        const float one = 1.0f;
        __builtin_memcpy(screen + 64, &one, 4);
        t = one;
    }
    y -= static_cast<int>((evalHermite__Ffffff(t, 0.0f, 1.0f, 0.0f, 0.0f) + floatFromBits(0x3c23d70au)) * 96.0f);
    if (load<unsigned>(cell, 0) == 0) return;
    unsigned char* description = at(load<unsigned>(cell, 0));

    unsigned short shortName[256];
    unsigned short measured[256];
    ustrcpy__FPUsT0(shortName, getName__4itemPQ24item11descriptioni(cell, 1));
    int length = ustrlen__FPUs(shortName);
    for (;;) {
        ustrcpy__FPUsT0(measured, shortName);
        if (fontStringSizeu__FP4FontPUsi(smallFont, measured, 1000000) < 285) break;
        --length;
        shortName[length] = '.';
        shortName[length + 1] = '.';
        shortName[length + 2] = '.';
        shortName[length + 3] = 0;
    }

    unsigned short* name = getName__4itemPQ24item11descriptioni(cell, 1);
    const unsigned shadeColor = static_cast<unsigned>(shade) << 16 | static_cast<unsigned>(shade) << 8 | static_cast<unsigned>(shade) | 0x80000000u;
    fontStart__FP4FontiUii(smallFont, 6, shadeColor, -1);
    if (cell[33] != 255) {
        fontColor__FUiii(0x805c2913u, -1, 0);
    } else if ((load<unsigned long long>(cell, 8) & 0x2000000000ull) != 0) {
        fontColor__FUiii(0x80004444u, -1, 0);
    }
    const int lines = fontPrintWrapCenteredu__FiiPUsiiii(x, y, name, 216, -100, 580, 20) / 20;
    itemInfoHeight[pn] = itemInfoHeight[pn] + lines;
    y = y + 22 * lines - 22;
    fontEnd__Fv();
    fontStart__FP4FontiUii(smallFont, 6, shadeColor, -1);
    fontScissor__Fiiii(0, y, 640, 407);
    float price = static_cast<float>(getValue__4itemPQ24item11description(cell));
    if (mode != 0) {
        if (static_cast<signed char>(description[28]) != 8 && strstr(reinterpret_cast<const char*>(at(load<unsigned>(description, 0))), "spear") == nullptr)
            price = price / static_cast<float>(load<short>(cell, 4));
        price = price * floatFromBits(0x3e4cccccu);
    }
    char text[1024];
    sprintf(text, "%d", static_cast<int>(price));
    int offset = 0;
    if (static_cast<signed char>(description[28]) == 8) offset = -19;
    commaText(text);
    const int baseY = y + offset;
    if (static_cast<signed char>(description[29]) == 2 || (load<unsigned long long>(cell, 8) & 0x100ull) == 0) {
        fontPrint__FiiPci(x - 74, baseY + 43, text, 100000);
    } else {
        fontPrint__FiiPci(x - 64, baseY + 64, text, 100000);
    }
    const int count = load<short>(cell, 4);
    if (count >= 2) {
        sprintf(text, "x %d", count);
        fontPrintCentered__FiiPci(x, y + 20, text, 10000);
    }
    fontEnd__Fv();
    const unsigned tintColor = static_cast<unsigned>(tint) << 16 | static_cast<unsigned>(tint) << 8 | static_cast<unsigned>(tint) | 0x80000000u;
    if (static_cast<signed char>(description[29]) == 2 || (load<unsigned long long>(cell, 8) & 0x100ull) == 0) {
        textureDrawSuperColor__FP8_textureiiiUiiiiicc(tinyIcon[7], x - 95, baseY + 47, 6, tintColor, 0, 92, 639, 447, 0, 0);
    } else {
        textureDrawSuperColor__FP8_textureiiiUiiiiicc(tinyIcon[7], x - 85, baseY + 68, 6, tintColor, 0, 92, 639, 447, 0, 0);
    }
    displayItemStats__FiiiPQ24item11descriptioniii(pn, x, y + 23, cell, tint, shade, lines);
}
