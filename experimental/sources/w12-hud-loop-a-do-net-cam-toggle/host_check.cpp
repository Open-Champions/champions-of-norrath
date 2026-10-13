// Host check for doNetCamToggle__Fv. Not an EE plan input.
// Build: c++ -std=c++17 -Wall -Wextra -fno-builtin host_check.cpp
#include "do_net_cam_toggle.cpp"

#include <cstdio>
#include <cstring>
#include <string>

extern "C" {
signed char netNetworkMode;
int nmPlayers;
int nmPlayersAlive;
alignas(8) unsigned char player[23984 * 4];
short gameDeathTimerSingle[4];
signed char cutMode;
unsigned padAcceptButton = 0x10;
int padAcceptButtonFontChar;
alignas(8) unsigned char netLocalPad[88 * 4];
int gameCamObject, fontObject;
void* gameCam = &gameCamObject;
void* defaultFont = &fontObject;
}

static std::string trace;
static int localMask, dlg, target;
static unsigned short textBuffer[4] = {0x41, 0x42, 0, 0};

static void add(const char* format, int a = 0, int b = 0, int c = 0) {
    char text[96];
    std::snprintf(text, sizeof text, format, a, b, c);
    trace += text;
}

extern "C" {
int netPlayerIsLocal__Fi(int index) { add("L%d;", index); return (localMask >> index) & 1; }
void ChangeNetCam__6Camerai(void* camera, int mode) { add("N%d%d;", camera == &gameCamObject, mode); }
int DLG_Active__Fv() { add("D;"); return dlg; }
int GetTargetNum__6Camera(void*) { add("T;"); return target; }
unsigned playerGetMarkerColor__Fii(int p, int alpha) { add("C%d,%d;", p, alpha); return 0x1000u + static_cast<unsigned>(p); }
void fontStart__FP4FontiUii(void* font, int mode, unsigned color, int last) {
    add("S%d,%d,", font == &fontObject, mode);
    add("%x,%d;", static_cast<int>(color), last);
}
void fontPrintCentered__FiiPci(int x, int y, const char* text, int limit) {
    add("P%d,%d,", x, y);
    trace += text;
    add(",%d;", limit);
}
void fontColor__FUiii(unsigned color, int a, int b) { add("F%x,", static_cast<int>(color)); add("%d,%d;", a, b); }
unsigned short* textGetString__FPc(const char* key) { trace += "K"; trace += key; trace += ";"; return textBuffer; }
void usprintf__FPUsT0ffffffff(unsigned short* dest, unsigned short* format, float a, float b, float c, float d,
                              float e, float f, float g, float h) {
    add("U%d,", format == textBuffer);
    add("%d,", static_cast<int>(a));
    add("%d;", static_cast<int>(b + c + d + e + f + g + h));
    dest[0] = 0x58;
    dest[1] = 0;
}
void fontPrintCenteredu__FiiPUsi(int x, int y, unsigned short* text, int limit) {
    add("Q%d,%d,", x, y);
    add("%d,", text[0]);
    add("%d;", limit);
}
void fontEnd__Fv() { add("E;"); }
}

static void reset() {
    std::memset(player, 0, sizeof player);
    std::memset(netLocalPad, 0, sizeof netLocalPad);
    std::memset(gameDeathTimerSingle, 0, sizeof gameDeathTimerSingle);
    for (int i = 0; i < 4; ++i) std::snprintf(reinterpret_cast<char*>(player + 23984 * i), 16, "name%d", i);
    netNetworkMode = 1;
    nmPlayers = 3;
    nmPlayersAlive = 1;
    cutMode = 0;
    padAcceptButton = 0x10;
    padAcceptButtonFontChar = 77;
    localMask = 1 << 1;
    dlg = 0;
    target = 2;
    trace.clear();
}

static int expect(int code, const char* want) {
    if (trace != want) {
        std::printf("case %d\n got  %s\n want %s\n", code, trace.c_str(), want);
        return code;
    }
    return 0;
}

int main() {
    int rc;
    reset(); netNetworkMode = 0; doNetCamToggle__Fv();
    if ((rc = expect(1, ""))) return rc;
    // local player is index 1, dead (health 0.0), one alive: only the single-line caption.
    reset(); doNetCamToggle__Fv();
    if ((rc = expect(2, "L0;L1;N10;D;T;C2,128;S1,6,1002,-1;T;P320,352,name2,10000;E;"))) return rc;
    // pad accept bit set adds the switch call; two alive adds the second line.
    reset(); nmPlayersAlive = 2;
    std::memcpy(netLocalPad + 88 + 12, "\x10\0\0\0", 4);
    doNetCamToggle__Fv();
    if ((rc = expect(3, "L0;L1;N10;D;N11;T;C2,128;S1,6,1002,-1;T;P320,352,name2,10000;F804d4d4d,-1,0;"
                        "KnetCamToggle;U1,77,0;Q320,372,88,1000000;E;"))) return rc;
    // no local player: scan runs to nmPlayers and uses record 3.
    reset(); localMask = 0; nmPlayers = 3; doNetCamToggle__Fv();
    if ((rc = expect(4, "L0;L1;L2;N10;D;T;C2,128;S1,6,1002,-1;T;P320,352,name2,10000;E;"))) return rc;
    // nonzero health at the examined record stops everything.
    reset();
    unsigned one = 0x3f800000u; std::memcpy(player + 23984 + 172, &one, 4);
    doNetCamToggle__Fv();
    if ((rc = expect(5, "L0;L1;"))) return rc;
    // an entity word plus a running death timer stops; a zero timer does not.
    reset(); player[23984 + 21872] = 1; gameDeathTimerSingle[1] = 3;
    doNetCamToggle__Fv();
    if ((rc = expect(6, "L0;L1;"))) return rc;
    reset(); player[23984 + 21872] = 1; gameDeathTimerSingle[0] = 3;
    doNetCamToggle__Fv();
    if ((rc = expect(7, "L0;L1;N10;D;T;C2,128;S1,6,1002,-1;T;P320,352,name2,10000;E;"))) return rc;
    reset(); nmPlayersAlive = 0; doNetCamToggle__Fv();
    if ((rc = expect(8, "L0;L1;"))) return rc;
    reset(); dlg = 1; doNetCamToggle__Fv();
    if ((rc = expect(9, "L0;L1;N10;D;"))) return rc;
    reset(); cutMode = -1; doNetCamToggle__Fv();
    if ((rc = expect(10, "L0;L1;N10;D;"))) return rc;
    std::puts("net cam toggle passed");
    return 0;
}
