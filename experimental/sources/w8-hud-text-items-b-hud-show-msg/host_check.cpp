#include <cstring>

extern "C" {
int frontEndRunning;
void* smallFont;
void hudShowMsg__FPUsN20iii(const unsigned short* title, const unsigned short* body,
                            const unsigned short* footer, int alpha, int width_arg, int flag);
}

enum Kind { START, WRAP, COLOR, CENTER, END, BORDER, LUMP };
struct Event {
    Kind kind;
    int v[8];
    const void* p;
    unsigned u;
};
static Event ev[32];
static int nev;
static int wrap_result[4];
static int wraps;
static char font_token;
static const char *lump_a, *lump_b;
static const char* border_name;

static Event& add(Kind k) {
    Event& e = ev[nev++];
    std::memset(&e, 0, sizeof e);
    e.kind = k;
    return e;
}

extern "C" {
void* lumpFindResource__FPcT0(const char* lump, const char* name) {
    add(LUMP);
    lump_a = lump;
    lump_b = name;
    return &font_token;
}
void fontStart__FP4FontiUii(void* font, int size, unsigned color, int flags) {
    Event& e = add(START);
    e.p = font; e.v[0] = size; e.u = color; e.v[1] = flags;
}
int fontPrintWrapCenteredu__FiiPUsiiii(int x, int y, const unsigned short* text, int a, int b, int c, int d) {
    Event& e = add(WRAP);
    e.p = text; e.v[0] = x; e.v[1] = y; e.v[2] = a; e.v[3] = b; e.v[4] = c; e.v[5] = d;
    return wrap_result[wraps++];
}
void fontColor__FUiii(unsigned color, int a, int b) {
    Event& e = add(COLOR);
    e.u = color; e.v[0] = a; e.v[1] = b;
}
void fontPrintCenteredu__FiiPUsi(int x, int y, const unsigned short* text, int color) {
    Event& e = add(CENTER);
    e.p = text; e.v[0] = x; e.v[1] = y; e.v[2] = color;
}
void fontEnd__Fv() { add(END); }
void textureDrawBorder__FiiiiiUiPcii(int x, int y, int w, int h, int a, unsigned color, const char* name, int b, int c) {
    Event& e = add(BORDER);
    e.v[0] = x; e.v[1] = y; e.v[2] = w; e.v[3] = h; e.v[4] = a; e.u = color; e.v[5] = b; e.v[6] = c;
    border_name = name;
}
}

static unsigned short T[2] = {1, 0}, B[2] = {2, 0}, F[2] = {3, 0};

static void reset(int h0, int h1) {
    nev = 0;
    wraps = 0;
    wrap_result[0] = h0;
    wrap_result[1] = h1;
}

static bool streq(const char* a, const char* b) { return std::strcmp(a, b) == 0; }

int main() {
    // Full message, flag 0, frontend font from the lump.
    frontEndRunning = 1;
    smallFont = nullptr;
    reset(40, 61);
    hudShowMsg__FPUsN20iii(T, B, F, 0x80, 0, 0);
    // events: LUMP START WRAP WRAP COLOR WRAP COLOR WRAP COLOR CENTER END BORDER
    if (nev != 12) return 1;
    if (ev[0].kind != LUMP || !streq(lump_a, "frontend.lmp") || !streq(lump_b, "gamefontsmall.fnt")) return 2;
    if (ev[1].kind != START || ev[1].p != &font_token || ev[1].v[0] != 6 || ev[1].u != (0x80000000u | 0x323c3c) || ev[1].v[1] != -1) return 3;
    if (ev[2].kind != WRAP || ev[2].p != T || ev[2].v[0] != 320 || ev[2].v[1] != 212 || ev[2].v[2] != 470) return 4;
    if (ev[2].v[3] != 100 || ev[2].v[4] != 100 || ev[2].v[5] != 17) return 5;
    if (ev[3].kind != WRAP || ev[3].p != B || ev[3].v[1] != 212) return 6;
    // box height = 40 + 61 + 85 = 186, half 93, text top 132, footer y 280
    if (ev[4].kind != COLOR || ev[4].u != (0x80000000u | 0x1e6464) || ev[4].v[0] != -1 || ev[4].v[1] != 0) return 7;
    if (ev[5].kind != WRAP || ev[5].p != T || ev[5].v[1] != 132 || ev[5].v[2] != 470 || ev[5].v[3] != -100 || ev[5].v[4] != 580 || ev[5].v[5] != 17) return 8;
    if (ev[6].kind != COLOR || ev[6].u != (0x80000000u | 0x323c3c)) return 9;
    if (ev[7].kind != WRAP || ev[7].p != B || ev[7].v[1] != 132 + 40 + 17) return 10;
    if (ev[8].kind != COLOR || ev[8].u != (0x80000000u | 0x3c4646)) return 11;
    if (ev[9].kind != CENTER || ev[9].p != F || ev[9].v[0] != 320 || ev[9].v[1] != 280 || ev[9].v[2] != 1000000) return 12;
    if (ev[10].kind != END) return 13;
    if (ev[11].kind != BORDER || ev[11].v[0] != 320 - 260 || ev[11].v[1] != 220 - 93 || ev[11].v[2] != 520 || ev[11].v[3] != 186) return 14;
    if (ev[11].v[4] != 6 || ev[11].u != (0x80000000u | 0x808080) || !streq(border_name, "frontend.lmp") || ev[11].v[5] != 0 || ev[11].v[6] != 0) return 15;

    // In-game font, explicit width, odd box height, no footer.
    frontEndRunning = 0;
    smallFont = &font_token + 1;
    reset(33, 0);
    hudShowMsg__FPUsN20iii(nullptr, B, nullptr, 0x10, 301, 0);
    // events: START WRAP COLOR WRAP END BORDER ; box height = 0 + 33 + 68 = 101, half 50
    if (nev != 6) return 16;
    if (ev[0].kind != START || ev[0].p != &font_token + 1 || ev[0].u != (0x10000000u | 0x323c3c)) return 17;
    if (ev[1].kind != WRAP || ev[1].v[2] != 251 || ev[1].p != B) return 18;
    if (ev[3].kind != WRAP || ev[3].v[1] != 225 - 50 + 0 + 17) return 20;
    if (ev[5].kind != BORDER || ev[5].v[0] != 320 - 150 || ev[5].v[1] != 220 - 50 || ev[5].v[2] != 301 || ev[5].v[3] != 101) return 21;

    // Flag set: the title is not measured, the middle height is the flag, and the body is centred on 212.
    frontEndRunning = 0;
    reset(77, 0);
    hudShowMsg__FPUsN20iii(T, B, F, 0xff, 400, 30);
    // events: START WRAP(body measure) COLOR WRAP(title) COLOR WRAP(body) COLOR CENTER END BORDER
    if (nev != 10) return 22;
    if (ev[1].kind != WRAP || ev[1].p != B) return 23;
    // box = 0 + 30 + 85 = 115, half 57
    if (ev[3].p != T || ev[3].v[1] != 225 - 57) return 24;
    if (ev[5].p != B || ev[5].v[1] != 212 - 77 / 2) return 25;
    if (ev[7].kind != CENTER || ev[7].v[1] != 57 + 187) return 26;
    if (ev[9].v[3] != 115 || ev[9].v[1] != 220 - 57) return 27;

    // Neither title nor body nor footer: only the frame.
    frontEndRunning = 0;
    reset(0, 0);
    hudShowMsg__FPUsN20iii(nullptr, nullptr, nullptr, 1, 0, 5);
    if (nev != 3 || ev[0].kind != START || ev[1].kind != END || ev[2].kind != BORDER) return 28;
    if (ev[2].v[3] != 5 + 68) return 29;
    return 0;
}
