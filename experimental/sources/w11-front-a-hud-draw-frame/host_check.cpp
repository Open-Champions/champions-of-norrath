// Host check for HUDDrawFrame__Fv. Not an EE plan input.
// Build: c++ -std=c++17 -Wall -Wextra -Werror -fno-builtin host_check.cpp
// Every callee is a stub that appends to a trace; the expected trace is built from the notes
// at the top of hud_draw_frame.cpp for each combination of the network-trouble inputs.
#include "hud_draw_frame.cpp"

#include <cstdio>
#include <cstring>
#include <string>

extern "C" {
int nmPlayers, coreLoading, netTrouble, drawFrameNm, gameBailFromGameLoop;
signed char cutMode, netNetworkMode, netKickedOut;
unsigned short infoMessage[8];
unsigned short infoMessageTimer;
unsigned char infoMessageAlways;
}
static std::string trace;
static int connected;
alignas(8) static unsigned char tex[16];
static unsigned short strings[3][2];

extern "C" {
void drawGold__Fv() { trace += "gold;"; }
void runHUD__Fi(int i) { trace += "hud" + std::to_string(i) + ";"; }
void helpfulMessagesDraw__Fv() { trace += "help;"; }
void drawBust__Fv() { trace += "bust;"; }
void drawCompass__Fv() { trace += "compass;"; }
void doNetCamToggle__Fv() { trace += "cam;"; }
void hudRunNoControllers__Fi(int n) { trace += "noctl" + std::to_string(n) + ";"; }
void* lumpFindResource__FPcT0(char* lump, const char* name) { trace += std::string("find ") + lump + "," + name + ";"; return tex; }
void textureDrawColor__FP8_textureiiiUi(void* t, int x, int y, int mode, unsigned color) {
    trace += std::string("tex") + (t == tex ? "" : "?") + std::to_string(x) + "," + std::to_string(y) + "," + std::to_string(mode) + "," + std::to_string(color) + ";";
}
unsigned short* textGetString__FPc(const char* name) {
    trace += std::string("text ") + name + ";";
    const std::string n = name;
    return strings[n == "netMessageLostConnection" ? 0 : n == "netMessageKickedOut" ? 1 : 2];
}
void ustrcpy__FPUsT0(unsigned short* to, unsigned short* from) {
    trace += std::string("copy") + (to == infoMessage ? "" : "?") + std::to_string(from - &strings[0][0]) + ";";
}
int netPlayerIsConnected__Fi(int i) { trace += "conn" + std::to_string(i) + ";"; return connected; }
void gameNetKick__Fi(int i) { trace += "kick" + std::to_string(i) + ";"; }
}

static int failures;
static void check(const char* what, const std::string& want) {
    if (trace != want) { std::printf("FAIL %s\n got  %s\n want %s\n", what, trace.c_str(), want.c_str()); ++failures; }
}
static void setup(int trouble, int kicked, int conn, int frameNm, int w, int h) {
    nmPlayers = 2; coreLoading = 0; cutMode = 0; netNetworkMode = 1;
    netTrouble = trouble; netKickedOut = static_cast<signed char>(kicked); connected = conn; drawFrameNm = frameNm;
    infoMessageTimer = 7; infoMessageAlways = 0; gameBailFromGameLoop = 0;
    const short ws = static_cast<short>(w), hs = static_cast<short>(h);
    std::memcpy(tex, &ws, 2); std::memcpy(tex + 2, &hs, 2);
    trace.clear();
}
static const char* const head = "gold;hud0;hud1;help;bust;compass;cam;noctl2;";
static const char* const lost = "text netMessageLostConnection;copy0;";
static const char* const kickedText = "text netMessageKickedOut;copy2;";
static const char* const hostGone = "text netMessageHostGone;copy4;";

int main() {
    // Not in a network game, or no trouble: only the common part runs.
    setup(5000, 1, 0, 0x20, 8, 8); netNetworkMode = 0; HUDDrawFrame__Fv(); check("no net", head);
    setup(0, 1, 0, 0x20, 8, 8); HUDDrawFrame__Fv(); check("no trouble", head);
    setup(1, 0, 1, 0, 8, 8); nmPlayers = 0; coreLoading = 1; HUDDrawFrame__Fv();
    check("loading, no players", "gold;help;bust;compass;cam;conn0;");
    setup(1, 0, 1, 0, 8, 8); cutMode = 1; HUDDrawFrame__Fv(); check("cut mode", "gold;hud0;hud1;help;bust;compass;cam;conn0;");
    // Trouble thresholds with the host connected and nobody kicked.
    setup(1, 0, 1, 0, 8, 8); HUDDrawFrame__Fv(); check("trouble 1", std::string(head) + "conn0;");
    setup(1200, 0, 1, 0, 8, 8); HUDDrawFrame__Fv(); check("trouble 1200", std::string(head) + "conn0;conn0;");
    setup(1201, 0, 1, 0, 8, 8); HUDDrawFrame__Fv(); check("trouble 1201", std::string(head) + lost + "conn0;conn0;");
    if (infoMessageTimer != 300 || infoMessageAlways != 1) { std::puts("FAIL message state"); ++failures; }
    setup(1800, 0, 1, 0, 8, 8); HUDDrawFrame__Fv(); check("trouble 1800", std::string(head) + lost + "conn0;conn0;");
    setup(1801, 0, 1, 0, 8, 8); HUDDrawFrame__Fv(); check("trouble 1801", std::string(head) + lost + "conn0;kick0;");
    if (gameBailFromGameLoop != 1) { std::puts("FAIL bail"); ++failures; }
    // Host gone: the message, and the early leave once above 600.
    setup(600, 0, 0, 0, 8, 8); HUDDrawFrame__Fv(); check("gone 600", std::string(head) + "conn0;" + hostGone);
    setup(601, 0, 0, 0, 8, 8); HUDDrawFrame__Fv(); check("gone 601", std::string(head) + "conn0;" + hostGone + "conn0;kick0;");
    if (gameBailFromGameLoop != 1) { std::puts("FAIL bail 2"); ++failures; }
    // Kicked: the message, and a leave above 600 that does not ask the connection again for the test.
    setup(601, 1, 1, 0, 8, 8); HUDDrawFrame__Fv(); check("kicked 601", std::string(head) + kickedText + "conn0;kick0;");
    setup(600, 1, 1, 0, 8, 8); HUDDrawFrame__Fv(); check("kicked 600", std::string(head) + kickedText + "conn0;");
    // Overlay texture, centred with the halves taken toward zero.
    setup(1, 0, 1, 0x20, 64, 32); HUDDrawFrame__Fv();
    check("overlay", std::string(head) + "find hud.lmp,noconnect.tex;tex288,208,7,2155905152;conn0;");
    setup(1, 0, 1, 0x1f, 64, 32); HUDDrawFrame__Fv(); check("overlay bit", std::string(head) + "conn0;");
    setup(1, 0, 1, 0x20, 33, -5); HUDDrawFrame__Fv();
    check("overlay odd", std::string(head) + "find hud.lmp,noconnect.tex;tex304,226,7,2155905152;conn0;");
    setup(1, 0, 1, 0x20, -33, 0); HUDDrawFrame__Fv();
    check("overlay negative", std::string(head) + "find hud.lmp,noconnect.tex;tex336,224,7,2155905152;conn0;");
    if (failures == 0) std::puts("hud_draw_frame host check passed");
    return failures != 0;
}
