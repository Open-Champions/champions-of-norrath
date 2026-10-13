// Host check only; not a build input. Build as x86_64 with statics below 4 GiB.
#include "onmessage.cpp"
#include <cstdio>
#include <string>

alignas(8) static int object[600];
alignas(8) static int table[16];
unsigned _5VIWnd$mUI = static_cast<unsigned>(reinterpret_cast<unsigned long>(table));
static std::string trace;
static void* mcerr = reinterpret_cast<void*>(0x2222220);
static std::string off(const void* p) {
    if (p == nullptr) return "null";
    if (p == mcerr) return "mc";
    const long d = static_cast<const char*>(p) - reinterpret_cast<const char*>(object);
    return d >= 0 && d < 2400 ? std::to_string(d) : "?";
}

extern "C" {
void AddChild__5VIWndP5VIWnd(void* s, void* c) { trace += "add:" + off(s) + "," + off(c) + ";"; }
void RemoveChild__5VIWndP5VIWnd(void* s, void* c) { trace += "rm:" + off(s) + "," + off(c) + ";"; }
void SetFocus__5VIWndP5VIWndb(void* s, void* t, bool f) { trace += "focus:" + off(s) + "," + off(t) + "," + std::to_string(f) + ";"; }
void OnMessage__5VIWndP5VIWndiii(void* s, void* f, int m, void* p1, void* p2) {
    trace += "base:" + off(s) + "," + off(f) + "," + std::to_string(m) + "," + off(p1) + "," + off(p2) + ";";
}
void SoundPlay__4VIUI11VIUISoundId(const void* ui, int s) { trace += std::string("snd:") + (ui == table ? "ui" : "?") + "," + std::to_string(s) + ";"; }
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
}

static void reset() {
    for (int& w : object) w = 0;
    object[478] = 0x1c00900; object[490] = 55; object[489] = 66; object[481] = 1;
    table[12] = 3;
    trace.clear();
}

int main() {
    bool ok = true;
    void* self = object;
    void* from = object + 1;
    reset(); OnMessage__12VIWndConnectP5VIWndiii(self, from, 4352, nullptr, nullptr); ok = ok && trace == "add:0,4;";
    reset(); OnMessage__12VIWndConnectP5VIWndiii(self, from, 4354, nullptr, nullptr); ok = ok && trace.empty() && object[478] == static_cast<int>(reinterpret_cast<unsigned long>(from));
    reset(); OnMessage__12VIWndConnectP5VIWndiii(self, from, 307, nullptr, nullptr); ok = ok && trace == "focus:0,0,1;";
    reset(); OnMessage__12VIWndConnectP5VIWndiii(self, from, 4356, nullptr, nullptr); ok = ok && trace == "focus:0,4,1;";
    reset(); OnMessage__12VIWndConnectP5VIWndiii(self, from, 259, nullptr, nullptr); ok = ok && trace.empty();
    reset(); OnMessage__12VIWndConnectP5VIWndiii(self, from, 4353, nullptr, nullptr); ok = ok && trace == "base:0,4,4353,null,null;";
    reset(); OnMessage__12VIWndConnectP5VIWndiii(self, from, 4358, reinterpret_cast<void*>(5), nullptr);
    ok = ok && trace == "focus:0,0,1;" && object[489] == 3;
    reset(); object[5] = static_cast<int>(reinterpret_cast<unsigned long>(object + 100)); object[7] = 9; object[480] = 4;
    OnMessage__12VIWndConnectP5VIWndiii(self, from, 4358, reinterpret_cast<void*>(3), nullptr);
    ok = ok && trace == "snd:ui,8;focus:0,400,1;" && object[490] == 1 && object[7] == 0 && object[480] == 0 && object[489] == 66;
    reset(); OnMessage__12VIWndConnectP5VIWndiii(self, from, 295, nullptr, object + 2); ok = ok && trace.empty() && object[490] == 55;
    for (void* p1 : {mcerr, static_cast<void*>(object + 5)}) {
        reset(); OnMessage__12VIWndConnectP5VIWndiii(self, from, 295, p1, self);
        const bool same = p1 == mcerr;
        const std::string want = std::string("close:1176/1;init:1176,ui,0,0,320,246,400,5,0;mc;open:1176/") + (same ? "1" : "0") +
                                 ";gs:ui,815,3;title:1176,17185;mc;" + (same ? "" : "rm:0,?;");
        ok = ok && trace == want && object[490] == 0 && object[481] == 0 && object[478] == (same ? 0x1c00900 : 0);
    }
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
