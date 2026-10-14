// Host check for subDrawLoop__FPFv_i. Not an EE plan input.
#include "sub_draw_loop.cpp"

#include <string>

extern "C" {
unsigned frameNm = 7u;
unsigned char supersampleMode = 7u;
unsigned coreClearColor = 0x80402010u;
unsigned gameControlPaused = 0u;
unsigned char pad[128];
}

static std::string trace;
static int calls = 0;
static int stop_at = 2;

extern "C" void engineSetDisplayDisable__Fi(int value) { trace += value == 2 ? "D" : value == 1 ? "d" : "?"; }
extern "C" void engineFrameStart__FUib(unsigned color, int second) {
    trace += "S";
    if (color != 0x80402010u || second != 0) trace += "?";
}
extern "C" void padProcess__Fv() { trace += "P"; }
extern "C" void cdProcess__Fv() { trace += "C"; }
extern "C" void engineFrameEnd__Fb(int value) { trace += value == 0 ? "E" : "?"; trace += std::to_string(frameNm); }
extern "C" void soundFrame__Fv() { trace += "F"; }
static int cb() { trace += "X"; return ++calls >= stop_at; }
static int cb_hold() { trace += "H"; return ++calls == 1 ? 1 : 0; }
static void set(unsigned char* p, unsigned v) { for (int i = 0; i < 4; ++i) p[i] = (unsigned char)(v >> (8 * i)); }

int main() {
    // Two passes; no gameClear/stream/lump calls at the end.
    subDrawLoop__FPFv_i(cb);
    if (trace != "DSPXCE1FSPXCE2Fd") return 1;
    if (supersampleMode != 1 || frameNm != 2) return 2;

    // The result is kept: a callback that returns nonzero once ends right then.
    trace.clear(); calls = 0;
    subDrawLoop__FPFv_i(cb_hold);
    if (trace != "DSPHCE1Fd") return 3;

    // Pause counter: decremented only when both masked words are zero.
    trace.clear(); calls = 0; stop_at = 1;
    gameControlPaused = 3;
    set(pad + 8, 0x00f00000u); set(pad + 96, 0);   // masked away by 0xff0fffff
    subDrawLoop__FPFv_i(cb);
    if (gameControlPaused != 2) return 4;
    calls = 0; set(pad + 8, 0x00000001u);
    subDrawLoop__FPFv_i(cb);
    if (gameControlPaused != 2) return 5;
    calls = 0; set(pad + 8, 0); set(pad + 96, 0x80000000u);
    subDrawLoop__FPFv_i(cb);
    if (gameControlPaused != 2) return 6;
    calls = 0; set(pad + 96, 0);
    gameControlPaused = 0;
    subDrawLoop__FPFv_i(cb);
    if (gameControlPaused != 0) return 7;
    return 0;
}
