// Not an EE plan input. Build: c++ -std=c++17 -Wall -Wextra -Werror -ffp-contract=off -fno-builtin -DW12_HUD_HOST host_check.cpp
// Records are byte arrays with the observed offsets; pointer words (the object word in the record, the
// spell pointer word) are 32-bit handles into small host tables. Every callee appends to a trace.
#include "run_hud.cpp"
#include <cmath>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

extern "C" {
int gameNoDraw, shopRunning, glistp, gstart, GLISTSIZE, frameNm, nmPlayers, commNameMask;
signed char hudNoDraw;
void* gameCam;
void* smallFont = reinterpret_cast<void*>(0x31);
alignas(8) unsigned char player[4 * 23984];
unsigned gameFlags[4];
int experienceTable[16];
int levelUpFade[4], iconAlpha[28], fadeIconY[28], fadeTimer[28], healthPotionsLeft[4], manaPotionsLeft[4];
void* globalIcons[40];
void* crystalTex[4];
}
alignas(8) static unsigned char cam[64];
alignas(8) static unsigned char objectBlock[1400];
alignas(8) static unsigned char spellBlock[16];
alignas(8) static unsigned char entryBlock[16];
alignas(8) static unsigned char textureBlocks[8][16];
static std::string trace;
static int connected = 1, localAnswer = 1, fontWidth = 10, markerColor = 0x80102030;
static std::string hex(unsigned v) { char b[16]; std::snprintf(b, sizeof b, "%x", v); return b; }
static std::string num(long v) { return std::to_string(v); }
static std::string tex(void* t) {
    for (int i = 0; i < 8; ++i) if (t == textureBlocks[i]) return "T" + num(i);
    for (int i = 0; i < 40; ++i) if (t == globalIcons[i]) return "I" + num(i);
    for (int i = 0; i < 4; ++i) if (t == crystalTex[i]) return "C" + num(i);
    return "?";
}
extern "C" {
int netPlayerIsConnected__Fi(int p) { trace += "conn" + num(p) + ";"; return connected; }
int netPlayerIsLocal__Fi(int p) { trace += "local" + num(p) + ";"; return localAnswer; }
void* lumpLoad__FPc(const char* n) { trace += std::string("load:") + n + ";"; return cam; }
void* lumpFind__FPvPc(void* l, const char* n) { trace += std::string("find") + (l == cam ? "" : "?") + ":" + n + ";"; return entryBlock; }
void* lumpFindResource__FPcT0(char* l, char* n) { trace += std::string("findres:") + l + "," + n + ";"; return entryBlock; }
int sprintf(char* out, const char* format, ...) {
    __builtin_va_list ap;
    __builtin_va_start(ap, format);
    std::string f = format, r;
    for (size_t i = 0; i < f.size(); ++i) {
        if (f[i] != '%') { r += f[i]; continue; }
        size_t j = i + 1;
        int width = 0;
        while (j < f.size() && f[j] >= '0' && f[j] <= '9') width = width * 10 + (f[j++] - '0');
        if (f[j] == 'd') { char b[32]; std::snprintf(b, sizeof b, "%0*d", width, __builtin_va_arg(ap, int)); r += b; }
        else if (f[j] == 's') r += __builtin_va_arg(ap, const char*);
        i = j;
    }
    __builtin_va_end(ap);
    std::strcpy(out, r.c_str());
    trace += "fmt[" + r + "];";
    return static_cast<int>(r.size());
}
unsigned long strlen(const char* t) { unsigned long n = 0; while (t[n]) ++n; return n; }
void textureDrawSuperColor__FP8_textureiiiUiiiiicc(void* t, int x, int y, int m, unsigned c, int a, int b, int d, int e, char f, char g) {
    trace += "super(" + tex(t) + "," + num(x) + "," + num(y) + "," + num(m) + "," + hex(c) + "," + num(a) + "," + num(b) + "," + num(d) + "," + num(e) + "," + num(f) + "," + num(g) + ");";
}
void textureDrawColor__FP8_textureiiiUi(void* t, int x, int y, int m, unsigned c) {
    trace += "texc(" + tex(t) + "," + num(x) + "," + num(y) + "," + num(m) + "," + hex(c) + ");";
}
void rectangleDraw__FiiiiiUi(int a, int b, int c, int d, int e, unsigned f) {
    trace += "rect(" + num(a) + "," + num(b) + "," + num(c) + "," + num(d) + "," + num(e) + "," + hex(f) + ");";
}
float isin__Fi(int a) { trace += "isin" + num(a) + ";"; return static_cast<float>((a >> 10) % 33 - 16) / 16.0f; }
void fontStart__FP4FontiUii(void*, int s, unsigned c, int f) { trace += "fstart(" + num(s) + "," + hex(c) + "," + num(f) + ");"; }
void fontEnd__Fv() { trace += "fend;"; }
void fontColor__FUiii(unsigned c, int a, int b) { trace += "fcolor(" + hex(c) + "," + num(a) + "," + num(b) + ");"; }
int fontStringSize__FP4FontPci(void*, char* t, int limit) { (void)limit; return fontWidth * static_cast<int>(::strlen(t)); }
void fontPrintCentered__FiiPci(int x, int y, char* t, int l) { trace += "centered(" + num(x) + "," + num(y) + "," + t + "," + num(l) + ");"; }
void fontPrint__FiiPci(int x, int y, char* t, int l) { trace += "print(" + num(x) + "," + num(y) + "," + t + "," + num(l) + ");"; }
unsigned playerGetMarkerColor__Fii(int p, int a) { trace += "marker(" + num(p) + "," + num(a) + ");"; return static_cast<unsigned>(markerColor); }
void __11ResourceMapPcT1(void* s, char* a, char* b) { trace += std::string("rmap:") + (s == levelUpIcon ? "" : "?") + a + "," + b + ";"; }
void drawNewestQuickSpellMenu__Fiiicl(int p, int a, int b, char c, long d) { trace += "quick(" + num(p) + "," + num(a) + "," + num(b) + "," + num(c) + "," + num(d) + ");"; }
}
static int bad;
static void expect(const char* what, const std::string& got, const std::string& want) {
    if (got != want) { std::printf("FAIL %s\n got  %s\n want %s\n", what, got.c_str(), want.c_str()); bad = 1; }
}
static void expectFrom(const char* what, const std::string& marker, const std::string& want) {
    const size_t p = trace.find(marker);
    if (p == std::string::npos || trace.compare(p, want.size(), want) != 0) {
        std::printf("FAIL %s\n got  %s\n want %s\n", what, p == std::string::npos ? "(marker missing)" : trace.substr(p, want.size() + 12).c_str(), want.c_str());
        bad = 1;
    }
}
static void setf(unsigned char* b, int off, float v) { std::memcpy(b + off, &v, 4); }
static void setw(unsigned char* b, int off, int v) { std::memcpy(b + off, &v, 4); }
static unsigned handleObject = 0x100, handleSpell = 0x200;
__UINTPTR_TYPE__ hostWordAddress(unsigned word) {
    return word == handleObject ? reinterpret_cast<__UINTPTR_TYPE__>(objectBlock) : word == handleSpell ? reinterpret_cast<__UINTPTR_TYPE__>(spellBlock) : 0;
}
static unsigned char* recordOf(int pn) { return player + pn * 23984; }
static void reset(int pn) {
    trace.clear();
    std::memset(player, 0, sizeof player); std::memset(objectBlock, 0, sizeof objectBlock); std::memset(cam, 0, sizeof cam); std::memset(spellBlock, 0, sizeof spellBlock);
    gameCam = cam; setw(cam, 52, 1);
    gameNoDraw = hudNoDraw = shopRunning = 0; glistp = gstart = 0; GLISTSIZE = 10000; frameNm = 4; nmPlayers = 1; commNameMask = 0;
    connected = 1; localAnswer = 1; fontWidth = 10; markerColor = static_cast<int>(0x80102030u);
    for (int i = 0; i < 4; ++i) { gameFlags[i] = 0; levelUpFade[i] = 0; healthPotionsLeft[i] = 1; manaPotionsLeft[i] = 2; }
    for (int i = 0; i < 28; ++i) { iconAlpha[i] = 0; fadeIconY[i] = 3; fadeTimer[i] = 0; }
    for (int i = 0; i < 16; ++i) experienceTable[i] = 100 * i * i;
    for (int i = 0; i < 40; ++i) globalIcons[i] = reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(0x5000 + i));
    std::memset(textureBlocks, 0, sizeof textureBlocks); std::memset(entryBlock, 0, sizeof entryBlock);
    for (int i = 0; i < 4; ++i) crystalTex[i] = textureBlocks[4 + i];
    void* t0 = textureBlocks[0]; std::memcpy(entryBlock + 4, &t0, sizeof t0);
    levelUpIconReady = 0;
    unsigned char* rec = recordOf(pn);
    setw(rec, 21872, static_cast<int>(handleObject));
    setf(rec, 172, 50.0f); setf(rec, 176, 100.0f); setf(rec, 200, 100.0f);
    rec[88] = 2; rec[89] = 5; setw(rec, 92, 250);
    std::strcpy(reinterpret_cast<char*>(rec), "Bob");
    for (int i = 0; i < 4; ++i) { short w = static_cast<short>(40 + 10 * i); std::memcpy(textureBlocks[4 + i], &w, 2); }
    { short w = 90; std::memcpy(textureBlocks[0], &w, 2); }
}
static const char* kBaseline =
    "conn1;load:hud.lmp;fmt[bar_red01.tex];find:bar_red01.tex;super(T0,493,46,6,80808080,493,24,542,124,0,0);"
    "fmt[bar_green01.tex];find:bar_green01.tex;super(T0,493,56,6,80808080,493,24,542,124,0,0);"
    "fmt[bar_blue01.tex];find:bar_blue01.tex;super(T0,493,63,6,80acacac,493,24,493,124,0,0);isin4096;"
    "fstart(5,80323c3c,-1);fcolor(80293333,-1,0);fmt[Bob];fcolor(80323c3c,-1,0);centered(551,8,Bob,10000);fend;"
    "fmt[hud_statusbar00.tex];fstart(6,80323c3c,-1);fcolor(80293333,-1,0);fmt[5];centered(607,43,5,10000);fend;find:hud_statusbar00.tex;"
    "rmap:icons.lmp,levelup.tex;findres:hud.lmp,hud_statusbar00glow.tex;super(?,482,26,6,4555555,0,0,1000,1000,0,0);"
    "super(T0,482,26,6,80808080,0,0,1000,1000,0,0);isin4096;quick(1,0,0,0,0);";
int main() {
    // Early exits.
    reset(1); gameNoDraw = 1; runHUD__Fi(1); expect("gameNoDraw", trace, "");
    reset(1); hudNoDraw = -1; runHUD__Fi(1); expect("hudNoDraw", trace, "");
    reset(1); shopRunning = 1; runHUD__Fi(1); expect("shop", trace, "");
    reset(1); setw(cam, 52, 0); runHUD__Fi(1); expect("camera word", trace, "");
    reset(1); connected = 0; runHUD__Fi(1); expect("not connected", trace, "conn1;");
    reset(1); setw(recordOf(1), 21872, 0); runHUD__Fi(1); expect("no object", trace, "conn1;load:hud.lmp;");
    reset(1); setw(objectBlock, 1328, 1); runHUD__Fi(1); expect("object word", trace, "conn1;load:hud.lmp;");
    reset(1); GLISTSIZE = 3000 + 800; glistp = 800 * 8; runHUD__Fi(1); expect("list exactly 3000 free", trace.substr(0, 20), "conn1;load:hud.lmp;f");
    reset(1); GLISTSIZE = 3000 + 800 - 1; glistp = 800 * 8; runHUD__Fi(1); expect("list 2999 free", trace, "conn1;load:hud.lmp;");
    // Baseline panel.
    reset(1); runHUD__Fi(1); expect("baseline", trace, kBaseline);
    expect("level glow state", num(levelUpFade[1]) + num(levelUpIconReady), "41");
    // The panel flag raises the bar alpha to 255 for one frame and clears.
    reset(1); gameFlags[1] = 3; runHUD__Fi(1);
    expectFrom("flag colour", "super", "super(T0,493,46,6,80ffffff,493,24,542,124,0,0);fmt[b");
    expect("flag cleared", num(gameFlags[1]), "2");
    // Bar extensions: 50 + 20 of 100 gives back 69, front 49: rectangle from x + 49 to x0 + 20 + 57 for player 1.
    reset(1); setf(recordOf(1), 180, 20.0f); runHUD__Fi(1);
    expectFrom("extension", "rect", "rect(542,47,562,51,6,80000060);fmt[bar_green");
    reset(1); setf(recordOf(1), 196, 40.0f); setf(recordOf(1), 200, 80.0f); setf(recordOf(1), 204, 10.0f); runHUD__Fi(1);
    expectFrom("second extension", "rect", "rect(542,66,554,70,6,805a4632);fmt[bar_green");
    // Level bar, hit points and the blue bar follow their own fractions; level 0 skips the green bar.
    reset(1); recordOf(1)[88] = 0; runHUD__Fi(1); expect("level zero", num(trace.find("bar_green") == std::string::npos), "1");
    reset(1); setf(recordOf(1), 196, 40.0f); setf(recordOf(1), 200, 80.0f); runHUD__Fi(1);
    expectFrom("blue bar", "super(T0,493,63", "super(T0,493,63,6,80acacac,493,24,542,124,0,0);isin4096;");
    reset(1); setw(objectBlock, 1182, 5); runHUD__Fi(1);
    expectFrom("blue alpha", "super(T0,493,63", "super(T0,493,63,6,2aacacac,493,24,493,124,0,0);");
    // Long names are cut to 120 pixels (ten per character here) and end in "..."; a six character floor stops the cut.
    reset(1); std::strcpy(reinterpret_cast<char*>(recordOf(1)), "Longnamehereforyou"); runHUD__Fi(1);
    expectFrom("trimmed name", "centered", "centered(551,8,Longnameher...,10000);fend;");
    reset(1); std::strcpy(reinterpret_cast<char*>(recordOf(1)), "abcdefgh"); fontWidth = 100; runHUD__Fi(1);
    expectFrom("floor", "centered", "centered(551,8,abcde...,10000);fend;");
    // Colours with several players: marker colour, halved on flashing frames (bit 5 of frameNm and the mask bit).
    reset(1); nmPlayers = 2; markerColor = static_cast<int>(0x80ffffffu); runHUD__Fi(1);
    expectFrom("marker", "marker", "marker(1,128);fco");
    expectFrom("marker colour", "fcolor(80ffffff", "fcolor(80ffffff,-1,0);");
    reset(1); nmPlayers = 2; markerColor = static_cast<int>(0x80ffffffu); commNameMask = 2; frameNm = 32; runHUD__Fi(1);
    expect("flash colour", num(trace.find("fcolor(807f7f7f,-1,0)") != std::string::npos), "1");
    reset(1); nmPlayers = 2; markerColor = static_cast<int>(0x80ffffffu); commNameMask = 2; frameNm = 0; runHUD__Fi(1);
    expect("no flash on frame 0", num(trace.find("fcolor(807f7f7f") == std::string::npos), "1");
    reset(1); nmPlayers = 2; commNameMask = 1; frameNm = 32; runHUD__Fi(1);
    expect("flash needs the player's bit", num(trace.find("fcolor(80081018") == std::string::npos), "1");
    // Two players draw the crystal: x0 + w(status)/2 - w(crystal)/2 - 16 with widths 90 and 50.
    reset(1); nmPlayers = 2; runHUD__Fi(1);
    expectFrom("crystal", "texc(T5", "texc(T5,489,29,6,80808080);su");
    // Level glow: an object with bit 8, no health or a capped level shows none; the fade stops at 128.
    reset(1); setw(objectBlock, 12, 8); runHUD__Fi(1); expect("glow off by object bit", num(levelUpFade[1]), "0");
    reset(1); setf(recordOf(1), 172, 0.0f); runHUD__Fi(1); expect("glow off by health", num(levelUpFade[1]), "0");
    reset(1); recordOf(1)[89] = 2; runHUD__Fi(1); expect("glow off at max level", num(levelUpFade[1]), "0");
    reset(1); levelUpFade[1] = 126; runHUD__Fi(1); expect("fade cap", num(levelUpFade[1]), "128");
    // Icon slots. Slot 0 (simple): icon 33 at x + 51, row 1; slot 4 shows the potion count with alpha.
    reset(1); fadeTimer[7 + 0] = 5; runHUD__Fi(1);
    expectFrom("slot 0", "local1;", "local1;texc(I33,565,109,4,80555555);quick(1,0,0,0,0);");
    expect("slot 0 timer", num(fadeTimer[7]) + "," + num(fadeIconY[7]), "4,3");
    reset(1); fadeTimer[7 + 0] = 1; runHUD__Fi(1); expect("slot 0 expires", num(fadeTimer[7]) + "," + num(fadeIconY[7]), "0,0");
    reset(1); fadeTimer[7 + 0] = 5; localAnswer = 0; runHUD__Fi(1); expect("remote", num(fadeTimer[7]) + trace.substr(trace.find("local1;")), "5local1;quick(1,0,0,0,0);");
    reset(1); fadeTimer[7 + 4] = 90; healthPotionsLeft[1] = 7; runHUD__Fi(1);
    expectFrom("slot 4", "local1;", "local1;fstart(4,44080e4,-1);fmt[x 7];print(528,81,x 7,100000);fend;texc(I34,497,82,4,4555555);quick(1,0,0,0,0);");
    expect("slot 4 alpha", num(iconAlpha[7 - 3 + 4]), "4");
    reset(1); fadeTimer[7 + 5] = 30; iconAlpha[7 - 3 + 5] = 6; manaPotionsLeft[1] = 0; runHUD__Fi(1);
    expect("slot 5", num(iconAlpha[7 - 3 + 5]) + trace.substr(trace.find("fstart(4"), 23), "2fstart(4,24080e4,-1);fm");
    reset(1); fadeTimer[7 + 5] = 30; iconAlpha[7 - 3 + 5] = 2; runHUD__Fi(1); expect("slot 5 floor", num(iconAlpha[7 - 3 + 5]), "0");
    reset(1); fadeTimer[7 + 5] = 90; iconAlpha[7 - 3 + 5] = 127; runHUD__Fi(1); expect("slot 5 cap", num(iconAlpha[7 - 3 + 5]), "128");
    reset(1); fadeTimer[7 + 5] = 60; healthPotionsLeft[1] = -1; manaPotionsLeft[1] = -1; runHUD__Fi(1);
    expect("negative count draws nothing", num(trace.find("texc(I35") == std::string::npos), "1");
    reset(1); fadeTimer[7 + 6] = 60; setw(recordOf(1), 21920 + 8, static_cast<int>(handleSpell)); { short n = 9; std::memcpy(spellBlock + 4, &n, 2); } runHUD__Fi(1);
    expectFrom("slot 6 spell count", "fmt[x ", "fmt[x 9];");
    reset(1); fadeTimer[7 + 6] = 60; runHUD__Fi(1); expectFrom("slot 6 no spell", "fmt[x ", "fmt[x 0];");
    std::printf(bad ? "FAIL\n" : "ok: run hud\n");
    return bad;
}
