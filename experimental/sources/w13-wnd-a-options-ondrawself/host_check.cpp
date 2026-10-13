// Host check only; not a build input. Build as x86_64 with statics below 4 GiB.
#include "ondrawself.cpp"
#include <cstdio>
#include <cstring>
#include <string>

alignas(8) static unsigned char object[1300];
alignas(8) static int table[16];
alignas(8) static unsigned char options[160];
unsigned _5VIWnd$mUI = static_cast<unsigned>(reinterpret_cast<unsigned long>(table));
unsigned _5VIWnd$mFocus;
unsigned sBattleMusicText[10], sSoundOutputText[2], sCompactKeyboardText[2], sOnOffText[2], sCameraTypeText[2], sCameraAxisText[2];
static std::string trace;
static unsigned flags;
static std::string off(const void* p) {
    const long d = static_cast<const unsigned char*>(p) - object;
    return d >= 0 && d < 1300 ? std::to_string(d) : "?";
}

extern "C" {
unsigned char* VIGetGameOptions__Fv() { trace += "opt;"; return options; }
unsigned VIGetFeatureFlags__Fv() { trace += "flags;"; return flags; }
void RemoveChild__5VIWndP5VIWnd(void* s, void* c) { trace += "rm:" + off(s) + "," + std::to_string(reinterpret_cast<unsigned long>(c)) + ";"; }
void ResetTickPulse__10VIUIWindow(void* w) { trace += "tick:" + off(w) + ";"; }
void Play__13VIBattleMusici(void* m, int t) { trace += "play:" + off(m) + "," + std::to_string(t) + ";"; }
void Stop__13VIBattleMusicb(void* m, bool f) { trace += "stop:" + off(m) + "," + std::to_string(f) + ";"; }
void StopNow__13VIBattleMusic(void* m) { trace += "stopnow:" + off(m) + ";"; }
void Process__13VIBattleMusic(void* m) { trace += "process:" + off(m) + ";"; }
void Begin2D__4VIUI(void*) { trace += "begin;"; }
void End2D__4VIUI(void*) { trace += "end;"; }
void DrawMainMenu__12VIWndOptions(void*) { trace += "main;"; }
void DrawGameOptions__12VIWndOptions(void*) { trace += "game;"; }
void DrawInterfaceOptions__12VIWndOptions(void*) { trace += "iface;"; }
void DrawSoundOptions__12VIWndOptions(void*) { trace += "sound;"; }
void SaveAndExit__12VIWndOptions(void*) { trace += "save;"; }
void Init__10VIUIWindowP4VIUIRC8VIVect2IT2iii(void* w, unsigned ui, const int* p, const int* s, int a, int b, int c) {
    trace += "init:" + off(w) + "," + std::string(ui == _5VIWnd$mUI ? "ui" : "?") + "," + std::to_string(p[0]) + "," + std::to_string(p[1]) + "," +
             std::to_string(s[0]) + "," + std::to_string(s[1]) + "," + std::to_string(a) + "," + std::to_string(b) + "," + std::to_string(c) + ";";
}
void Open__10VIUIWindowb(void* w, bool f) { trace += "open:" + off(w) + "/" + std::to_string(f) + ";"; }
void Draw__10VIUIWindow(void* w) { trace += "draw:" + off(w) + ";"; }
void TextBox__10VIUIWindowPCUsiiii9VIUICOLORUi(void* w, const unsigned short* t, int a, int b, int c, int d, int color, unsigned e) {
    trace += "tb:" + off(w) + "," + std::to_string(reinterpret_cast<unsigned long>(t)) + "," + std::to_string(a) + "," + std::to_string(b) + "," +
             std::to_string(c) + "," + std::to_string(d) + "," + std::to_string(color) + "," + std::to_string(e) + ";";
}
const unsigned short* T__10VIUIWindowPCc(void* w, const char* k) { trace += "T:" + off(w) + "," + k + ";"; return reinterpret_cast<const unsigned short*>(9000); }
const unsigned short* GetString__13VIStringTableii(const void* t, int id, int col) {
    trace += std::string("gs:") + (t == table ? "ui" : "?") + "," + std::to_string(id) + "," + std::to_string(col) + ";";
    return reinterpret_cast<const unsigned short*>(id);
}
}

static int word(int offset) { int v; std::memcpy(&v, object + offset, 4); return v; }
static void set_word(int offset, int v) { std::memcpy(object + offset, &v, 4); }
static void reset() {
    std::memset(object, 0, sizeof object);
    for (int i = 0; i < 160; ++i) options[i] = static_cast<unsigned char>(i * 7 + 3);
    table[12] = 3;
    _5VIWnd$mFocus = static_cast<unsigned>(reinterpret_cast<unsigned long>(object));
    sBattleMusicText[0] = 1;
    trace.clear();
    flags = 0;
}

int main() {
    bool ok = true;
    reset(); set_word(1144, 3);
    OnDrawSelf__12VIWndOptions(object);
    ok = ok && trace == "opt;stopnow:1200;process:1200;begin;main;end;";
    reset(); set_word(1144, 0); _5VIWnd$mFocus = 5;
    OnDrawSelf__12VIWndOptions(object);
    ok = ok && trace == "opt;tick:40;stopnow:1200;process:1200;begin;main;game;end;";
    for (int mode : {1, 2}) {
        reset(); set_word(1144, mode); options[4] = 3;
        OnDrawSelf__12VIWndOptions(object);
        ok = ok && trace == std::string("opt;") + (mode == 1 ? "stopnow:1200;process:1200;begin;main;iface;end;" : "flags;play:1200,3;process:1200;begin;main;sound;end;");
    }
    reset(); set_word(1144, 2); options[4] = 9; set_word(1204, 9); flags = 0x100;
    OnDrawSelf__12VIWndOptions(object);
    ok = ok && trace == "opt;flags;stop:1200,0;process:1200;begin;main;sound;end;";
    reset(); set_word(1144, 2); options[4] = 9; set_word(1204, 9);
    OnDrawSelf__12VIWndOptions(object);
    ok = ok && trace == "opt;flags;play:1200,-1;process:1200;begin;main;sound;end;";
    reset(); set_word(1196, 77);
    OnDrawSelf__12VIWndOptions(object);
    ok = ok && trace.find("opt;rm:0,77;") == 0 && word(1196) == 0;
    reset(); set_word(1196, 77); set_word(1184, 1);
    OnDrawSelf__12VIWndOptions(object);
    ok = ok && trace.find("opt;rm") == std::string::npos && word(1196) == 77;
    reset(); sBattleMusicText[0] = 0; flags = 0x100;
    OnDrawSelf__12VIWndOptions(object);
    ok = ok && sBattleMusicText[0] == 517 && sBattleMusicText[9] == 1630 && sSoundOutputText[1] == 804 && sCompactKeyboardText[0] == 802 &&
         sOnOffText[1] == 551 && sCameraTypeText[1] == 68 && sCameraAxisText[1] == 779 && sBattleMusicText[3] == 797;
    reset(); sBattleMusicText[0] = 0; for (unsigned& v : sBattleMusicText) v = 0; sBattleMusicText[5] = 55;
    OnDrawSelf__12VIWndOptions(object);
    ok = ok && sBattleMusicText[0] == 517 && sBattleMusicText[2] == 796 && sBattleMusicText[5] == 55 && sCameraAxisText[0] == 784;
    reset(); set_word(1188, 2); set_word(52, 11); set_word(56, 22); set_word(776 + 20, 500);
    OnDrawSelf__12VIWndOptions(object);
    ok = ok && trace.find("init:776,ui,0,0,11,22,400,3,0;open:776/1;draw:776;gs:ui,1615,3;tb:776,1615,40,0,420,3,0,8;") != std::string::npos &&
         trace.find("save;") == std::string::npos && word(1188) == 1;
    reset(); set_word(1188, 1);
    OnDrawSelf__12VIWndOptions(object);
    ok = ok && trace.find("save;") != std::string::npos && word(1188) == 0;
    reset(); set_word(1192, 1); set_word(776 + 20, 500);
    OnDrawSelf__12VIWndOptions(object);
    ok = ok && trace.find("init:776,ui,0,0,") != std::string::npos && trace.find(",440,6,0;open:776/1;draw:776;gs:ui,1616,3;tb:776,1616,40,0,420,4,0,8;T:776,|qOk|w;tb:776,9000,40,5,420,1,0,8;end;") != std::string::npos;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
