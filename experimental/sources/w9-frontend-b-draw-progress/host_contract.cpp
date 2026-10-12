#include "draw_progress.cpp"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>

float progress;
int flareAlpha, asyncMode, browserOverwrite;
static std::string log_text;
static std::string key_seen;
static int rects, show_alpha, show_extra, slot_ret = 4;
static unsigned rect_color[2];
static int rect_args[2][5];
static unsigned short fmt_obj[4];
static unsigned short* shown;
static float uargs[8];
static unsigned short* uout;
static unsigned short* ufmt;

extern "C" void rectangleDraw__FiiiiiUi(int a, int b, int c, int d, int e, unsigned color) {
    log_text += "R";
    rect_args[rects][0] = a; rect_args[rects][1] = b; rect_args[rects][2] = c;
    rect_args[rects][3] = d; rect_args[rects][4] = e;
    rect_color[rects++] = color;
}
extern "C" unsigned short* textGetString__FPc(char* k) { log_text += "T"; key_seen = k; return fmt_obj; }
extern "C" int getSlotName__FiiPc(int a, int b, char* c) {
    log_text += "S";
    if (a != -1 || b != -1 || c != nullptr) std::abort();
    return slot_ret;
}
extern "C" void usprintf__FPUsT0ffffffff(unsigned short* o, unsigned short* f, float a, float b,
                                         float c, float d, float e, float g, float h, float i) {
    log_text += "U"; uout = o; ufmt = f;
    float v[8] = {a, b, c, d, e, g, h, i};
    for (int k = 0; k < 8; ++k) uargs[k] = v[k];
}
extern "C" void hudShowMsg__FPUsN20iii(unsigned short* t, unsigned short* b, unsigned short* f,
                                       int a, int bb, int c) {
    log_text += "H"; shown = b; show_alpha = a;
    if (t || f || bb || c) std::abort();
    show_extra = 1;
}

static void run(int alpha, float prog, int mode, int over) {
    log_text.clear(); rects = 0; flareAlpha = alpha; progress = prog; asyncMode = mode;
    browserOverwrite = over; show_extra = 0;
    drawProgress__Fv();
}

int main() {
    run(128, 100.9f, 3, 1);
    if (log_text != "RRTSUH") return 1;
    if (key_seen != "memcardLoadGame") return 2;
    if (rect_args[0][0] != 119 || rect_args[0][1] != 261 || rect_args[0][2] != 219 ||
        rect_args[0][3] != 271 || rect_args[0][4] != 6) return 3;
    if (rect_color[0] != ((128u << 24) | 0x276b7fu)) return 4;
    if (rect_args[1][0] != 119 || rect_args[1][1] != 259 || rect_args[1][2] != 521 ||
        rect_args[1][3] != 273 || rect_args[1][4] != 6) return 5;
    if (rect_color[1] != ((64u << 24) | 0x141414u)) return 6;
    if (uargs[0] != 137.0f || uargs[1] != 4.0f || uargs[2] != 137.0f) return 7;
    for (int k = 3; k < 8; ++k) if (uargs[k] != 0.0f) return 8;
    if (ufmt != fmt_obj || shown != uout || show_alpha != 128) return 9;
    run(77, 0.0f, 1, 1);
    if (key_seen != "Memcard2OverwriteMsg" || rect_args[0][2] != 119) return 10;
    if (rect_color[0] != ((77u << 24) | 0x276b7fu) || rect_color[1] != ((38u << 24) | 0x141414u)) return 11;
    run(0, 402.0f, 0, 0);
    if (key_seen != "Memcard3SaveMsg" || rect_args[0][2] != 521) return 12;
    if ((rect_color[0] >> 24) != 0 || (rect_color[1] >> 24) != 0) return 13;
    run(5, 1.0f, 3, 0);
    if (key_seen != "memcardLoadGame") return 14;
    run(255, 1.0f, 2, 0);
    if (rect_color[0] != ((255u << 24) | 0x276b7fu) || rect_color[1] != ((127u << 24) | 0x141414u)) return 15;
    std::puts("draw progress passed");
    return 0;
}
