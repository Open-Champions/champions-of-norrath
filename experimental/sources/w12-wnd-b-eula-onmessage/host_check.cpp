// Host check only; not a build input. Defines NORRATH_HOST_CONTRACT so the UI word translates to a
// host buffer. The 295 path must produce the full call trace and clear both words; every other
// combination must record nothing and leave the object untouched.
#define NORRATH_HOST_CONTRACT 1
#include "onmessage.cpp"
#include <cstdio>
#include <cstring>
#include <string>

unsigned _5VIWnd$mUI = 0x1234u;
static unsigned char ui_block[128];
static unsigned char self_object[10000], other[16];
static std::string trace;
static std::string off(const void* w) {
    return std::to_string(static_cast<const unsigned char*>(w) - self_object);
}

extern "C" {
char* norrath_host_translate(unsigned address) { return address == 0x1234u ? reinterpret_cast<char*>(ui_block) : nullptr; }
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
}

int main() {
    bool ok = true;
    const int ui48 = 77;
    std::memcpy(ui_block + 48, &ui48, 4);
    const std::string full =
        "close:40/1;init:40,4660,0,0,320,224,600,18,0;get:UI,1596,77;title:40,2596;open:40/0;";
    struct Case { int message; void* b; bool runs; };
    const Case cases[] = {{295, self_object, true}, {295, other, false}, {295, nullptr, false},
                          {294, self_object, false}, {296, self_object, false}, {0, self_object, false}};
    for (const Case& c : cases) {
        std::memset(self_object, 0x66, sizeof self_object);
        trace.clear();
        OnMessage__9VIWndEulaP5VIWndiii(self_object, other, c.message, other, c.b);
        ok = ok && trace == (c.runs ? full : "");
        for (unsigned i = 0; i < sizeof self_object; ++i) {
            const bool cleared = c.runs && ((i >= 9764 && i < 9768) || (i >= 1544 && i < 1548));
            if (self_object[i] != (cleared ? 0 : 0x66)) ok = false;
        }
    }
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
