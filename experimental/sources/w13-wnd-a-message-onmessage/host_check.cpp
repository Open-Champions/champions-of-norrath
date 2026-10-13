// Host check only; not a build input. Build as x86_64 with statics below 4 GiB.
#include "onmessage.cpp"
#include <cstdio>
#include <string>

unsigned _5VIWnd$mUI = 0x1234u;
alignas(8) static unsigned char object[1100];
static std::string trace;
static std::string off(const void* p) { return std::to_string(static_cast<const unsigned char*>(p) - object); }

extern "C" {
void Close__10VIUIWindowb(void* w, bool f) { trace += "close:" + off(w) + "/" + std::to_string(f) + ";"; }
void Init__10VIUIWindowP4VIUIRC8VIVect2IT2iii(void* w, unsigned ui, const int* p, const int* s, int a, int b, int c) {
    trace += "init:" + off(w) + "," + std::to_string(ui) + "," + std::to_string(p[0]) + "," + std::to_string(p[1]) + "," +
             off(s) + "," + std::to_string(a) + "," + std::to_string(b) + "," + std::to_string(c) + ";";
}
void Open__10VIUIWindowb(void* w, bool f) { trace += "open:" + off(w) + "/" + std::to_string(f) + ";"; }
const unsigned short* T__10VIUIWindowPCUs(void* w, const void* e) { trace += "T:" + off(w) + "," + off(e) + ";"; return reinterpret_cast<const unsigned short*>(0x4321); }
void SetTitle__10VIUIWindowPCUs(void* w, const unsigned short* t) { trace += "title:" + off(w) + "," + std::to_string(reinterpret_cast<unsigned long>(t)) + ";"; }
void OnMessage__5VIWndP5VIWndiii(void*, void*, int m, int p1, void* p2) { trace += "base:" + std::to_string(m) + "," + std::to_string(p1) + "," + off(p2) + ";"; }
}

int main() {
    bool ok = true;
    int* words = reinterpret_cast<int*>(object);
    words[249] = 11; words[250] = 12;
    for (int msg : {0, 294, 296}) {
        trace.clear();
        OnMessage__12VIWndMessageP5VIWndiii(object, object + 1, msg, 7, object);
        ok = ok && trace == "base:" + std::to_string(msg) + ",7,0;";
    }
    trace.clear();
    words[246] = 33;
    OnMessage__12VIWndMessageP5VIWndiii(object, object + 1, 295, 7, object + 8);
    ok = ok && trace.empty() && words[246] == 33;
    for (int w1004 : {0, 5}) {
        trace.clear();
        words[251] = w1004;
        OnMessage__12VIWndMessageP5VIWndiii(object, object + 1, 295, 7, object);
        ok = ok && trace == "close:40/1;init:40,4660,0,0,988,11,12,0;open:40/0;T:40,408;title:40,17185;" && words[246] == (w1004 != 0);
    }
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
