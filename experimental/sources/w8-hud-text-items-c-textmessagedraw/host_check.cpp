#include <cstring>
#include <string>
#include <vector>
signed char netNetworkMode;
int frontEndRunning, chatOn, waiting, drawFrameNm, msgTimeout[8];
void* smallFont = (void*)0x1234;
unsigned char outMsg[160];
unsigned char inMsg[8][126];
unsigned char player[23984 * 4];
static int dlg, raw_calls; static std::vector<int> raw;
struct Draw { int x, y; const unsigned char* text; const unsigned char* name; int width; unsigned color; unsigned char cell; };
static std::vector<Draw> draws; static std::vector<std::string> events;
extern "C" int keyboardGetNextKey__Fv() { raw_calls++; if (raw.empty()) return -1; int k = raw.back(); raw.pop_back(); return k; }
extern "C" int DLG_Active__Fv() { return dlg; }
extern "C" void fontStart__FP4FontiUii(void* f, int m, unsigned c, int l) {
    events.push_back("start"); if (f != smallFont || m != 6 || c != 0x7f808080u || l != -1) events.push_back("BADSTART");
}
extern "C" void fontEnd__Fv() { events.push_back("end"); }
static std::vector<std::pair<int,int>> color_calls;
extern "C" unsigned playerGetMarkerColor__Fii(int p, int a) { color_calls.push_back({p, a}); return 0xff000000u | (p & 0xff) | (unsigned)(a << 8); }
extern "C" int drawUpFrom__FiiPCUcT2iUi(int x, int y, const unsigned char* t, const unsigned char* n, int w, unsigned c) {
    draws.push_back({x, y, t, n, w, c, 0});
    return y - 30;
}
extern "C" void textMessageDraw__Fv();

static void reset() {
    netNetworkMode = 1; frontEndRunning = 0; dlg = 0; chatOn = 1; waiting = 0; drawFrameNm = 0;
    memset(outMsg, 0, sizeof outMsg); outMsg[0] = 1; outMsg[1] = 0; outMsg[2] = 5; outMsg[3] = 32;
    memset(inMsg, 0, sizeof inMsg); for (int i = 0; i < 8; i++) { inMsg[i][2] = 0xfe; inMsg[i][1] = 255; inMsg[i][0] = 2; msgTimeout[i] = 0; }
    draws.clear(); events.clear(); color_calls.clear(); raw.clear(); raw_calls = 0;
}
int main() {
    reset(); netNetworkMode = 0; raw = {1, 2};
    textMessageDraw__Fv();
    if (!raw.empty() || !draws.empty() || !events.empty()) return 1;
    reset(); frontEndRunning = 1; textMessageDraw__Fv(); if (!events.empty()) return 2;
    reset(); dlg = 1; textMessageDraw__Fv(); if (!events.empty()) return 3;
    reset(); chatOn = 0; textMessageDraw__Fv(); if (!events.empty()) return 4;
    // Only the edit line: len 1 gives null strings; blink cell at pos+3 is -115 at frame 0.
    reset(); textMessageDraw__Fv();
    if (draws.size() != 1 || events != std::vector<std::string>{"start", "end"}) return 5;
    if (draws[0].x != 20 || draws[0].y != 390 || draws[0].text || draws[0].name || draws[0].width != 600) return 6;
    if (draws[0].color != (0xff000000u | 5 | (127u << 8))) return 7;
    if (outMsg[3] != (unsigned char)-115) return 8;
    drawFrameNm = 45; textMessageDraw__Fv(); if (outMsg[3] != (unsigned char)-116) return 9;
    drawFrameNm = 59 + 60 * 7; textMessageDraw__Fv(); if (outMsg[3] != (unsigned char)-116) return 10;
    drawFrameNm = 60 * 3 + 29; textMessageDraw__Fv(); if (outMsg[3] != (unsigned char)-115) return 11;
    // No blink when the cursor byte is 0xff; longer line passes outMsg + 3.
    reset(); outMsg[0] = 4; outMsg[1] = 255; outMsg[3] = 'a'; textMessageDraw__Fv();
    if (draws[0].text != outMsg + 3 || outMsg[3] != 'a') return 12;
    // Waiting halves the three low bytes and keeps the top byte.
    reset(); waiting = 1; textMessageDraw__Fv();
    if (draws[0].color != (0xff000000u | (5u >> 1) | (((127u) >> 1) << 8))) return 13;
    // Incoming entries.
    reset();
    inMsg[1][2] = 2; msgTimeout[1] = 100; inMsg[1][1] = 255; inMsg[1][3] = 'h';
    inMsg[3][2] = 1; msgTimeout[3] = 700;                                  // too old: skipped
    inMsg[4][2] = 3; msgTimeout[4] = 540; inMsg[4][1] = 2;                 // fading, blink cell
    inMsg[6][2] = 0; msgTimeout[6] = 480; inMsg[6][1] = 255;
    inMsg[7][2] = 1; msgTimeout[7] = 600; inMsg[7][1] = 255;
    textMessageDraw__Fv();
    // Edit line plus entries 1, 4, 6 and 7; entry 3 is older than 600 and is skipped.
    if (draws.size() != 5) return 14;
    if (draws[1].y != 360 || draws[1].text != inMsg[1] + 3 || draws[1].name != player + 2 * 23984) return 15;
    if (draws[2].y != 330 || draws[2].name != player + 3 * 23984) return 16;
    if (draws[3].y != 300 || draws[3].name != player + 0) return 17;
    if (draws[4].y != 270 || draws[4].name != player + 1 * 23984 || draws[4].x != 20 || draws[4].width != 600) return 18;
    std::vector<std::pair<int,int>> want = {{5, 127}, {2, 127}, {3, (600 - 540) * 127 / 120}, {0, 127}, {1, 0}};
    if (color_calls != want) return 19;
    if (inMsg[4][5] != (unsigned char)-115) return 20;
    if (draws[2].color != (0xff000000u | 3 | (63u << 8))) return 21;
    return 0;
}
