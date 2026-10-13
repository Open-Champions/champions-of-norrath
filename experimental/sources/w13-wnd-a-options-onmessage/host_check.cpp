// Host check only; not a build input. Build as x86_64 with statics below 4 GiB.
#include "onmessage.cpp"
#include <cstdio>
#include <cstring>
#include <string>

alignas(8) static unsigned char object[1400];
alignas(8) static int table[16];
alignas(8) static unsigned char options[160];
unsigned _5VIWnd$mUI = static_cast<unsigned>(reinterpret_cast<unsigned long>(table));
static std::string trace;
static void* mcerr = reinterpret_cast<void*>(0x2222220);
static std::string off(const void* p) {
    if (p == nullptr) return "null";
    if (p == mcerr) return "mc";
    const long d = static_cast<const unsigned char*>(p) - object;
    return d >= 0 && d < 1400 ? std::to_string(d) : "?";
}

extern "C" {
unsigned char* VIGetGameOptions__Fv() { trace += "opt;"; return options; }
void AddChild__5VIWndP5VIWnd(void* s, void* c) { trace += "add:" + off(s) + "," + off(c) + ";"; }
void RemoveChild__5VIWndP5VIWnd(void* s, void* c) { trace += "rm:" + off(s) + "," + off(c) + ";"; }
void SetFocus__5VIWndP5VIWndb(void* s, void* t, bool f) { trace += "focus:" + off(s) + "," + off(t) + "," + std::to_string(f) + ";"; }
void OnMessage__5VIWndP5VIWndiii(void* s, void* f, int m, void* p1, void* p2) {
    trace += "base:" + off(s) + "," + off(f) + "," + std::to_string(m) + "," + off(p1) + "," + off(p2) + ";";
}
void Close__10VIUIWindowb(void* w, bool f) { trace += "close:" + off(w) + "/" + std::to_string(f) + ";"; }
void Init__10VIUIWindowP4VIUIRC8VIVect2IT2iii(void* w, unsigned ui, const int* p, const int* s, int a, int b, int c) {
    trace += "init:" + off(w) + "," + std::string(ui == _5VIWnd$mUI ? "ui" : "?") + "," + std::to_string(p[0]) + "," + std::to_string(p[1]) + "," +
             std::to_string(s[0]) + "," + std::to_string(s[1]) + "," + std::to_string(a) + "," + std::to_string(b) + "," + std::to_string(c) + ";";
}
void Open__10VIUIWindowb(void* w, bool f) { trace += "open:" + off(w) + "/" + std::to_string(f) + ";"; }
void SetTitle__10VIUIWindowPCUs(void* w, const unsigned short* t) { trace += "title:" + off(w) + "," + std::to_string(reinterpret_cast<unsigned long>(t)) + ";"; }
const unsigned short* GetString__13VIStringTableii(const void* t, int id, int col) {
    trace += std::string("gs:") + (t == table ? "ui" : "?") + "," + std::to_string(id) + "," + std::to_string(col) + ";";
    return reinterpret_cast<const unsigned short*>(0x4321);
}
void* McErrWnd__Fv() { trace += "mc;"; return mcerr; }
void Unload__13VIBattleMusic(void* m) { trace += "unload:" + off(m) + ";"; }
}

static void reset() {
    std::memset(object, 0, sizeof object);
    for (int i = 0; i < 160; ++i) options[i] = static_cast<unsigned char>(i * 7 + 3);
    options[146] = 0; options[147] = 1; options[148] = 0;
    float a = 1.5f, b = 2.5f;
    std::memcpy(options + 8, &a, 4); std::memcpy(options + 12, &b, 4);
    table[12] = 3;
    trace.clear();
}
static int word(int offset) { int v; std::memcpy(&v, object + offset, 4); return v; }
static void set_word(int offset, int v) { std::memcpy(object + offset, &v, 4); }

int main() {
    bool ok = true;
    void* self = object;
    void* from = object + 1;
    reset(); OnMessage__12VIWndOptionsP5VIWndiii(self, from, 4352, nullptr, nullptr);
    ok = ok && trace == "opt;add:0,1;mc;" && word(1196) == static_cast<int>(reinterpret_cast<unsigned long>(mcerr));
    reset(); OnMessage__12VIWndOptionsP5VIWndiii(self, from, 4354, nullptr, nullptr);
    ok = ok && trace == "opt;" && word(1196) == static_cast<int>(reinterpret_cast<unsigned long>(from));
    reset(); OnMessage__12VIWndOptionsP5VIWndiii(self, from, 4356, nullptr, nullptr); ok = ok && trace == "opt;focus:0,1,1;";
    reset(); OnMessage__12VIWndOptionsP5VIWndiii(self, from, 259, nullptr, nullptr); ok = ok && trace == "opt;";
    reset(); OnMessage__12VIWndOptionsP5VIWndiii(self, from, 4353, nullptr, nullptr); ok = ok && trace == "opt;base:0,1,4353,null,null;";
    reset(); set_word(1184, 9); set_word(28, 9); set_word(20, 0x6000);
    OnMessage__12VIWndOptionsP5VIWndiii(self, from, 4358, nullptr, nullptr);
    ok = ok && trace == "opt;focus:0,0,1;focus:0,?,1;" && word(1184) == 0 && word(28) == 0;
    reset(); OnMessage__12VIWndOptionsP5VIWndiii(self, from, 4358, reinterpret_cast<void*>(1), nullptr); ok = ok && word(1188) == 4 && word(1184) == 0;
    reset(); OnMessage__12VIWndOptionsP5VIWndiii(self, from, 4358, reinterpret_cast<void*>(2), nullptr); ok = ok && word(1188) == 0;
    reset(); object[1174] = 0; object[1175] = 5; object[1176] = 0x42; object[1177] = 0x43;
    OnMessage__12VIWndOptionsP5VIWndiii(self, object + 1232, 307, nullptr, nullptr);
    ok = ok && trace == "opt;focus:0,0,1;" && options[147] == 1 && options[148] == 0 && options[149] == 0x42 && options[151] == 0x43 && word(1188) == 4;
    reset(); OnMessage__12VIWndOptionsP5VIWndiii(self, from, 307, nullptr, nullptr); ok = ok && trace == "opt;focus:0,0,1;" && word(1188) == 0;
    reset(); set_word(1196, 0x1c00900);
    OnMessage__12VIWndOptionsP5VIWndiii(self, from, 295, from, self);
    ok = ok && trace.find("opt;mc;close:40/1;init:40,ui,0,0,320,246,300,7,0;open:40/0;gs:ui,788,3;title:40,17185;rm:0,?;") == 0 &&
         word(1196) == 0 && word(1156) == options[6] && object[1160] == options[7] && object[1161] == options[4] && object[1162] == options[5] &&
         object[1172] == options[145] && object[1173] == 1 && object[1174] == 0 && object[1175] == 1 && object[1176] == options[149] &&
         object[1177] == options[151] && word(1152) == -1 && word(1144) == -1 && word(1260) == 0;
    reset(); OnMessage__12VIWndOptionsP5VIWndiii(self, from, 295, mcerr, self); ok = ok && trace == "opt;mc;";
    reset(); OnMessage__12VIWndOptionsP5VIWndiii(self, from, 295, self, mcerr); ok = ok && trace == "opt;mc;";
    reset(); OnMessage__12VIWndOptionsP5VIWndiii(self, from, 295, self, object + 4); ok = ok && trace == "opt;mc;unload:1200;";
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
