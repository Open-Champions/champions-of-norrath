// Not an EE plan input. Build: c++ -std=c++17 -Wall -Wextra -Werror -ffp-contract=off host_check.cpp
// Callees are stubs that record their arguments. Cell pointers are small integers (never dereferenced).
#include "shop_inventory_grid_draw.cpp"
#include <cstdio>
#include <cstring>
#include <string>

extern "C" {
int buySellVertSpacing, localRandState, iRandState;
int itemInfoHeight[4];
int hideAbilityAdjustMouse[2], shopDisableMouse[2], shopMouseDecay[2];
signed char multiHelpMessageOn[4];
alignas(8) unsigned char player[4 * 23984];
unsigned char* rcursor;
unsigned shopGrid, invGrid;
}
alignas(8) static unsigned char cursorTex[16];
static std::string trace;
static int rects, bitmaps, infoBoxes;
static std::string bitmapTrace, infoTrace;
extern "C" {
void* lumpFindResource__FPcT0(char* l, char* n) { trace += std::string("find:") + l + "," + n + ";"; return reinterpret_cast<void*>(0x5000 + std::string(n).size()); }
void textureDrawFloat__FP8_textureffiUiff(void* t, float x, float y, int m, unsigned c, float sx, float sy) {
    char b[120]; std::snprintf(b, sizeof b, "texf%lx,%g,%g,%d,%08x,%g,%g;", (unsigned long)(__UINTPTR_TYPE__)t, x, y, m, c, sx, sy); trace += b;
}
void textureDrawColor__FP8_textureiiiUi(void* t, int x, int y, int m, unsigned c) {
    char b[120]; std::snprintf(b, sizeof b, "texc%lx,%d,%d,%d,%08x;", (unsigned long)(__UINTPTR_TYPE__)t, x, y, m, c); trace += b;
}
void* getPic__4itemiPQ24item11description(int i, void* d) { trace += "pic" + std::to_string(i) + "," + std::to_string((unsigned long)(__UINTPTR_TYPE__)d) + ";"; return reinterpret_cast<void*>(0x9000); }
void runStats__Fii(int s, int p) { trace += "stats" + std::to_string(s) + "," + std::to_string(p) + ";"; }
void rectangleDraw__FiiiiiUi(int a, int b, int c, int d, int e, unsigned f) {
    ++rects;
    if (rects == 1 || rects == 65) { char x[100]; std::snprintf(x, sizeof x, "rect%d,%d,%d,%d,%d,%08x;", a, b, c, d, e, f); trace += x; }
}
void shopDrawInfoBox__FiP12screenStructPQ24item11descriptioniiiii(int p, void* s, void* cell, int mode, int a, int b, int c, int d) {
    ++infoBoxes;
    infoTrace += "info" + std::to_string(p) + (s ? "" : "?") + "," + std::to_string((unsigned long)(__UINTPTR_TYPE__)cell) + "," + std::to_string(mode) + "," +
                 std::to_string(a) + "," + std::to_string(b) + "," + std::to_string(c) + "," + std::to_string(d) + ";";
}
void shopDrawItemBitmap__FiP12screenStructfPQ24item11descriptioniii(int side, void* s, float y, void* cell, int col, int row, int flag) {
    ++bitmaps;
    char x[120]; std::snprintf(x, sizeof x, "bm%d,%g,%lu,%d,%d,%d;", side, y, (unsigned long)(__UINTPTR_TYPE__)cell, col, row, flag);
    if (s) bitmapTrace += x;
}
void shopDrawTaskBar__FP12screenStruct(void* s) { trace += s ? "bar;" : "bar?;"; }
}
static int bad;
static void expect(const char* what, const std::string& got, const std::string& want) {
    if (got != want) { std::printf("FAIL %s\n got  %s\n want %s\n", what, got.c_str(), want.c_str()); bad = 1; }
}
struct Screen { alignas(8) int w[32]; float& f(int i) { return reinterpret_cast<float&>(w[i]); } };
static Screen scr;
static void reset() {
    std::memset(&scr, 0, sizeof scr); std::memset(player, 0, sizeof player); std::memset(multiHelpMessageOn, 0, sizeof multiHelpMessageOn);
    std::memset(hideAbilityAdjustMouse, 0, sizeof hideAbilityAdjustMouse); std::memset(shopDisableMouse, 0, sizeof shopDisableMouse);
    std::memset(shopMouseDecay, 0, sizeof shopMouseDecay);
    for (int& v : itemInfoHeight) v = 99;
    buySellVertSpacing = localRandState = iRandState = 7;
    std::memset(cursorTex, 0, sizeof cursorTex); rcursor = cursorTex;
    shopGrid = 0x1000000; invGrid = 0x2000000;
    trace.clear(); bitmapTrace.clear(); infoTrace.clear(); rects = bitmaps = infoBoxes = 0;
    scr.f(17) = 100.0f; scr.f(18) = 200.0f;
}
static void run(int pn, int mode, int side) { shopInventoryGridDraw__FiiiP12screenStruct(pn, mode, side, scr.w); }

int main() {
    // Negative player: nothing at all.
    reset(); run(-1, 0, 0); expect("negative", trace, ""); expect("negative globals", std::to_string(buySellVertSpacing), "7");
    // Globals set for a valid player; tab 3 / 4 go to the stats pages.
    reset(); scr.w[20] = 3; run(2, 0, 1);
    expect("globals", std::to_string(buySellVertSpacing) + "," + std::to_string(localRandState) + "," + std::to_string(itemInfoHeight[0] + itemInfoHeight[3]), "34," + std::to_string(2 * 0xe171f) + ",0");
    expect("tab 3", trace.substr(trace.find("stats")), "stats1,0;bar;");
    reset(); scr.w[20] = 4; run(0, 1, 0); expect("tab 4", trace.substr(trace.find("stats")), "stats0,1;bar;");
    reset(); scr.w[20] = 5; run(0, 0, 0); expect("tab 5 a", trace.substr(trace.find("stats")), "stats0,2;bar;");
    reset(); scr.w[20] = 5; player[242] = 1; run(0, 0, 0); expect("tab 5 b", trace.substr(trace.find("stats")), "stats0,3;bar;");
    // Mouse cursor, plain: two draws (the second a shadow) from the first texture name.
    reset(); scr.w[20] = 3; run(0, 0, 0);
    expect("cursor plain", trace.substr(0, trace.find("stats")),
           "find:inventry.lmp,mousecursor.tex;texf500f,100,200,7,80808080,1,1;find:inventry.lmp,mousecursor.tex;texf500f,100,203,7,34000000,1,1;");
    // Cursor with a picked-up item: second texture name and two picture draws.
    reset(); scr.w[20] = 3; scr.w[22] = 1; scr.w[25] = 0x7777; { int cls = 6; std::memcpy(player + 84, &cls, 4); } run(0, 0, 0);
    expect("cursor held", trace.substr(0, trace.find("stats")),
           "find:inventry.lmp,mousecursor2.tex;texf5010,100,200,7,80808080,1,1;find:inventry.lmp,mousecursor2.tex;texf5010,100,203,7,34000000,1,1;pic6,30583;"
           "texc9000,122,222,7,80808080;texc9000,122,225,7,34000000;");
    // The cursor is skipped when any of the three flags is set; shopDisableMouse is rewritten afterwards.
    reset(); scr.w[20] = 3; shopDisableMouse[0] = 1; run(0, 0, 0); expect("disabled", trace.substr(0, 5), "stats");
    reset(); scr.w[20] = 3; shopMouseDecay[0] = 2; run(0, 0, 0); expect("decay skips draw", trace.substr(0, 5), "stats");
    expect("decay counts down", std::to_string(shopMouseDecay[0]) + std::to_string(shopDisableMouse[0]), "10");
    reset(); scr.w[20] = 5; player[240] = 1; run(0, 0, 1); expect("hide set by byte 240 on tab 5", std::to_string(hideAbilityAdjustMouse[1]) + trace.substr(0, 5), "1stats");
    reset(); scr.w[20] = 3; player[240] = 1; run(0, 0, 1); expect("byte 240 off tab 5", std::to_string(hideAbilityAdjustMouse[1]) + trace.substr(0, 4), "0find");
    reset(); scr.w[20] = 3; multiHelpMessageOn[1] = 1; run(1, 0, 0); expect("help message", trace.substr(0, 5), "stats");
    reset(); scr.w[20] = 3; scr.w[12] = 1; run(0, 0, 0); expect("w12 re-disables", std::to_string(shopDisableMouse[0]), "1");
    reset(); scr.w[20] = 3; scr.w[12] = 2; run(0, 0, 0); expect("w12 other", std::to_string(shopDisableMouse[0]), "0");
    // Grid: tab 0, no scroll, top 0, cursor texture half-height 0. Row r is at y = 140 + 34 r - scroll; rows with y < 475 and > -50 draw.
    reset(); scr.w[20] = 0; scr.w[0] = 0; scr.w[22] = 0; scr.w[10] = 0; scr.f(17) = 17.0f + 32.0f * 2; scr.f(18) = 229.0f - 140.0f + 34.0f * 3 + 5.0f;  // column 2, row 3
    run(0, 0, 1);
    {
        int rows = 0;
        for (int r = 0; r < 75; ++r) { const float y = static_cast<float>(140 + 34 * r); if (y < 475.0f && -50.0f < y) ++rows; }
        expect("bitmap count", std::to_string(bitmaps), std::to_string(rows * 9));
        expect("info box", infoTrace, "info0," + std::to_string(0x1000000u + 0x23988u + 3u * 72u + 2u * 5400u) + ",0,70,128,81,228;");
        expect("first bitmap", bitmapTrace.substr(0, bitmapTrace.find(';') + 1), "bm1,140," + std::to_string(0x1000000u + 0x23988u) + ",0,0,1;");
        expect("rect fade", trace.substr(trace.find("rect")), "rect15,413,305,414,4,01000000;rect15,477,305,478,4,7f000000;find:inventry.lmp,inventoryunderbar.tex;texc5015,320,98,5,80808080;bar;");
        expect("rand state", std::to_string(iRandState), "0");
    }
    // Inventory mode uses invGrid and a zero flag; negative scroll shows rows above 475.
    reset(); scr.w[20] = 1; scr.w[10] = 0; scr.f(17) = -500.0f; scr.f(18) = -500.0f; run(1, 5, 0);
    expect("inv base", bitmapTrace.substr(0, bitmapTrace.find(';') + 1), "bm0,140," + std::to_string(0x2000000u + 0xbdd8u) + ",0,0,0;");
    expect("no info box off cursor", std::to_string(infoBoxes), "0");
    expect("rand state p1", std::to_string(iRandState), std::to_string(0x3184b3));
    // Scrolled far: nothing visible.
    reset(); scr.w[20] = 0; scr.w[10] = 100000; run(0, 0, 0); expect("scrolled", std::to_string(bitmaps), "0");
    std::printf(bad ? "FAIL\n" : "ok: shop inventory grid draw\n");
    return bad;
}
