#include <cstdio>
#include <cstring>
struct Font { int id; };
extern "C" {
signed char powerOffMode, playerSelectCount, netNetworkMode;
int frontEndRunning;
Font fontA{1}, fontB{2};
Font* mediumFont = &fontA;
char levelName[322];
unsigned char hudNoController;
}
#include "no_controllers.cpp"
extern "C" {
norrath_pad_record pad[4];
unsigned short key_text[4] = {1, 2, 3, 0};
const char* key_seen; float fmt_arg; unsigned short* fmt_dest; unsigned short* fmt_format; float fmt_rest;
int events[16], ev;
Font* started; int start_mode; unsigned start_color; int start_last;
int pw_args[7]; unsigned short* pw_text;
int tb[9]; unsigned tb_color; const char* tb_lump;
unsigned short* textGetString__FPc(const char* key) { key_seen = key; events[ev++] = 1; return key_text; }
void usprintf__FPUsT0ffffffff(unsigned short* d, unsigned short* f, float a, float b, float c, float dd, float e,
                              float ff, float g, float h) {
    events[ev++] = 2; fmt_dest = d; fmt_format = f; fmt_arg = a; fmt_rest = b + c + dd + e + ff + g + h;
}
Font* lumpFindResource__FPcT0(const char* l, const char* n) {
    events[ev++] = 3;
    if (std::strcmp(l, "frontend.lmp") || std::strcmp(n, "gamefontsmall.fnt")) return 0;
    return &fontB;
}
void fontStart__FP4FontiUii(Font* f, int m, unsigned c, int l) { events[ev++] = 4; started = f; start_mode = m; start_color = c; start_last = l; }
int fontPrintWrapCenteredu__FiiPUsiiii(int x, int y, unsigned short* t, int w, int a, int b, int c) {
    events[ev++] = 5; pw_args[0] = x; pw_args[1] = y; pw_args[2] = w; pw_args[3] = a; pw_args[4] = b; pw_args[5] = c; pw_text = t; return 77;
}
void fontEnd__Fv() { events[ev++] = 6; }
void textureDrawBorder__FiiiiiUiPcii(int x, int y, int w, int h, int a, unsigned c, const char* l, int b, int d) {
    events[ev++] = 7; tb[0] = x; tb[1] = y; tb[2] = w; tb[3] = h; tb[4] = a; tb_color = c; tb_lump = l; tb[5] = b; tb[6] = d;
}
}
static void reset() {
    ev = 0; key_seen = 0; started = 0; tb_lump = 0; hudNoController = 9;
    powerOffMode = 0; netNetworkMode = 0; frontEndRunning = 0; playerSelectCount = 0;
    std::memset(levelName, 0, sizeof levelName); std::strcpy(levelName + 81, "other");
    for (auto& p : pad) { p.word4 = 1; }
}
int main() {
    reset(); powerOffMode = 1; pad[0].word4 = 0; hudRunNoControllers__Fi(4);
    if (ev || hudNoController != 9) return 1;
    reset(); hudRunNoControllers__Fi(4);                  // all connected
    if (ev || hudNoController != 0) return 2;
    reset(); pad[1].word4 = 0; pad[3].word4 = 0; hudRunNoControllers__Fi(4);   // last zero wins
    if (hudNoController != 1 || fmt_arg != static_cast<float>(reinterpret_cast<__INTPTR_TYPE__>(&pad[3]))) return 3;
    reset(); pad[3].word4 = 0; hudRunNoControllers__Fi(3);                     // beyond count ignored
    if (ev || hudNoController != 0) return 4;
    reset(); pad[2].word4 = 0; netNetworkMode = 1; hudRunNoControllers__Fi(4); // net: only element 0
    if (ev || hudNoController != 0) return 5;
    reset(); pad[0].word4 = 0; netNetworkMode = 1; hudRunNoControllers__Fi(4);
    if (hudNoController != 1 || fmt_arg != static_cast<float>(reinterpret_cast<__INTPTR_TYPE__>(&pad[0]))) return 6;
    reset(); std::strcpy(levelName + 81, "kelsel2"); playerSelectCount = 2; pad[2].word4 = 0; pad[1].word4 = 0;
    hudRunNoControllers__Fi(4);                                               // count replaced by 2
    if (hudNoController != 1 || fmt_arg != static_cast<float>(reinterpret_cast<__INTPTR_TYPE__>(&pad[1]))) return 7;
    reset(); std::strcpy(levelName + 81, "kelsel2"); playerSelectCount = 1; pad[2].word4 = 0;
    hudRunNoControllers__Fi(4);
    if (ev || hudNoController != 0) return 8;
    reset(); pad[0].word4 = 0; hudRunNoControllers__Fi(1);                    // not in front end
    int want[] = {1, 2, 4, 5, 6, 7};
    if (ev != 6) return 9;
    for (int i = 0; i < 6; ++i) if (events[i] != want[i]) return 10;
    if (std::strcmp(key_seen, "controllerOutMsg") || fmt_format != key_text || fmt_dest != pw_text || fmt_rest != 0.0f) return 11;
    if (started != &fontA || start_mode != 7 || start_color != 0x801e6464u || start_last != -1) return 12;
    if (pw_args[0] != 320 || pw_args[1] != 200 || pw_args[2] != 400 || pw_args[3] != -100 || pw_args[4] != 580 || pw_args[5] != 21) return 13;
    if (tb[0] != 112 || tb[1] != 192 || tb[2] != 416 || tb[3] != 77 + 21 || tb[4] != 7 || tb_color != 0x80808080u ||
        tb_lump != 0 || tb[5] != 0 || tb[6] != 0) return 14;
    reset(); pad[0].word4 = 0; frontEndRunning = 1; hudRunNoControllers__Fi(1);   // in front end
    int want2[] = {1, 2, 3, 4, 5, 6, 7};
    if (ev != 7) return 15;
    for (int i = 0; i < 7; ++i) if (events[i] != want2[i]) return 16;
    if (started != &fontB || !tb_lump || std::strcmp(tb_lump, "frontend.lmp")) return 17;
    std::puts("hudRunNoControllers__Fi check passed");
    return 0;
}
