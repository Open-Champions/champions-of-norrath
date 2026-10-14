// Host check only; not a build input. Build as x86_64 with statics below 4 GiB.
#include "onkeydown.cpp"
#include <cstdio>
#include <string>

alignas(8) static int object[600];
alignas(8) static unsigned char ui_object[128];
alignas(8) static unsigned char entries[3 * 620];
unsigned _5VIWnd$mUI = static_cast<unsigned>(reinterpret_cast<unsigned long>(ui_object));
static std::string trace;
static int width_result;
static std::string off(const void* p) {
    const long d = static_cast<const char*>(p) - reinterpret_cast<const char*>(object);
    return d >= 0 && d < 2400 ? std::to_string(d) : "?";
}

extern "C" {
void SoundPlay__4VIUI11VIUISoundId(const void* ui, int s) { trace += std::string("snd:") + (ui == ui_object ? "ui" : "?") + "," + std::to_string(s) + ";"; }
void SetFocus__5VIWndP5VIWndb(void* s, void* t, bool f) { trace += "focus:" + off(s) + "," + std::to_string(reinterpret_cast<unsigned long>(t)) + "," + std::to_string(f) + ";"; }
void BeginConnect__12VIWndConnect(void* s) { trace += "begin:" + off(s) + ";"; }
void Close__10VIUIWindowb(void* w, bool f) { trace += "close:" + off(w) + "/" + std::to_string(f) + ";"; }
void Init__10VIUIWindowP4VIUIRC8VIVect2IT2iii(void* w, unsigned ui, const int* p, const int* s, int a, int b, int c) {
    trace += "init:" + off(w) + "," + std::string(ui == _5VIWnd$mUI ? "ui" : "?") + "," + std::to_string(p[0]) + "," + std::to_string(p[1]) + "," +
             std::to_string(s[0]) + "," + std::to_string(s[1]) + "," + std::to_string(a) + "," + std::to_string(b) + "," + std::to_string(c) + ";";
}
void Open__10VIUIWindowb(void* w, bool f) { trace += "open:" + off(w) + "/" + std::to_string(f) + ";"; }
int GetStringWidth__6VIFontP8VIRasterP12VIDictionaryPCc(void* font, void* raster, void* dict, const char* text) {
    trace += std::string("width:") + (font == ui_object + 56 ? "font" : "?") + "," + std::to_string(reinterpret_cast<unsigned long>(raster)) + "," +
             std::to_string(reinterpret_cast<unsigned long>(dict)) + "," + text + ";";
    return width_result;
}
int ezNetCtlDownInterface(int v) { trace += "down:" + std::to_string(v) + ";"; return 0; }
int sceGsSyncV(int) { trace += "s"; return 0; }
}

static void reset() {
    for (int& w : object) w = 0;
    object[5] = 0x4000; object[7] = 9;
    object[479] = 1; object[492] = 3; object[493] = static_cast<int>(reinterpret_cast<unsigned long>(entries));
    object[297] = 111; object[298] = 222;
    *reinterpret_cast<unsigned*>(ui_object + 72) = 0x7000;
    *reinterpret_cast<unsigned*>(ui_object + 80) = 0x8000;
    for (int i = 0; i < 3; ++i) std::snprintf(reinterpret_cast<char*>(entries) + 620 * i + 260, 16, "name%d", i);
    trace.clear();
}

int main() {
    bool ok = true;
    reset(); object[489] = 1; OnKeyDown__12VIWndConnecti(object, 134); ok = ok && trace.empty() && object[480] == 0;
    reset(); OnKeyDown__12VIWndConnecti(object, 136); ok = ok && trace == "snd:ui,11;" && object[479] == 0;
    reset(); object[479] = 0; OnKeyDown__12VIWndConnecti(object, 136); ok = ok && object[479] == 2;
    reset(); OnKeyDown__12VIWndConnecti(object, 137); ok = ok && trace == "snd:ui,11;" && object[479] == 2;
    reset(); object[479] = 2; OnKeyDown__12VIWndConnecti(object, 137); ok = ok && object[479] == 0;
    reset(); object[480] = 1; OnKeyDown__12VIWndConnecti(object, 137); ok = ok && trace.empty() && object[479] == 1;
    reset(); object[480] = 1; OnKeyDown__12VIWndConnecti(object, 134); ok = ok && trace.empty();
    reset(); OnKeyDown__12VIWndConnecti(object, 135); ok = ok && trace == "snd:ui,8;focus:0,16384,1;" && object[7] == 0;
    reset(); object[480] = 1; object[491] = 1; OnKeyDown__12VIWndConnecti(object, 135); ok = ok && trace.empty() && object[480] == 1;
    reset(); object[480] = 1; OnKeyDown__12VIWndConnecti(object, 135);
    ok = ok && trace == "down:0;snd:ui,8;" + std::string(60, 's') && object[480] == 0;
    for (int width : {10, 300, 301, 500}) {
        reset(); width_result = width;
        OnKeyDown__12VIWndConnecti(object, 134);
        const int want_a = (width < 300 ? 300 : width) + 120;
        ok = ok && trace == "snd:ui,15;begin:0;close:1544/1;width:font,28672,32768,name1;init:1544,ui,0,0,111,222," + std::to_string(want_a) + ",4,0;open:1544/0;" &&
             object[480] == 1;
    }
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
