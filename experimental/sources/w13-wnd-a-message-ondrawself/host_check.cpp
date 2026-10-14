// Host check only; not a build input. Build as x86_64 with statics below 4 GiB.
#include "ondrawself.cpp"
#include <cstdio>
#include <cstring>
#include <string>

alignas(8) static unsigned char object[1100];
alignas(8) static int table[16];
unsigned _5VIWnd$mUI = static_cast<unsigned>(reinterpret_cast<unsigned long>(table));
static std::string trace;
static std::string off(const void* p) {
    const long d = static_cast<const unsigned char*>(p) - object;
    return d >= 0 && d < 1100 ? std::to_string(d) : "?";
}
extern "C" {
void Begin2D__4VIUI(void*) { trace += "begin;"; }
void End2D__4VIUI(void*) { trace += "end;"; }
void Draw__10VIUIWindow(void* w) { trace += "draw:" + off(w) + ";"; }
void TextBox__10VIUIWindowPCUsiiii9VIUICOLORUi(void* w, const unsigned short* t, int a, int b, int c, int d, int color, unsigned e) {
    trace += "tb:" + off(w) + "," + (t == reinterpret_cast<const unsigned short*>(object + 472) ? std::string("text") : std::to_string(reinterpret_cast<unsigned long>(t))) +
             "," + std::to_string(a) + "," + std::to_string(b) + "," + std::to_string(c) + "," + std::to_string(d) + "," + std::to_string(color) + "," + std::to_string(e) + ";";
}
const unsigned short* PromptText__12VIWndMessagePCUsi(void* s, const unsigned short* t, int f) {
    trace += "prompt:" + off(s) + "," + std::to_string(reinterpret_cast<unsigned long>(t)) + "," + std::to_string(f) + ";";
    return reinterpret_cast<const unsigned short*>(1000 + reinterpret_cast<unsigned long>(t));
}
const unsigned short* GetString__13VIStringTableii(const void* t, int id, int col) {
    trace += std::string("gs:") + (t == table ? "ui" : "?") + "," + std::to_string(id) + "," + std::to_string(col) + ";";
    return reinterpret_cast<const unsigned short*>(id);
}
}
static void set_word(int offset, int v) { std::memcpy(object + offset, &v, 4); }

int main() {
    bool ok = true;
    table[12] = 3;
    const std::string head = "begin;draw:40;tb:40,text,40,1,220,4,0,8;";
    for (int sel : {-1, 3}) {
        std::memset(object, 0, sizeof object); trace.clear();
        set_word(60, 300); set_word(1000, 6); set_word(1004, sel);
        OnDrawSelf__12VIWndMessage(object);
        ok = ok && trace == head + "end;";
    }
    std::memset(object, 0, sizeof object); trace.clear();
    set_word(60, 300); set_word(1000, 6); set_word(1004, 0);
    OnDrawSelf__12VIWndMessage(object);
    ok = ok && trace == head + "gs:ui,786,3;prompt:0,786,1;tb:40,1786,0,5,300,1,0,8;end;";
    for (int sel : {1, 2}) {
        for (int n : {0, 1}) {
            std::memset(object, 0, sizeof object); trace.clear();
            set_word(60, 300); set_word(1000, 6); set_word(1004, sel); set_word(984, n);
            OnDrawSelf__12VIWndMessage(object);
            const int a = sel == 1 ? 786 : 21, b = sel == 1 ? 753 : 22;
            const std::string want = head + "gs:ui," + std::to_string(a) + ",3;prompt:0," + std::to_string(a) + "," + std::to_string(n == 0) + ";tb:40," +
                                     std::to_string(1000 + a) + ",0,5,200,1,0,8;gs:ui," + std::to_string(b) + ",3;prompt:0," + std::to_string(b) + "," +
                                     std::to_string(n == 1) + ";tb:40," + std::to_string(1000 + b) + ",100,5,200,1,0,8;end;";
            ok = ok && trace == want;
        }
    }
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
