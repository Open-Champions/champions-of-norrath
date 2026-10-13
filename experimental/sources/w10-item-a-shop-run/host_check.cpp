// Host check for shopRun__Fi. Not an EE plan input.
#include "shop_run.cpp"
#include <string>
unsigned char player[4 * 23984];
unsigned char shop[232] __attribute__((aligned(4)));
unsigned char inventory[232] __attribute__((aligned(4)));
int frameNm = 77, shopActiveFrame, nmPlayers = 2, localNmPlayersAlive = 2, padAcceptButton = 0x40;
int fontObject;
void* defaultFont = &fontObject;
static std::string ev;
static int found = -1;
#define N(x) std::to_string(x)
extern "C" {
void displayGold__FP12playerStructP12screenStruct(unsigned char* r, unsigned char* s) { ev += "G" + N((r - player) / 23984) + ";"; }
void displayWeight__FP12playerStructP12screenStruct(unsigned char*, unsigned char*) { ev += "W;"; }
void displayItems__FiiP12screenStructi(int a, int b, unsigned char* s, int d) { ev += "I" + N(a) + N(b) + (s == shop + 116 * a ? "s" : s == inventory + 116 * a ? "i" : "?") + N(d) + ";"; }
void doHelpfulMessage__FiiP12screenStructiiPQ24item11description(int a, int b, unsigned char* s, int d, int e, void* f) { ev += "H" + N(a) + N(b) + (s == shop + 116 * a ? "s" : "i") + N(d) + N(e) + N(f != nullptr) + ";"; }
void mainMenu__Fii(int a, int b) { ev += "M" + N(a) + N(b) + ";"; }
int inventoryPadFindButton__Fi(int a) { ev += "F" + N(a) + ";"; return found; }
signed char netGetLocalPlayerNm__Fi(int a) { ev += "L" + N(a) + ";"; return 5; }
void msgHudSetPlayerBusy__Fcc(signed char a, signed char b) { ev += "B" + N(a) + N(b) + ";"; }
void shopKick__Fv() { ev += "K;"; }
void shopUnpackToGrid__Fiii(int a, int b, int c) { ev += "U" + N(a) + N(b) + N(c) + "o" + N(*reinterpret_cast<int*>(shop + 116 * b + 52)) + ";"; }
void textureDrawBorder__FiiiiiUiPcii(int a, int b, int c, int d, int e, unsigned f, char* g, int h, int i) { ev += "T" + N(a) + "," + N(b) + "," + N(c) + "," + N(d) + "," + N(e) + "," + N(f) + "," + N(g != nullptr) + N(h) + N(i) + ";"; }
void fontStart__FP4FontiUii(void* f, int a, unsigned b, int c) { ev += "S" + N(f == defaultFont) + N(a) + "," + N(b) + "," + N(c) + ";"; }
void fontEnd__Fv() { ev += "E;"; }
unsigned short* textGetString__FPc(char* k) { ev += std::string("K=") + k + ";"; static unsigned short t[4] = {'h', 'i', 0}; return t; }
void usprintf__FPUsT0ffffffff(unsigned short* d, unsigned short* s, float, float, float, float, float, float, float, float) { while ((*d++ = *s++)) {} ev += "P;"; }
void fontPrintCenteredu__FiiPUsi(int x, int y, unsigned short* t, int w) { ev += "C" + N(x) + "," + N(y) + "," + N(t[0]) + "," + N(w) + ";"; }
}
static int& W(int screen, int off) { return *reinterpret_cast<int*>(shop + 116 * screen + off); }
#include <cstdio>
#define CHK(c, n) if (c) { printf("%s\n", ev.c_str()); return n; }
int main() {
    W(0, 52) = 1; W(0, 76) = 0;
    shopRun__Fi(0);
    CHK(ev != "G1;W;I01s0;H01s000;" || shopActiveFrame != 77, 1)
    ev.clear(); W(0, 76) = 1; shopRun__Fi(0);
    CHK(ev != "G1;W;I01i1;H01i100;", 2)
    ev.clear(); W(0, 76) = 3; shopRun__Fi(0);
    CHK(ev != "G1;W;H01s-200;M01;", 3)
    ev.clear(); W(0, 76) = 9; shopRun__Fi(0);
    CHK(ev != "G1;W;", 4)
    // mode 2: both screens in mode 2 -> shutdown
    W(0, 76) = 2; W(1, 76) = 2; ev.clear(); shopRun__Fi(0);
    CHK(ev != "F64;L0;B50;K;", 5)
    // mode 2, other screen not in mode 2, a new player joins
    W(1, 76) = 0; W(1, 52) = 3; W(0, 52) = 0; found = 2; ev.clear();
    W(0, 36) = 9; W(0, 64) = 9;
    shopRun__Fi(0);
    CHK(ev != "F64;U201o2;" || W(0, 76) != 3 || W(0, 52) != 2 || W(0, 36) != 0 || W(0, 64) != 0, 6)
    // the same player as the other screen closes this one; nobody else is active so no message
    W(0, 76) = 2; ev.clear(); found = 3; shopRun__Fi(0);
    CHK(ev != "F64;" || W(0, 52) != -1, 7)
    // a different player already on one of the screens: nothing
    W(0, 52) = 0; ev.clear(); found = 0; shopRun__Fi(0);
    CHK(ev != "F64;" || W(0, 52) != 0, 8)
    // close with an active other player: message drawn
    *reinterpret_cast<float*>(player + 3 * 23984 + 172) = 2.0f;
    W(0, 52) = 0; W(0, 0) = 320; W(0, 8) = 101; W(0, 12) = 100; ev.clear(); found = -1; shopRun__Fi(0);
    CHK(ev != "F64;T350,100,270,51,4,2153866089,000;S15,2150775868,-1;K=PressEnterShop;P;C370,110,104,250;E;" , 9)
    // localNmPlayersAlive below 2 suppresses it
    localNmPlayersAlive = 1; W(0, 52) = 0; ev.clear(); shopRun__Fi(0);
    CHK(ev != "F64;", 10)
    nmPlayers = 1; ev.clear(); shopRun__Fi(0);
    CHK(ev != "F64;L0;B50;K;", 11)
    return 0;
}
