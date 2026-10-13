// Host check for shopFrame__Fv (includes the source; build the file alone).
#include "shop_frame.cpp"
#include <cstring>
#include <string>
#include <vector>
unsigned char player[2 * 23984];
int nmPlayers, drawFrameNm, startFrame, shopIntro;
signed char netNetworkMode;
unsigned char shop[464];
static unsigned char marble_data[8], bevel_data[8];
unsigned char* marble = marble_data;
unsigned char* bevel = bevel_data;
static std::vector<std::string> calls; static int lump_present[3];
static int query_count;
extern "C" int lumpQuery__FPc(char* n) { calls.push_back(std::string("query:") + n); return lump_present[query_count++ < 3 ? query_count - 1 : 2]; }
extern "C" void inventoryLoad__Fv() { calls.push_back("load"); }
extern "C" void shopUnpackToGrid__Fiii(int a, int b, int c) { calls.push_back("unpack" + std::to_string(a) + std::to_string(b) + std::to_string(c)); }
extern "C" void shopRun__Fi(int a) { calls.push_back("run" + std::to_string(a)); }
extern "C" void inventoryDrawPlayerColor__Fiii(int p, int a, int b) { calls.push_back("dpc" + std::to_string(p) + "," + std::to_string(a) + "," + std::to_string(b)); }
extern "C" char* lumpFindResource__FPcT0(char* l, char* n) { calls.push_back(std::string("find:") + l + ":" + n); return reinterpret_cast<char*>(bevel_data + 4); }
extern "C" void textureDrawColor__FP8_textureiiiUi(void* t, int x, int y, int m, unsigned c) {
    calls.push_back(std::string("tex") + (t == marble_data ? "M" : t == bevel_data ? "B" : "F") + std::to_string(x) + "," + std::to_string(y) + "," + std::to_string(m) + (c == 0x80808080u ? "" : "!"));
}
extern "C" void textureDrawSuperColor__FP8_textureiiiUiiiiicc(void*, int a, int b, int c, unsigned col, int d, int e, int f, int g, char h, char i) {
    calls.push_back("super" + std::to_string(a) + std::to_string(b) + std::to_string(c) + "," + std::to_string(d) + std::to_string(e) + "," + std::to_string(f) + "," + std::to_string(g) + "," + std::to_string(h) + std::to_string(i) + (col == 0x80808080u ? "" : "!"));
}
static int& sw(unsigned o) { return *reinterpret_cast<int*>(shop + o); }
static void reset(int players, int net) {
    std::memset(shop, 0, sizeof shop); std::memset(player, 0, sizeof player); calls.clear(); query_count = 0;
    lump_present[0] = lump_present[1] = lump_present[2] = 1; nmPlayers = players; netNetworkMode = (signed char)net;
    drawFrameNm = 100; startFrame = 100; shopIntro = 7;
    *reinterpret_cast<short*>(marble_data + 2) = 10;
    sw(52) = 0; sw(168) = 1;
    *reinterpret_cast<float*>(player + 172) = 1.0f; *reinterpret_cast<float*>(player + 23984 + 172) = 1.0f;
}
int main() {
    typedef std::vector<std::string> V;
    reset(1, 0); lump_present[0] = 0; lump_present[1] = 0; shopFrame__Fv();
    if (calls != V{"query:inventry.lmp", "load", "query:inventry.lmp"}) return 1;
    reset(1, 0); lump_present[0] = 0; shopFrame__Fv();
    if (calls != V{"query:inventry.lmp", "load", "query:inventry.lmp", "unpack001"}) return 2;
    reset(1, 0); shopFrame__Fv();
    if (calls != V{"query:inventry.lmp", "run0", "query:inventry.lmp", "texM0,111,0", "texM0,121,0", "find:shop.lmp:front.tex", "texF320,0,1"}) return 3;
    if (shopIntro != 7) return 4;
    reset(1, 0); lump_present[1] = 0; startFrame = 80; shopFrame__Fv();         // drawFrameNm - startFrame == 20
    if (calls != V{"query:inventry.lmp", "run0", "query:inventry.lmp", "find:shop.lmp:front.tex", "texF320,0,1"} || shopIntro != 0) return 5;
    reset(4, 2); shopFrame__Fv();                                                // network mode behaves like one player for the marble
    if (calls.size() != 5 || calls[1] != "run0") return 6;
    // two players, both alive: full draw
    reset(2, 0); shopFrame__Fv();
    if (calls != V{"query:inventry.lmp", "run1", "run0", "query:inventry.lmp", "texM0,111,0", "texM0,121,0", "dpc0,0,1", "dpc1,1,1",
                   "texM320,111,0", "texM320,121,0", "texB314,0,1"}) return 7;
    // state 2 of record 1 (shop+192) also draws the front texture once
    reset(2, 0); sw(192) = 2; shopFrame__Fv();
    if (calls != V{"query:inventry.lmp", "run1", "run0", "query:inventry.lmp", "texM0,111,0", "texM0,121,0", "dpc0,0,1", "find:shop.lmp:front.tex", "texF320,0,1"}) return 8;
    // a dead player (float <= 0) marks its record state 2 and, for record 0, draws the super texture
    reset(2, 0); *reinterpret_cast<float*>(player + 172) = 0.0f; sw(52) = 0; sw(168) = 0;   // both entries name player 0
    shopFrame__Fv();
    if (sw(76) != 2 || sw(192) != 0) return 9;
    if (calls.back() != "super001,00,640,448,10") return 10;
    reset(2, 0); *reinterpret_cast<float*>(player + 23984 + 172) = -1.0f; sw(52) = -1; sw(168) = 1; shopFrame__Fv();
    if (sw(192) != 2 || sw(76) != 0) return 11;
    reset(0, 0); shopFrame__Fv();                                                // fewer than two players: no draw
    if (calls != V{"query:inventry.lmp"}) return 12;
    return 0;
}
