// Host check only; not a build input. Build as x86_64 with statics below 4 GiB.
#include "ondrawself.cpp"
#include <cstdio>
#include <cstring>
#include <string>

alignas(8) static unsigned char object[16832];
alignas(8) static int table[16];
unsigned _5VIWnd$mUI = static_cast<unsigned>(reinterpret_cast<unsigned long>(table));
static std::string trace;
static int line_count;
static std::string off(const void* p) {
    const long d = static_cast<const unsigned char*>(p) - object;
    return d >= 0 && d < 16832 ? std::to_string(d) : "?";
}
extern "C" {
void Begin2D__4VIUI(void*) { trace += "begin;"; }
void End2D__4VIUI(void*) { trace += "end;"; }
void Draw__10VIUIWindow(void* w) { trace += "draw:" + off(w) + ";"; }
int CountLines__10VIUIWindowPCUsi(void* w, const unsigned short* t, int n) {
    trace += "count:" + off(w) + "," + (t == reinterpret_cast<const unsigned short*>(object + 412) ? "text" : "?") + "," + std::to_string(n) + ";";
    return line_count;
}
void DrawFrameVScroll__10VIUIWindowiiiif(void* w, int a, int b, int c, int d, float f) {
    trace += "vscroll:" + off(w) + "," + std::to_string(a) + "," + std::to_string(b) + "," + std::to_string(c) + "," + std::to_string(d) + "," + std::to_string(f) + ";";
}
void TextBox__10VIUIWindowPCUsiiii9VIUICOLORUi(void* w, const unsigned short* t, int a, int b, int c, int d, int color, unsigned e) {
    trace += "tb:" + off(w) + "," + (t == reinterpret_cast<const unsigned short*>(object + 412) ? "text" : "?") + "," + std::to_string(a) + "," + std::to_string(b) +
             "," + std::to_string(c) + "," + std::to_string(d) + "," + std::to_string(color) + "," + std::to_string(e) + ";";
}
void Text__10VIUIWindowPCUsii9VIUICOLORUi(void* w, const unsigned short* t, int a, int b, int color, unsigned c) {
    std::string s;
    for (int i = 0; t[i]; ++i) s += static_cast<char>(t[i]);
    trace += "text:" + off(w) + "," + s + "," + std::to_string(a) + "," + std::to_string(b) + "," + std::to_string(color) + "," + std::to_string(c) + ";";
}
void str8to16__8VIStringPUsUiPCc(unsigned short* d, unsigned n, const char* s) {
    trace += "8to16:" + std::to_string(n) + "," + s + ";";
    int i = 0;
    for (; s[i]; ++i) d[i] = static_cast<unsigned char>(s[i]);
    d[i] = 0;
}
void strcat16__8VIStringPUsUiPCUs(unsigned short* d, unsigned n, const unsigned short* s) {
    trace += "cat16:" + std::to_string(n) + "," + std::to_string(reinterpret_cast<unsigned long>(s)) + ";";
    int e = 0;
    while (d[e]) ++e;
    d[e] = 'Z'; d[e + 1] = 0;
}
const unsigned short* GetString__13VIStringTableii(const void* t, int id, int col) {
    trace += std::string("gs:") + (t == table ? "ui" : "?") + "," + std::to_string(id) + "," + std::to_string(col) + ";";
    return reinterpret_cast<const unsigned short*>(id);
}
}
static void set_word(int offset, int v) { std::memcpy(object + offset, &v, 4); }
static int word(int offset) { int v; std::memcpy(&v, object + offset, 4); return v; }

int main() {
    bool ok = true;
    table[12] = 3;
    const std::string tail = "8to16:512,|t ;gs:ui,750,3;cat16:512,750;text:40,|t Z,40,9,0,0;end;";
    for (int lines : {0, 7, 8, 9, 17}) {
        std::memset(object, 0, sizeof object); trace.clear();
        set_word(16796, lines); set_word(16800, 5); set_word(60, 400);
        OnDrawSelf__16VIWndReadMessage(object);
        char buf[32];
        std::snprintf(buf, sizeof buf, "%f", lines >= 8 ? 5.0f / static_cast<float>(lines - 7) : -1.0f);
        const std::string want = std::string("begin;draw:40;vscroll:40,40,1,320,7,") + buf + ";tb:40,text,40,1,320,7,0,83886080;" + tail;
        ok = ok && trace.substr(0, want.size() - tail.size()) == want.substr(0, want.size() - tail.size());
    }
    std::memset(object, 0, sizeof object); trace.clear(); line_count = 12;
    set_word(16804, 1); set_word(16796, 99); set_word(16800, 77); set_word(60, 400);
    OnDrawSelf__16VIWndReadMessage(object);
    ok = ok && trace.find("begin;draw:40;count:40,text,360;vscroll:40,40,1,320,7,0.000000;tb:40,text,40,1,320,7,0,0;") == 0 && word(16796) == 12 && word(16800) == 0 && word(16804) == 0;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
