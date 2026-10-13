// Host check only; not a build input. Defines NORRATH_HOST_CONTRACT so the UI word translates to a
// host buffer. Every callee records its arguments; the expected trace is written out in full for
// the message-295 path, and the skip paths must record nothing (or only the base-class call).
#define NORRATH_HOST_CONTRACT 1
#include "onmessage.cpp"
#include <cstdio>
#include <cstring>
#include <string>

unsigned _5VIWnd$mUI = 0x1234u;
static unsigned char ui_block[128];
static unsigned char self_object[2048], other[16];
static std::string trace;
static std::string off(const void* w) {
    return std::to_string(static_cast<const unsigned char*>(w) - self_object);
}

extern "C" {
char* norrath_host_translate(unsigned address) { return address == 0x1234u ? reinterpret_cast<char*>(ui_block) : nullptr; }
void OnMessage__5VIWndP5VIWndiii(void* self, void*, int message, void* a, void* b) {
    trace += "base:" + off(self) + "," + std::to_string(message) + "," + (a == other ? "O" : "?") + "," + (b == other ? "O" : "?") + ";";
}
void Init__10VIUIWindowP4VIUIRC8VIVect2IT2iii(void* w, unsigned ui, const int* p, const int* s, int a, int b, int c) {
    trace += "init:" + off(w) + "," + std::to_string(ui) + "," + std::to_string(p[0]) + "," + std::to_string(p[1]) + "," +
             std::to_string(s[0]) + "," + std::to_string(s[1]) + "," + std::to_string(a) + "," + std::to_string(b) + "," +
             std::to_string(c) + ";";
}
void Open__10VIUIWindowb(void* w, bool f) { trace += "open:" + off(w) + "/" + std::to_string(f) + ";"; }
void Close__10VIUIWindowb(void* w, bool f) { trace += "close:" + off(w) + "/" + std::to_string(f) + ";"; }
void SetTitle__10VIUIWindowPCUs(void* w, const unsigned short* t) {
    trace += "title:" + off(w) + "," + std::to_string(t[0]) + ";";
}
const unsigned short* GetString__13VIStringTableii(void* table, int id, int arg) {
    static unsigned short text[2];
    trace += std::string("get:") + (table == ui_block ? "UI" : "?") + "," + std::to_string(id) + "," + std::to_string(arg) + ";";
    text[0] = static_cast<unsigned short>(id + 1000);
    return text;
}
void* memset(void* d, int v, unsigned long n) {
    trace += "memset:" + off(d) + "," + std::to_string(v) + "," + std::to_string(n) + ";";
    unsigned char* p = static_cast<unsigned char*>(d);
    for (unsigned long i = 0; i < n; ++i) p[i] = static_cast<unsigned char>(v);
    return d;
}
}

int main() {
    bool ok = true;
    const int ui48 = 77;
    std::memcpy(ui_block + 48, &ui48, 4);
    const std::string full =
        "init:40,4660,0,350,304,360,440,1,0;open:40/1;close:784/1;init:784,4660,0,0,320,246,470,5,0;open:784/0;"
        "get:UI,761,77;title:784,1761;init:408,4660,0,0,320,246,480,6,0;open:408/0;get:UI,767,77;title:408,1767;"
        "memset:1152,0,16;";
    for (int message : {0, 294, 296}) {
        std::memset(self_object, 0x66, sizeof self_object);
        trace.clear();
        OnMessage__9VIWndDnasP5VIWndiii(self_object, other, message, other, self_object);
        ok = ok && trace == "base:0," + std::to_string(message) + ",O,?;";
    }
    // 295: needs b == this and a != this + 1176.
    struct Case { void* a; void* b; bool full; };
    const Case cases[] = {{other, self_object, true},
                          {self_object, self_object, true},
                          {self_object + 1176, self_object, false},
                          {other, other, false},
                          {other, nullptr, false}};
    for (const Case& c : cases) {
        std::memset(self_object, 0x66, sizeof self_object);
        trace.clear();
        OnMessage__9VIWndDnasP5VIWndiii(self_object, other, 295, c.a, c.b);
        ok = ok && trace == (c.full ? full : "");
        unsigned word;
        std::memcpy(&word, self_object + 1168, 4);
        ok = ok && word == (c.full ? 0u : 0x66666666u);
        for (int i = 1152; i < 1168; ++i) ok = ok && self_object[i] == (c.full ? 0 : 0x66);
    }
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
