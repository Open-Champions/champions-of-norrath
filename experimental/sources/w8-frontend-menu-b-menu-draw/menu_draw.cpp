// Observed: menuDraw__FiiP4MenuUiUiP4FontP8_texturei at 0x001934a8 is 2432
// bytes. Arguments: x, y (the menu centre), the menu records, two packed
// colors (idle, highlight), a font, a texture that this body never reads,
// and one integer passed on to queryInventoryItem.
// Returns at once when GLISTSIZE minus ((glistp - gstart) >> 3) is below 3000.
// Records are the 16-byte entries described in menu_run.cpp; word +12 bit 0
// hides an entry. With n visible entries the panel height is 22 * n + 6.
// The terminator's word +4 gives the cursor (bits 0 to 6) and the previous
// cursor (shifted by 8). The highlight eases with evalHermite(t, 0, 1, .5, .5)
// where t is the same file-local float menuRun advances. The selected row is
// kept between y 20 and 400 by shifting all rows in steps of 22 (a shift
// resets t to 1.0). Text is built per entry: a label containing "menu_" goes
// through textGetString, except "menu_recall", which feeds
// queryInventoryItem("recallpotion", last) as a float to usprintf; other
// labels are narrow strings copied to the buffer. A kind 3 entry appends
// " : " and textGetString of "on" or "off" from the value word; kind 4 and 8
// reserve 156 width width and shift the label left by 23. Entries 5 and 8
// print at half brightness, 6 and 7 use fixed colors; the cursor and previous
// entries blend idle to highlight by t. After the text, kind 4 and 8 draw a
// bar from the float value (kind 8 also a 2-pixel marker at 116 * commThresh).
// The backing plate is a smooth border sized by the eased width/row and,
// unless frontEndRunning is set, a second border around all rows.
// Inferred: kind 8 is a threshold meter because it reads commThresh.
// Unknown: the identity of the word 116 at 0x00484514, whether the unused
// texture argument mattered, and the names of the border helpers' fields.
extern "C" {
void* lumpFindResource__FPcT0(char* lump, char* name);
float evalHermite__Ffffff(float t, float p0, float p1, float m0, float m1);
unsigned colorScale__FUif(unsigned color, float scale);
unsigned lerpColor__FUiUif(unsigned from, unsigned to, float amount);
void fontStart__FP4FontiUii(void* font, int mode, unsigned color, int last);
void fontEnd__Fv();
void fontColor__FUiii(unsigned color, int second, int third);
int fontStringSizeu__FP4FontPUsi(void* font, unsigned short* text, int scale);
void fontPrintCenteredu__FiiPUsi(int x, int y, unsigned short* text, int scale);
char* strstr(const char* haystack, const char* needle);
int strcmp(const char* left, const char* right);
unsigned short* textGetString__FPc(char* key);
int queryInventoryItem__FPci(char* name, int owner);
void usprintf__FPUsT0ffffffff(unsigned short* dest, unsigned short* format, float a, float b, float c,
                              float d, float e, float f, float g, float h);
void ustrcpy__FPUsT0(unsigned short* dest, unsigned short* src);
void ustrcpy__FPUsPc(unsigned short* dest, char* src);
void ustrcat__FPUsPc(unsigned short* dest, char* src);
void ustrcat__FPUsT0(unsigned short* dest, unsigned short* src);
void rectangleDraw__FiiiiiUi(int x0, int y0, int x1, int y1, int mode, unsigned color);
void textureSmoothBorder__FiiiiiUiiii(int x, int y, int width, int height, int mode, unsigned color,
                                      int a, int b, int c);
void textureDrawBorder__FiiiiiUiPcii(int x, int y, int width, int height, int mode, unsigned color,
                                     char* texture, int a, int b);
extern unsigned gstart;
extern unsigned glistp;
extern unsigned GLISTSIZE;
extern float norrath_menu_local_t_004844e8;
extern int norrath_menu_word_00484514;
extern float coreInterfaceBrightness[];
extern float commThresh;
extern int frontEndRunning;
}

namespace {
#ifdef NORRATH_HOST_ARENA
extern "C" void* norrath_host_pointer(unsigned word);
void* pointer_of(unsigned word) { return norrath_host_pointer(word); }
#else
void* pointer_of(unsigned word) { return reinterpret_cast<void*>(word); }
#endif

unsigned load_word(const unsigned char* at) {
    return static_cast<unsigned>(at[0]) | (static_cast<unsigned>(at[1]) << 8) |
           (static_cast<unsigned>(at[2]) << 16) | (static_cast<unsigned>(at[3]) << 24);
}

int load_int(const unsigned char* at) { return static_cast<int>(load_word(at)); }

int round_half_away(float value) {
    if (0.0f <= value) return static_cast<int>(value + 0.5f);
    return static_cast<int>(value - 0.5f);
}

void reset_color(unsigned base) { fontColor__FUiii(colorScale__FUif(base, coreInterfaceBrightness[0]), -1, 0); }
}

extern "C" void menuDraw__FiiP4MenuUiUiP4FontP8_texturei(int x, int y, unsigned char* menu, unsigned idle,
                                                         unsigned highlight, void* font, void* texture,
                                                         int owner) {
    (void)texture;
    if (static_cast<int>(GLISTSIZE) - ((static_cast<int>(glistp) - static_cast<int>(gstart)) >> 3) < 3000) return;

    int count = 0;
    int terminator = 0;
    if (load_word(menu) != 0) {
        const unsigned char* record = menu;
        int offset = 0;
        do {
            unsigned flags = load_word(record + 12);
            record += 16;
            offset += 16;
            terminator = offset;
            count += (flags ^ 1u) & 1u;
        } while (load_word(record) != 0);
    }

    int state = load_int(menu + terminator + 4);
    int cursor = state & 0x7f;
    int previous = state >> 8;
    float eased = evalHermite__Ffffff(norrath_menu_local_t_004844e8, 0.0f, 1.0f, 0.5f, 0.5f);
    float widthPrevious = 0.0f;
    float widthCursor = 0.0f;
    int height = count * 22 + 6;
    int shift = 0;
    int center = cursor * 22 - (height >> 1) + 237;
    if (center < 20) {
        do {
            shift += 22;
            center += 22;
        } while (center < 20);
    }
    if (cursor * 22 - (height >> 1) + shift + 237 >= 401) {
        int moved = cursor * 22 - (height >> 1) + shift + 237;
        do {
            moved -= 22;
            shift -= 22;
        } while (moved >= 401);
    }
    if (shift != 0) norrath_menu_local_t_004844e8 = 1.0f;

    int maxWidth = 0;
    int yPrevious = 0;
    int yCursor = 0;
    fontStart__FP4FontiUii(font, 7, colorScale__FUif(idle, coreInterfaceBrightness[0]), -1);

    if (load_word(menu) != 0) {
        const unsigned char* record = menu;
        int index = 0;
        int row = 0;
        do {
            if ((load_word(record + 12) & 1u) == 0) {
                int textY = y - (height >> 1) + row + shift - 3;
                row += 22;
                int textX = x;
                unsigned short text[128];
                char* label = static_cast<char*>(pointer_of(load_word(record)));
                if (strstr(label, "menu_") != 0) {
                    if (strcmp(label, "menu_recall") == 0) {
                        unsigned short* format = textGetString__FPc(const_cast<char*>("menu_recall"));
                        int have = queryInventoryItem__FPci(const_cast<char*>("recallpotion"), owner);
                        usprintf__FPUsT0ffffffff(text, format, static_cast<float>(have), 0.0f, 0.0f, 0.0f,
                                                 0.0f, 0.0f, 0.0f, 0.0f);
                    } else {
                        ustrcpy__FPUsT0(text, textGetString__FPc(label));
                    }
                } else {
                    ustrcpy__FPUsPc(text, label);
                }
                int kind = load_int(record + 4);
                int width = 0;
                if (kind == 4 || kind == 8) {
                    int measured = fontStringSizeu__FP4FontPUsi(font, text, 1000000);
                    width = 156;
                    textX = x - measured / 2 - 23;
                } else if (kind == 3) {
                    ustrcat__FPUsPc(text, const_cast<char*>(" : "));
                    int* value = static_cast<int*>(pointer_of(load_word(record + 8)));
                    unsigned short* word = *value != 0 ? textGetString__FPc(const_cast<char*>("on"))
                                                       : textGetString__FPc(const_cast<char*>("off"));
                    ustrcat__FPUsT0(text, word);
                    width = 2;
                }
                width += fontStringSizeu__FP4FontPUsi(font, text, 1000000) + 40;
                if (maxWidth < width) maxWidth = width;
                if (index == previous) {
                    widthPrevious = static_cast<float>(width);
                    yPrevious = textY;
                }
                if (index == cursor) {
                    widthCursor = static_cast<float>(width);
                    yCursor = textY;
                }
                bool marked = index == cursor || index == previous;
                if (marked) {
                    if (index == cursor) {
                        unsigned color = lerpColor__FUiUif(idle, highlight, norrath_menu_local_t_004844e8);
                        fontColor__FUiii(colorScale__FUif(color, coreInterfaceBrightness[0]), -1, 0);
                    } else {
                        unsigned color = lerpColor__FUiUif(idle, highlight, 1.0f - norrath_menu_local_t_004844e8);
                        fontColor__FUiii(colorScale__FUif(color, coreInterfaceBrightness[0]), -1, 0);
                    }
                }
                if (kind == 5) {
                    fontColor__FUiii(colorScale__FUif(idle, coreInterfaceBrightness[0] * 0.5f), -1, 0);
                }
                if (kind == 8) {
                    fontColor__FUiii(colorScale__FUif(idle, coreInterfaceBrightness[0] * 0.5f), -1, 0);
                }
                if (kind == 6) {
                    fontColor__FUiii(colorScale__FUif(0x80202040u, coreInterfaceBrightness[0]), -1, 0);
                }
                if (kind == 7) {
                    fontColor__FUiii(colorScale__FUif(0x80402020u, coreInterfaceBrightness[0]), -1, 0);
                }
                fontPrintCenteredu__FiiPUsi(textX, textY, text, 1000000);
                if (marked || kind == 5 || kind == 6 || kind == 7 || kind == 8) reset_color(idle);
            }
            record += 16;
            index += 1;
        } while (load_word(record) != 0);
    }
    fontEnd__Fv();

    if (load_word(menu) != 0) {
        const unsigned char* record = menu;
        int rowY = y - (height >> 1) + shift;
        do {
            if ((load_word(record + 12) & 1u) == 0) {
                int top = rowY + 6;
                int bottom = rowY + 15;
                int kind = load_int(record + 4);
                int left;
                int edge;
                bool draw = false;
                if (kind == 4) {
                    left = x - 18;
                    edge = x - 20;
                    draw = true;
                } else if (kind == 8) {
                    left = x - 18;
                    edge = x - 20;
                    float marker = static_cast<float>(left) +
                                   static_cast<float>(norrath_menu_word_00484514) * commThresh;
                    int at = static_cast<int>(marker);
                    rectangleDraw__FiiiiiUi(at - 1, top, at + 1, bottom, 7, 0x80808080u);
                    draw = true;
                }
                if (draw) {
                    float* value = static_cast<float*>(pointer_of(load_word(record + 8)));
                    float length = static_cast<float>((x + 120) - edge - 4);
                    rectangleDraw__FiiiiiUi(left, top, static_cast<int>(static_cast<float>(left) + length * *value),
                                            bottom, 7, 0x800c2b40u);
                    rectangleDraw__FiiiiiUi(left, top, x + 118, bottom, 7, 0x80000000u);
                    rectangleDraw__FiiiiiUi(edge, rowY + 4, x + 120, rowY + 17, 7, 0x80808080u);
                }
                rowY += 22;
            }
            record += 16;
        } while (load_word(record) != 0);
    }

    int plateWidth = round_half_away(widthPrevious + (widthCursor - widthPrevious) * eased);
    float fromY = static_cast<float>(yPrevious);
    int plateY = round_half_away(fromY + (static_cast<float>(yCursor) - fromY) * eased);
    textureSmoothBorder__FiiiiiUiiii(320 - plateWidth / 2, plateY, plateWidth, 32 + (plateY & 1), 7,
                                     0x300000ffu, 0, 0, 0);
    if (frontEndRunning == 0) {
        int top = y - (height >> 1) - 10;
        if (top < 0) top = 0;
        int tall = height + 20;
        if (tall > 448) tall = 448;
        textureDrawBorder__FiiiiiUiPcii(313 - maxWidth / 2, top, maxWidth + 14, tall, 7, 0x80808080u, 0, 0, 0);
    }
}
