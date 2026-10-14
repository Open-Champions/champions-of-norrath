// Host check for showNetPlayerStatus__Fv. Not an EE plan input.
// Build on the host with: c++ -std=c++17 -fno-builtin host_check.cpp
// Wide strings are modelled as the 16-bit units of the key text.
#include "show_net_player_status.cpp"
#include <cstdio>
#include <cstring>
#include <string>

extern "C" {
alignas(8) unsigned char netGameLaunchInfo[0x148];
alignas(8) unsigned char launchPeers[1008];
unsigned short hudFooterUni[128];
Font* smallFont = reinterpret_cast<Font*>(0x10);
int padAcceptButtonFontChar = 11;
int padBackButtonFontChar = 22;
signed char MC_Debug;
signed char hostStartedWithoutMe;
int netLaunchExpirationCount;
int netLaunchTimer;
}
static std::string ev;
static std::string wide(unsigned short* s) { std::string r; for (; *s; ++s) r += (char)*s; return r; }
static unsigned short key_text[16][64];
static int key_count;
extern "C" unsigned short* textGetString__FPc(char* key) {
    unsigned short* t = key_text[key_count++ % 16];
    int n = 0; for (; key[n]; ++n) t[n] = (unsigned short)key[n];
    t[n] = 0; return t;
}
extern "C" void fontStart__FP4FontiUii(Font*, int s, unsigned c, int) { ev += "S" + std::to_string(s) + ":" + std::to_string(c) + ";"; }
extern "C" void fontEnd__Fv() { ev += "E;"; }
extern "C" void fontColor__FUiii(unsigned c, int, int) { ev += "C" + std::to_string(c) + ";"; }
extern "C" void fontPrintCenteredu__FiiPUsi(int x, int y, unsigned short* t, int) {
    ev += "U" + std::to_string(x) + "," + std::to_string(y) + "=" + wide(t) + ";";
}
extern "C" void fontPrintCentered__FiiPci(int x, int y, char* t, int) {
    ev += "P" + std::to_string(x) + "," + std::to_string(y) + "=" + t + ";";
}
extern "C" void fontPrintWrapCenteredu__FiiPUsiiii(int x, int y, unsigned short* t, int a, int b, int c, int d) {
    ev += "W" + std::to_string(x) + "," + std::to_string(y) + "=" + wide(t) + "," + std::to_string(a) + std::to_string(b) + "," + std::to_string(c) + "," + std::to_string(d) + ";";
}
extern "C" void textureDrawBorder__FiiiiiUiPcii(int x, int, int, int, int, unsigned, char*, int, int) { ev += "B" + std::to_string(x) + ";"; }
extern "C" void textureDrawColor__FP8_textureiiiUi(Texture*, int x, int y, int, unsigned) { ev += "T" + std::to_string(x) + "," + std::to_string(y) + ";"; }
extern "C" Texture* lumpFindResource__FPcT0(char* g, char* n) { ev += std::string("L") + g + "/" + n + ";"; return 0; }
extern "C" void usprintf__FPUsT0ffffffff(unsigned short* out, unsigned short* f, float a, float b, float, float, float, float, float, float) {
    std::string s = wide(f) + "(" + std::to_string((int)a) + "," + std::to_string((int)b) + ")";
    for (size_t i = 0; i < s.size(); ++i) out[i] = (unsigned short)s[i];
    out[s.size()] = 0;
}
extern "C" void ustrcpy__FPUsT0(unsigned short* d, unsigned short* s) { while ((*d++ = *s++)) {} }
extern "C" void hudShowMsg__FPUsN20iii(unsigned short* t, unsigned short* s, unsigned short* f, int a, int x, int y) {
    ev += "H" + wide(t) + "|" + (s ? "x" : "null") + "|" + wide(f) + "|" + std::to_string(a) + "," + std::to_string(x) + "," + std::to_string(y) + ";";
}
extern "C" int sprintf(char* d, const char* f, ...) {
    // Only the two forms used: a plain copy of the format, or "%d" with an int.
    if (std::strcmp(f, "%d") == 0 || std::strcmp(f, "dev fail=%d\n") == 0) {
        __builtin_va_list ap; __builtin_va_start(ap, f);
        int v = __builtin_va_arg(ap, int); __builtin_va_end(ap);
        return std::snprintf(d, 128, f, v);
    }
    std::strcpy(d, f); return (int)std::strlen(d);
}
extern "C" unsigned long strlen(const char* t) { unsigned long n = 0; while (t[n]) ++n; return n; }
extern "C" char* strcat(char* d, const char* s) { char* r = d; while (*d) ++d; while ((*d++ = *s++)) {} return r; }

static void peer(int i, int state, int cls, const char* name, int gold, int level, int done, int total) {
    unsigned char* p = launchPeers + 252 * i;
    p[92] = (unsigned char)state; p[136] = (unsigned char)cls;
    std::strcpy((char*)p + 104, name);
    *(int*)(p + 148) = gold; *(int*)(p + 152) = level; *(int*)(p + 244) = done; *(int*)(p + 248) = total;
}
int main() {
    // Host started without me: one wrapped line, then the footer.
    hostStartedWithoutMe = 1; netGameLaunchInfo[4] = 2; netLaunchTimer = 0;
    showNetPlayerStatus__Fv();
    if (ev.find("W320,200=netLaunchStartedWithoutMe,400-100,580,21;E;") == std::string::npos) return 1;
    if (ev.find("HnetLaunchTitle|null|MemcardBack(22,0)|128,460,284;") == std::string::npos) return 2;
    // Two entries, local index 1 and host index 1: entry 0 transfers 50 of 200, entry 1 is the local one.
    ev.clear(); hostStartedWithoutMe = 0; netLaunchExpirationCount = 0; MC_Debug = 0;
    netGameLaunchInfo[4] = 2; netGameLaunchInfo[5] = 1; netGameLaunchInfo[6] = 1;
    peer(0, 3, 2, "Abcdefghijklmnop", 123, 7, 50, 200);
    peer(1, 7, 5, "Hi", 5, 6, 0, 1);
    showNetPlayerStatus__Fv();
    // x positions: 339 - 148 = 191 and 339; centre = x + 54.
    if (ev.find("B191;") == std::string::npos || ev.find("B339;") == std::string::npos) return 3;
    if (ev.find("U245,70=netLaunchHost;") == std::string::npos) return 4;
    if (ev.find("U393,70=netLaunchOther(2,0);") == std::string::npos) return 5;
    // Entry 0 has a state, so its portrait and shortened name are drawn.
    if (ev.find("Lhud.lmp/b_wem.tex;T195,104;") == std::string::npos) return 6;
    if (ev.find("P245,211=Abcdefghijk...;") == std::string::npos) return 7;
    if (ev.find("P245,262=7;") == std::string::npos || ev.find("P245,296=123;") == std::string::npos) return 8;
    if (ev.find("U245,330=netLaunchTransfer(25,0);") == std::string::npos) return 9;
    // The local entry (index 1) shows ready and, as host, enables the prompt.
    if (ev.find("U393,330=netLaunchReady;") == std::string::npos) return 10;
    if (ev.find("HnetLaunchTitle|null|MemcardBack(22,0)|128,460,284;") == std::string::npos) return 11;
    // A ready remote entry seen by the host (index 5 equals index 6) turns on the prompt.
    ev.clear();
    peer(0, 8, 2, "A", 1, 1, 0, 1);
    showNetPlayerStatus__Fv();
    if (ev.find("U245,330=netLaunchReady;") == std::string::npos) return 12;
    if (ev.find("HnetLaunchTitle|null|netLaunchHostPrompt(11,22)|128,460,284;") == std::string::npos) return 13;
    // The timer text wins over the prompt.
    ev.clear(); netLaunchTimer = 4;
    showNetPlayerStatus__Fv();
    if (ev.find("HnetLaunchTitle|null|netLaunchStarting(22,0)|128,460,284;") == std::string::npos) return 14;
    return 0;
}
