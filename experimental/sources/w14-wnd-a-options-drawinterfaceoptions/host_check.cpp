// Host check only; not a build input. Build: c++ -std=c++17 -arch x86_64 -Wl,-pagezero_size,0x1000 host_check.cpp
// (statics must sit below 4 GiB because mUI and the table are 32-bit words).
#include "drawinterfaceoptions.cpp"
#include <cstdio>
#include <cstring>
#include <string>

unsigned _5VIWnd$mUI;
unsigned sOnOffText[2];
unsigned sCameraTypeText[2];
unsigned sCameraAxisText[2];
unsigned sCompactKeyboardText[2];
static unsigned feature_flags;
alignas(8) static unsigned char ui[128];
alignas(8) static int options[400];
alignas(8) static unsigned char game[200];
static std::string trace;
static unsigned short strings[2000][4];
static unsigned short off_text[4] = {'O', 'f', 'f', 0}, on_text[4] = {'O', 'n', 0, 0}, kb_text[4] = {'K', 'b', 0, 0};
static std::string narrow(const unsigned short* s) {
    std::string r;
    for (; *s; ++s) r += static_cast<char>(*s);
    return r;
}
extern "C" {
unsigned char* VIGetGameOptions__Fv() { return game; }
unsigned VIGetFeatureFlags__Fv() { return feature_flags; }
static unsigned short auto_text[16] = {'A', 'u', 't', 'o', 0};
const unsigned short* T__10VIUIWindowPCc(void*, const char*) { return auto_text; }
void Draw__10VIUIWindow(void*) { trace += "D;"; }
const unsigned short* GetString__13VIStringTableii(const void*, int id, int) {
    strings[id][0] = 'S'; strings[id][1] = static_cast<unsigned short>('0' + id % 10); strings[id][2] = 0;
    return strings[id];
}
void str8to16__8VIStringPUsUiPCc(unsigned short* d, unsigned, const char* s) {
    int i = 0;
    for (; s[i]; ++i) d[i] = static_cast<unsigned char>(s[i]);
    d[i] = 0;
}
void strcat16__8VIStringPUsUiPCUs(unsigned short* d, unsigned, const unsigned short* s) {
    int n = 0;
    while (d[n]) ++n;
    for (int i = 0;; ++i) { d[n + i] = s[i]; if (!s[i]) break; }
}
int TextBox__10VIUIWindowPCUsiiii9VIUICOLORUi(void*, const unsigned short* t, int a, int b, int c, int d, int color, unsigned flags) {
    trace += "T<" + narrow(t) + ">" + std::to_string(a) + "," + std::to_string(b) + "," + std::to_string(c) + "," + std::to_string(d) + "c" + std::to_string(color) + "f" + std::to_string(flags) + ";";
    return 0;
}
int Text__10VIUIWindowPCUsii9VIUICOLORUi(void*, const unsigned short* t, int a, int b, int color, unsigned flags) {
    trace += "X<" + narrow(t) + ">" + std::to_string(a) + "," + std::to_string(b) + "c" + std::to_string(color) + "f" + std::to_string(flags) + ";";
    return 0;
}
void TickR__10VIUIWindowiib(void*, int a, int b, bool c) { trace += "R" + std::to_string(a) + "," + std::to_string(b) + "," + std::to_string(c) + ";"; }
void DrawFrame__10VIUIWindowiiiiUi(void*, int a, int b, int c, int d, unsigned flags) {
    trace += "F" + std::to_string(a) + "," + std::to_string(b) + "," + std::to_string(c) + "," + std::to_string(d) + "f" + std::to_string(flags) + ";";
}
}

int main() {
    bool ok = true;
    if (reinterpret_cast<unsigned long>(ui) > 0xffffffffUL || reinterpret_cast<unsigned long>(off_text) > 0xffffffffUL) {
        std::puts("FAIL (statics above 4 GiB)");
        return 1;
    }
    _5VIWnd$mUI = static_cast<unsigned>(reinterpret_cast<unsigned long>(ui));
    for (unsigned& w : sOnOffText) w = static_cast<unsigned>(reinterpret_cast<unsigned long>(off_text));
    sOnOffText[1] = static_cast<unsigned>(reinterpret_cast<unsigned long>(on_text));
    for (unsigned& w : sCameraTypeText) w = static_cast<unsigned>(reinterpret_cast<unsigned long>(kb_text));
    for (unsigned& w : sCameraAxisText) w = static_cast<unsigned>(reinterpret_cast<unsigned long>(kb_text));
    for (unsigned& w : sCompactKeyboardText) w = static_cast<unsigned>(reinterpret_cast<unsigned long>(kb_text));
    options[102 + 5] = 400;                       // window (this + 408) width at +20 -> word 107
    options[287] = 4;
    options[288] = 55;
    reinterpret_cast<unsigned char*>(options)[1176] = 1;
    reinterpret_cast<unsigned char*>(options)[1177] = 0;
    feature_flags = 0;
    DrawInterfaceOptions__12VIWndOptions(options);
    ok = ok && options[288] == 11 &&
         trace == "D;T<Auto>40,1,320,1c0f0;T<S7>40,2,320,1c0f0;T<S2>40,3,320,1c0f0;T<S4>40,4,320,1c0f0;R36,5,1;"
                  "F224,1,132,1f64;T<On>224,1,132,1c0f8;F224,2,132,1f64;T<Kb>224,2,132,1c0f8;F224,3,132,1f64;T<Kb>224,3,132,1c0f8;"
                  "F224,4,132,1f64;T<Kb>224,4,132,1c0f8;X<|t S0>40,6c0f0;";
    feature_flags = 0x200;
    trace.clear();
    DrawInterfaceOptions__12VIWndOptions(options);
    ok = ok && trace.find("T<S4>40,5,320,1c1f0;") != std::string::npos && trace.find("F224,5,132,1f96;T<Off>224,5,132,1c0f8;") != std::string::npos &&
         trace.find("X<|t S0>40,7c0f0;") != std::string::npos;
    const int expect[] = {10, 0, 1, 6, 11};
    for (int sel = 0; sel < 5; ++sel) {
        options[287] = sel; options[288] = 55;
        DrawInterfaceOptions__12VIWndOptions(options);
        ok = ok && options[288] == expect[sel];
    }
    options[287] = 5; options[288] = 55;
    DrawInterfaceOptions__12VIWndOptions(options);
    ok = ok && options[288] == 55;
    options[287] = -1;
    DrawInterfaceOptions__12VIWndOptions(options);
    ok = ok && options[288] == 55;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
