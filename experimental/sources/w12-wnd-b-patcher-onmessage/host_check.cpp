// Host check only; not a build input. Defines NORRATH_HOST_CONTRACT so the UI word translates to a
// host buffer. Every callee records its arguments; the setup path, the pending-flag path, the
// message-307 state paths and the pass-through path are each compared with a written-out trace and
// the expected object words.
#define NORRATH_HOST_CONTRACT 1
#include "onmessage.cpp"
#include <cstdio>
#include <cstring>
#include <string>

unsigned _5VIWnd$mUI = 0x1234u;
static unsigned char ui_block[128];
static unsigned char self_object[2400], other[16];
static std::string trace;
static std::string off(const void* w) {
    return std::to_string(static_cast<const unsigned char*>(w) - self_object);
}
static std::string who(const void* p) { return p == self_object ? "T" : p == other ? "O" : p == nullptr ? "0" : "?"; }

extern "C" {
char* norrath_host_translate(unsigned address) { return address == 0x1234u ? reinterpret_cast<char*>(ui_block) : nullptr; }
void OnMessage__5VIWndP5VIWndiii(void* self, void* from, int message, void* a, void* b) {
    trace += "base:" + off(self) + "," + who(from) + "," + std::to_string(message) + "," + who(a) + "," + who(b) + ";";
}
void SetFocus__5VIWndP5VIWndb(void* self, void* window, bool force) {
    trace += "focus:" + off(self) + "," + who(window) + "," + std::to_string(force) + ";";
}
void Clear__16VIWndReadMessage(void* w) { trace += "clear:" + off(w) + ";"; }
void Init__10VIUIWindowP4VIUIRC8VIVect2IT2iii(void* w, unsigned ui, const int* p, const int* s, int a, int b, int c) {
    trace += "init:" + off(w) + "," + std::to_string(ui) + "," + std::to_string(p[0]) + "," + std::to_string(p[1]) + "," +
             std::to_string(s[0]) + "," + std::to_string(s[1]) + "," + std::to_string(a) + "," + std::to_string(b) + "," +
             std::to_string(c) + ";";
}
void Open__10VIUIWindowb(void* w, bool f) { trace += "open:" + off(w) + "/" + std::to_string(f) + ";"; }
void Close__10VIUIWindowb(void* w, bool f) { trace += "close:" + off(w) + "/" + std::to_string(f) + ";"; }
void SetTitle__10VIUIWindowPCUs(void* w, const unsigned short* t) { trace += "title:" + off(w) + "," + std::to_string(t[0]) + ";"; }
const unsigned short* GetString__13VIStringTableii(void* table, int id, int arg) {
    static unsigned short text[2];
    trace += std::string("get:") + (table == ui_block ? "UI" : "?") + "," + std::to_string(id) + "," + std::to_string(arg) + ";";
    text[0] = static_cast<unsigned short>(id + 1000);
    return text;
}
}

static int word(int offset) { int v; std::memcpy(&v, self_object + offset, 4); return v; }
static void set(int offset, int v) { std::memcpy(self_object + offset, &v, 4); }
static void fresh() { std::memset(self_object, 0x66, sizeof self_object); trace.clear(); }

int main() {
    bool ok = true;
    const int ui48 = 77;
    std::memcpy(ui_block + 48, &ui48, 4);
    const std::string setup =
        "init:40,4660,0,350,304,360,440,1,0;open:40/1;close:776/1;init:776,4660,0,0,320,246,430,4,0;open:776/0;"
        "get:UI,811,77;title:776,1811;init:408,4660,0,0,320,246,480,6,0;open:408/0;get:UI,767,77;title:408,1767;"
        "clear:2312;";
    // 295, b == this, a unrelated or null: setup path.
    for (void* a : {static_cast<void*>(other), static_cast<void*>(nullptr)}) {
        fresh();
        OnMessage__12VIWndPatcherP5VIWndiii(self_object, other, 295, a, self_object);
        ok = ok && trace == setup && word(1144) == 0 && word(1152) == 0 && word(1160) == -1 && word(1156) == 0 &&
             word(1164) == 0 && word(1168) == 0 && word(1148) == 0x66666666;
    }
    // 295, b != this: nothing.
    fresh();
    OnMessage__12VIWndPatcherP5VIWndiii(self_object, other, 295, self_object + 2312, other);
    ok = ok && trace.empty() && word(1168) == 0x66666666;
    // a == this + 1176: skips setup and, being not the message window, does nothing.
    fresh();
    OnMessage__12VIWndPatcherP5VIWndiii(self_object, other, 295, self_object + 1176, self_object);
    ok = ok && trace.empty() && word(1156) == 0x66666666;
    // a == message window: pending flag.
    for (int pending : {0, 1, 2}) {
        for (int latched : {0, 5}) {
            fresh();
            set(1156, pending);
            set(1168, latched);
            OnMessage__12VIWndPatcherP5VIWndiii(self_object, other, 295, self_object + 2312, self_object);
            const bool set_it = pending == 1 && latched == 0;
            ok = ok && trace.empty() && word(1168) == (set_it ? 1 : latched) && word(1164) == (set_it ? 1 : 0x66666666);
        }
    }
    // 307.
    struct Row { int state; int w2160; const char* want; int w1148; };
    const Row rows[] = {{0, 0, "focus:0,T,1;", 0x66666666}, {0, 1, "focus:0,T,1;", 0x66666666},
                        {1, 1, "focus:0,T,1;", 0x66666666}, {1, 0, "focus:0,0,1;", -1},
                        {1, 2, "focus:0,0,1;", -1}, {2, 1, "", 0x66666666}, {-1, 1, "", 0x66666666}};
    for (const Row& r : rows) {
        fresh();
        set(1160, r.state);
        set(2160, r.w2160);
        OnMessage__12VIWndPatcherP5VIWndiii(self_object, other, 307, other, self_object);
        ok = ok && trace == r.want && word(1148) == r.w1148;
    }
    // Other messages pass through unchanged.
    for (int message : {0, 294, 296, 306, 308}) {
        fresh();
        OnMessage__12VIWndPatcherP5VIWndiii(self_object, other, message, other, self_object);
        ok = ok && trace == "base:0,O," + std::to_string(message) + ",O,T;";
    }
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
