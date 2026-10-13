// Host check only; not a build input. Build: c++ -std=c++17 -arch x86_64 -Wl,-pagezero_size,0x1000 host_check.cpp
// (statics must sit below 4 GiB because mUI and the table are 32-bit words).
#include "drawsoundoptions.cpp"
#include <cstdio>
#include <cstring>
#include <string>

unsigned _5VIWnd$mUI;
unsigned sBattleMusicText[10];
unsigned sSoundOutputText[2];
alignas(8) static unsigned char ui[128];
alignas(8) static int options[400];
alignas(8) static unsigned char game[200];
static std::string trace;
static unsigned short strings[1000][4];
static unsigned short music_text[4] = {'M', 'u', 0, 0}, out_text[4] = {'S', 't', 0, 0};
static std::string narrow(const unsigned short* s) {
    std::string r;
    for (; *s; ++s) r += static_cast<char>(*s);
    return r;
}
extern "C" {
unsigned char* VIGetGameOptions__Fv() { return game; }
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
void DrawHScroll__10VIUIWindowiiifUi(void*, int a, int b, int c, float v, unsigned flags) {
    trace += "H" + std::to_string(a) + "," + std::to_string(b) + "," + std::to_string(c) + "v" + std::to_string(static_cast<int>(v * 100 + 0.5f)) + "f" + std::to_string(flags) + ";";
}
void TickR__10VIUIWindowiib(void*, int a, int b, bool c) { trace += "R" + std::to_string(a) + "," + std::to_string(b) + "," + std::to_string(c) + ";"; }
void DrawFrame__10VIUIWindowiiiiUi(void*, int a, int b, int c, int d, unsigned flags) {
    trace += "F" + std::to_string(a) + "," + std::to_string(b) + "," + std::to_string(c) + "," + std::to_string(d) + "f" + std::to_string(flags) + ";";
}
}

int main() {
    bool ok = true;
    if (reinterpret_cast<unsigned long>(ui) > 0xffffffffUL || reinterpret_cast<unsigned long>(music_text) > 0xffffffffUL) {
        std::puts("FAIL (statics above 4 GiB)");
        return 1;
    }
    _5VIWnd$mUI = static_cast<unsigned>(reinterpret_cast<unsigned long>(ui));
    for (unsigned& w : sBattleMusicText) w = static_cast<unsigned>(reinterpret_cast<unsigned long>(music_text));
    for (unsigned& w : sSoundOutputText) w = static_cast<unsigned>(reinterpret_cast<unsigned long>(out_text));
    options[102 + 5] = 400;                       // window (this + 408) width at +20 -> word 107
    options[287] = 2;
    options[288] = 55;
    game[4] = 3; game[5] = 1;
    reinterpret_cast<float*>(game)[2] = 0.5f;
    reinterpret_cast<float*>(game)[3] = 0.25f;
    DrawSoundOptions__12VIWndOptions(options);
    ok = ok && options[288] == 4 &&
         trace == "D;T<S1>40,1,320,1c0f0;T<S2>40,2,320,1c0f0;T<S2>40,3,320,1c1f0;T<S1>40,4,320,1c0f0;R36,3,1;"
                  "F224,1,132,1f64;T<Mu>224,1,132,1c0f8;H220,2,140v50f0;H220,3,140v25f32;"
                  "F224,4,132,1f64;T<St>224,4,132,1c0f8;X<|t S0>40,6c0f0;";
    options[287] = 0;
    trace.clear();
    DrawSoundOptions__12VIWndOptions(options);
    ok = ok && options[288] == 2 && trace.find("F224,1,132,1f96;") != std::string::npos;
    options[287] = 1;
    DrawSoundOptions__12VIWndOptions(options);
    ok = ok && options[288] == 3;
    options[287] = 3;
    DrawSoundOptions__12VIWndOptions(options);
    ok = ok && options[288] == 5;
    options[287] = 9;
    DrawSoundOptions__12VIWndOptions(options);
    ok = ok && options[288] == 5;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
