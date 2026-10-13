// Host check only; not a build input. Build as x86_64 with statics below 4 GiB.
#include "onmessage.cpp"
#include <cstdio>
#include <string>

alignas(8) static unsigned char object[16832];
alignas(8) static int table[16];
unsigned _5VIWnd$mUI = static_cast<unsigned>(reinterpret_cast<unsigned long>(table));
static std::string trace;
static std::string off(const void* p) { return std::to_string(static_cast<const unsigned char*>(p) - object); }

extern "C" {
void Close__10VIUIWindowb(void* w, bool f) { trace += "close:" + off(w) + "/" + std::to_string(f) + ";"; }
void Init__10VIUIWindowP4VIUIRC8VIVect2IT2iii(void* w, unsigned ui, const int* p, const int* s, int a, int b, int c) {
    trace += "init:" + off(w) + "," + std::string(ui == _5VIWnd$mUI ? "ui" : "?") + "," + std::to_string(p[0]) + "," + std::to_string(p[1]) +
             "," + std::to_string(s[0]) + "," + std::to_string(s[1]) + "," + std::to_string(a) + "," + std::to_string(b) + "," + std::to_string(c) + ";";
}
void Open__10VIUIWindowb(void* w, bool f) { trace += "open:" + off(w) + "/" + std::to_string(f) + ";"; }
void SetTitle__10VIUIWindowPCUs(void* w, const unsigned short* t) { trace += "title:" + off(w) + "," + std::to_string(reinterpret_cast<unsigned long>(t)) + ";"; }
const unsigned short* GetString__13VIStringTableii(const void* t, int id, int col) {
    trace += std::string("gs:") + (t == table ? "ui" : "?") + "," + std::to_string(id) + "," + std::to_string(col) + ";";
    return reinterpret_cast<const unsigned short*>(0x4321);
}
}

int main() {
    bool ok = true;
    table[12] = 3;
    for (int msg : {0, 294, 296}) {
        trace.clear();
        OnMessage__16VIWndReadMessageP5VIWndiii(object, object + 1, msg, 7, object);
        ok = ok && trace.empty() && *reinterpret_cast<int*>(object + 16804) == 0;
    }
    OnMessage__16VIWndReadMessageP5VIWndiii(object, object + 1, 295, 7, object + 8);
    ok = ok && trace.empty() && *reinterpret_cast<int*>(object + 16804) == 0;
    OnMessage__16VIWndReadMessageP5VIWndiii(object, object + 1, 295, 7, object);
    ok = ok && trace == "close:40/1;init:40,ui,0,0,320,246,440,10,0;gs:ui,789,3;title:40,17185;open:40/0;" &&
         *reinterpret_cast<int*>(object + 16804) == 1;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
