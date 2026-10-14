// Host check for shopDrawItemBitmap__FiP12screenStructfPQ24item11descriptioniii. Not an EE plan input.
// Build: c++ -std=c++17 -Wall -Wextra -Werror -ffp-contract=off -fno-builtin host_check.cpp
// The image's 32-bit pointer words are offsets into one host arena. Callees append to a trace and
// the expected trace is built by a small model that follows the rules in the source header, for
// many combinations of side, tab, column, row, mode, picture size, mouse position and results of
// the comparison, equipped and wielding calls.
#define NORRATH_HOST_ARENA
#include "shop_draw_item_bitmap.cpp"

#include <cstdio>
#include <cstring>
#include <initializer_list>
#include <string>
#include <vector>

static unsigned char arena[1 << 22];
extern "C" {
unsigned char* norrath_host_base = arena;
alignas(8) unsigned char player[4 * 23984 + 1024];
alignas(8) unsigned char shopList[3 * 4616 + 64];
unsigned invGrid, shopGrid;
int buySellVertSpacing;
int drawFrameNm;
unsigned iRandState;
int drewBitmap[4];
}

static std::string trace;
static std::vector<int> compareAnswers;
static size_t compareIndex;
static int equippedAnswer, wieldAnswer;
static unsigned char pictureObject[16];
static int clipDx, clipDy;
static std::string num(long v) { return std::to_string(v); }
static std::string hex(unsigned v) { char b[16]; std::snprintf(b, sizeof b, "%x", v); return b; }
static long offsetOf(const void* p) { return static_cast<long>(static_cast<const unsigned char*>(p) - arena); }

extern "C" void* getPic__4itemiPQ24item11description(int index, void* cell) {
    trace += "getPic(" + num(index) + "," + num(offsetOf(cell)) + ");";
    return pictureObject;
}
extern "C" void shopClipGridCoordinates__FPiN30(int* x0, int* y0, int* x1, int* y1) {
    trace += "clip(" + num(*x0) + "," + num(*y0) + "," + num(*x1) + "," + num(*y1) + ");";
    *x0 += clipDx;
    *y1 += clipDy;
}
extern "C" int compareToItem__Q24item11descriptionPQ24item11description(void* left, void* right) {
    trace += "cmp(" + num(offsetOf(left)) + "," + num(offsetOf(right)) + ");";
    return compareIndex < compareAnswers.size() ? compareAnswers[compareIndex++] : 0;
}
extern "C" int sprintf(char* out, const char* format, ...) {
    __builtin_va_list ap;
    __builtin_va_start(ap, format);
    int value = __builtin_va_arg(ap, int);
    __builtin_va_end(ap);
    trace += std::string("fmt ") + format + " " + num(value) + ";";
    return std::snprintf(out, 64, "inventrybox%d.tex", value);
}
extern "C" void* lumpFindResource__FPcT0(char* lump, char* name) {
    trace += std::string("find ") + lump + " " + name + ";";
    return arena + 0x3f0000 + (std::strlen(name) & 0xff) * 16;
}
extern "C" void textureDrawSuperColor__FP8_textureiiiUiiiiicc(void* tex, int x, int y, int mode, unsigned color, int a, int b, int c, int d, char e, char f) {
    trace += "tex(" + std::string(tex == pictureObject ? "pic" : "other") + "," + num(x) + "," + num(y) + "," + num(mode) + "," + hex(color) + "," + num(a) + "," + num(b) + "," + num(c) + "," + num(d) + "," + num(e) + "," + num(f) + ");";
}
extern "C" float isin__Fi(int angle) { trace += "isin(" + num(angle) + ");"; return angle == 0 ? 0.5f : 0.25f; }
extern "C" int itemEquipped__FiPQ24item11description(int pl, void* item) {
    const unsigned char* p = static_cast<const unsigned char*>(item);
    if (p >= shopList && p < shopList + sizeof shopList) trace += "equipped(" + num(pl) + ",S" + num(p - shopList) + ");";
    else trace += "equipped(" + num(pl) + ",P" + num(p - player) + ");";
    return equippedAnswer;
}
extern "C" int inventoryCheckWieldingStatus__FiPQ24item11description(int pl, void* cell) {
    trace += "wield(" + num(pl) + "," + num(offsetOf(cell)) + ");";
    return wieldAnswer;
}
extern "C" void rectangleDraw__FiiiiiUi(int x0, int y0, int x1, int y1, int mode, unsigned color) {
    trace += "rect(" + num(x0) + "," + num(y0) + "," + num(x1) + "," + num(y1) + "," + num(mode) + "," + hex(color) + ");";
}
extern "C" void shopDrawItemBitmapGlow__FP8_textureii(void* tex, int x, int y) {
    trace += std::string("glow(") + (tex == pictureObject ? "pic" : "other") + "," + num(x) + "," + num(y) + ");";
}

#define CHECK(c) do { if (!(c)) { std::printf("line %d: %s\n    trace: %s\n", __LINE__, #c, trace.c_str()); return 1; } } while (0)

template <class T> static void put(unsigned char* base, int off, T v) { std::memcpy(base + off, &v, sizeof v); }

struct Case {
    int side = 0, tab = 0, column = 0, row = 0, shop = 0, playerIndex = 1;
    float y = 100.0f;
    int cellWord = 1;                 // word at cell + 0
    int picW = 64, picH = 40;
    float mouseX = 0, mouseY = 0;
    std::vector<int> compares;        // results handed out in order
    int equipped = 0, wield = 0;
    int screenLeft = 10;
    int frame = 0;
    int slot = 3;                     // halfword at cell(col,row) + 34
    int clipDX = 0, clipDY = 0;
};

static const long kCell = 0x300000;

// Follows the rules of the source header; returns the trace the call should leave.
static std::string model(const Case& c, unsigned& seedOut, bool& seedSet, bool& hoverOut) {
    std::string t;
    seedSet = false; hoverOut = false;
    const long gridBase = (c.shop ? 0x100000 : 0x200000) + c.side * 0x23988L + c.tab * 0xbdd8L;
    const long cellAt = gridBase + 5400L * c.column + 72L * c.row;
    int cw = 32, ch = 34;
    if (c.cellWord != 0) {
        t += "getPic(" + num(c.playerIndex + 10) + "," + num(kCell) + ");";
        cw = (c.picW / 32 + 1) * 32;
        ch = ((c.picH - 6) / 34 + 1) * 34;
    }
    const int colX = c.column * 32;
    const int stripY = static_cast<int>(c.y - 24.0f);
    const int stripX = c.screenLeft + colX + 14;
    t += "clip(" + num(stripX + 1) + "," + num(stripY) + "," + num(stripX + 33) + "," + num(stripY + 1) + ");";
    t += "cmp(" + num(cellAt) + "," + num(cellAt - 72) + ");";
    const int cx = stripX + 1 + c.clipDX;     // the stub moves the clipped x0 of the first strip
    t += "clip(" + num(cx + 32) + "," + num(stripY) + "," + num(cx + 33) + "," + num(stripY + buySellVertSpacing) + ");";
    t += "cmp(" + num(cellAt) + "," + num(cellAt + 5400) + ");";
    if (c.column % 3 == 0 && ((c.row - 1) & 1) != 0) {
        const unsigned seed = (static_cast<unsigned>(c.column * 12012 + c.row * 0x3cc5b) + static_cast<unsigned>(c.playerIndex * 0x352927)) * 0x19660du + 0x3c6ef35fu;
        seedOut = seed; seedSet = true;
        const int n = static_cast<int>((seed >> 16) % 3u) + 1;
        t += "fmt inventrybox%d.tex " + num(n) + ";find inventry.lmp inventrybox" + num(n) + ".tex;";
        t += "tex(other," + num(stripX + 1) + "," + num(stripY) + ",0,80808080,0,105,640,512,0,0);";
        if (c.column == 6) {
            t += "find inventry.lmp inventryboxend.tex;tex(other," + num(stripX + 1 + 96) + "," + num(stripY) + ",0,80808080,0,105,640,512,0,0);";
        }
    }
    if (c.cellWord == 0) return t;
    size_t next = 2;
    bool same = true;
    if (c.row != 0) {
        t += "cmp(" + num(cellAt - 72) + "," + num(kCell) + ");";
        same = (next < c.compares.size() ? c.compares[next] : 0) == 0;
        ++next;
    }
    if (c.column != 0) {
        t += "cmp(" + num(cellAt - 5400) + "," + num(kCell) + ");";
        if ((next < c.compares.size() ? c.compares[next] : 0) != 0) same = false;
        ++next;
    }
    const int top = static_cast<int>(c.y - 30.0f);
    int x0 = c.screenLeft + colX + 17, y0 = top + 9, x1 = c.screenLeft + cw + colX + 15, y1 = top + 9 + ch - 2;
    t += "clip(" + num(x0) + "," + num(y0) + "," + num(x1) + "," + num(y1) + ");";
    x0 += c.clipDX; y1 += c.clipDY;
    int brightness = 128;
    if (static_cast<float>(x0) <= c.mouseX && c.mouseX <= static_cast<float>(x1 + 2) && static_cast<float>(y0 - 4) <= c.mouseY && c.mouseY <= static_cast<float>(y1 - 2)) {
        hoverOut = true;
        t += "isin(" + num(c.frame << 10) + ");";
        brightness = static_cast<int>((c.frame == 0 ? 0.5f : 0.25f) * 32.0f + 140.0f);
    }
    const int px = c.screenLeft + (cw / 2 - c.picW / 2) + colX + 15;
    const int py = (ch - c.picH) / 2 + 6 + top;
    const std::string item = c.shop ? "S" + num(4616L * c.tab + 72L * c.slot) : "P" + num(328L + 23984L * c.playerIndex + 4616L * c.tab + 72L * c.slot);
    t += "equipped(" + num(c.playerIndex) + "," + item + ");";
    const std::string rect = num(x0) + "," + num(y0) + "," + num(x1) + "," + num(y1) + ",1,";
    const std::string texCall = "tex(pic," + num(px) + "," + num(py) + ",5," + hex(0x80000000u | brightness * 0x010101u) + ",0,102,640,600,0,0);";
    const std::string glow = "glow(pic," + num(px) + "," + num(py) + ");";
    if (c.equipped != 0) {
        if (!same) return t;
        t += "rect(" + rect + "20008000);" + texCall;
    } else {
        t += "wield(" + num(c.playerIndex) + "," + num(kCell) + ");";
        if (!same) return t;
        t += "rect(" + rect + (c.wield != 0 ? "20000080" : "20445558") + ");" + texCall;
    }
    if (hoverOut) t += glow;
    return t;
}

static int run(const Case& c) {
    std::memset(arena, 0, 0x200);
    std::memset(player, 0, sizeof player);
    std::memset(shopList, 0, sizeof shopList);
    unsigned char* screen = arena + 0x100;
    put<int>(screen, 0, c.screenLeft);
    put<int>(screen, 52, c.playerIndex);
    put<int>(screen, 80, c.tab);
    put<float>(screen, 68, c.mouseX);
    put<float>(screen, 72, c.mouseY);
    unsigned char* cell = arena + kCell;
    std::memset(cell, 0, 72);
    put<int>(cell, 0, c.cellWord);
    put<int>(player + 23984 * c.playerIndex, 84, c.playerIndex + 10);
    invGrid = 0x200000; shopGrid = 0x100000;
    buySellVertSpacing = 34; drawFrameNm = c.frame; iRandState = 0x12345678;
    for (int i = 0; i < 4; ++i) drewBitmap[i] = 0;
    unsigned char* gridCell = arena + (c.shop ? 0x100000 : 0x200000) + c.side * 0x23988L + c.tab * 0xbdd8L + 5400L * c.column + 72L * c.row;
    put<short>(gridCell, 34, static_cast<short>(c.slot));
    put<short>(pictureObject, 0, static_cast<short>(c.picW));
    put<short>(pictureObject, 2, static_cast<short>(c.picH));
    compareAnswers = c.compares; compareIndex = 0;
    equippedAnswer = c.equipped; wieldAnswer = c.wield;
    clipDx = c.clipDX; clipDy = c.clipDY;
    trace.clear();
    shopDrawItemBitmap__FiP12screenStructfPQ24item11descriptioniii(c.side, screen, c.y, cell, c.column, c.row, c.shop);
    unsigned seed = 0; bool seedSet = false, hover = false;
    const std::string want = model(c, seed, seedSet, hover);
    if (trace != want) {
        std::printf("case side=%d tab=%d col=%d row=%d shop=%d pl=%d y=%g word=%d pic=%dx%d mouse=%g,%g eq=%d wd=%d\n  got  %s\n  want %s\n", c.side, c.tab, c.column, c.row, c.shop,
                    c.playerIndex, c.y, c.cellWord, c.picW, c.picH, c.mouseX, c.mouseY, c.equipped, c.wield, trace.c_str(), want.c_str());
        return 1;
    }
    if (seedSet && iRandState != seed) { std::puts("random state differs"); return 2; }
    if (!seedSet && iRandState != 0x12345678) { std::puts("random state changed without decoration"); return 3; }
    if (c.cellWord != 0 && drewBitmap[c.playerIndex] != (hover ? 1 : 0)) { std::puts("drewBitmap differs"); return 4; }
    for (int i = 0; i < 4; ++i) if (i != c.playerIndex && drewBitmap[i] != 0) { std::puts("drewBitmap of another player set"); return 5; }
    return 0;
}

int main() {
    // an empty cell: strips and the decoration only
    for (int column : {0, 1, 3, 6, 9})
        for (int row : {0, 1, 2, 3}) {
            Case c; c.cellWord = 0; c.column = column; c.row = row;
            CHECK(run(c) == 0);
        }
    // a filled cell, many combinations
    int cases = 0;
    for (int side : {0, 1})
        for (int tab : {0, 2})
            for (int shop : {0, 1})
                for (int column : {0, 1, 3, 6})
                    for (int row : {0, 1, 2}) {
                        for (int variant = 0; variant < 12; ++variant) {
                            Case c;
                            c.side = side; c.tab = tab; c.shop = shop; c.column = column; c.row = row;
                            c.playerIndex = variant % 4;
                            const float ys[] = {60.0f, 100.0f, 150.5f, 200.0f, -5.0f};
                            c.y = ys[variant % 5];
                            const int pws[] = {1, 31, 32, 33, 64, 70, 96};
                            const int phs[] = {2, 5, 6, 7, 39, 40, 41, 74, 75, 100};
                            c.picW = pws[(variant * 3 + column) % 7];
                            c.picH = phs[(variant * 7 + row) % 10];
                            c.compares = {0, 0, (variant & 1), (variant >> 1) & 1};
                            c.equipped = (variant >> 2) & 1;
                            c.wield = (variant >> 1) & 1;
                            c.screenLeft = variant % 3 == 0 ? 0 : 10 + variant;
                            c.frame = variant % 2;
                            c.slot = variant + 1;
                            c.clipDX = variant % 3; c.clipDY = -(variant % 2);
                            // aim the mouse inside, left of, right of, above or below the picture rectangle
                            const int colX = column * 32;
                            const int cw = (c.picW / 32 + 1) * 32, ch = ((c.picH - 6) / 34 + 1) * 34;
                            const int top = static_cast<int>(c.y - 30.0f);
                            const float cxm = static_cast<float>(c.screenLeft + colX + 17 + cw / 2);
                            const float cym = static_cast<float>(top + 9 + ch / 2);
                            const float mx[] = {cxm, 0.0f, 900.0f, cxm, cxm};
                            const float my[] = {cym, cym, cym, 0.0f, 900.0f};
                            c.mouseX = mx[variant % 5]; c.mouseY = my[variant % 5];
                            CHECK(run(c) == 0);
                            ++cases;
                        }
                    }
    std::printf("%d cases\n", cases);
    // the edges of the hover rectangle: x0 <= mx <= x1 + 2 and y0 - 4 <= my <= y1 - 2
    {
        Case c; c.column = 0; c.row = 0; c.picW = 64; c.picH = 40; c.y = 100.0f; c.screenLeft = 10;
        const int cw = 96, ch = 68, top = 70;
        const int x0 = 10 + 17, x1 = 10 + cw + 15, y0 = top + 9, y1 = top + 9 + ch - 2;
        struct Edge { float mx, my; bool hover; } edges[] = {
            {static_cast<float>(x0), static_cast<float>(y0), true}, {static_cast<float>(x0 - 1), static_cast<float>(y0), false},
            {static_cast<float>(x1 + 2), static_cast<float>(y1 - 2), true}, {static_cast<float>(x1 + 3), static_cast<float>(y1 - 2), false},
            {static_cast<float>(x0), static_cast<float>(y0 - 4), true}, {static_cast<float>(x0), static_cast<float>(y0 - 5), false},
            {static_cast<float>(x0), static_cast<float>(y1 - 2), true}, {static_cast<float>(x0), static_cast<float>(y1 - 1), false},
            {x0 + 0.5f, y0 + 0.5f, true}, {x0 - 0.5f, y0 + 0.5f, false}};
        for (const Edge& e : edges) {
            c.mouseX = e.mx; c.mouseY = e.my;
            CHECK(run(c) == 0);
            CHECK((drewBitmap[c.playerIndex] == 1) == e.hover);
        }
    }
    std::puts("shop draw item bitmap passed");
    return 0;
}
