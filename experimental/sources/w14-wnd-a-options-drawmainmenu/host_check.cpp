// Host check only; not a build input. Build: c++ -std=c++17 -arch x86_64 -Wl,-pagezero_size,0x1000 host_check.cpp
// (statics must sit below 4 GiB because mUI is a 32-bit word).
#include "drawmainmenu.cpp"
#include <cstdio>
#include <cstring>
#include <string>

unsigned _5VIWnd$mUI;
alignas(8) static unsigned char ui[128];
alignas(8) static int options[400];
static std::string trace;
static unsigned short strings[1000][4];
static std::string narrow(const unsigned short* s) {
    std::string r;
    for (; *s; ++s) r += static_cast<char>(*s);
    return r;
}
extern "C" {
void Draw__10VIUIWindow(void*) { trace += "D;"; }
const unsigned short* GetString__13VIStringTableii(const void*, int id, int column) {
    trace += "G" + std::to_string(id) + "c" + std::to_string(column) + ";";
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
void strcat16__8VIStringPUsUiPCc(unsigned short* d, unsigned, const char* s) {
    int n = 0;
    while (d[n]) ++n;
    for (int i = 0;; ++i) { d[n + i] = static_cast<unsigned char>(s[i]); if (!s[i]) break; }
}
int TextBox__10VIUIWindowPCUsiiii9VIUICOLORUi(void*, const unsigned short* t, int a, int b, int c, int d, int color, unsigned flags) {
    trace += "T<" + narrow(t) + ">" + std::to_string(a) + "," + std::to_string(b) + "," + std::to_string(c) + "," + std::to_string(d) + "c" + std::to_string(color) + "f" + std::to_string(flags) + ";";
    return 0;
}
int Text__10VIUIWindowPCUsii9VIUICOLORUi(void*, const unsigned short* t, int a, int b, int color, unsigned flags) {
    trace += "X<" + narrow(t) + ">" + std::to_string(a) + "," + std::to_string(b) + "c" + std::to_string(color) + "f" + std::to_string(flags) + ";";
    return 0;
}
}

int main() {
    bool ok = true;
    if (reinterpret_cast<unsigned long>(ui) > 0xffffffffUL) { std::puts("FAIL (statics above 4 GiB)"); return 1; }
    _5VIWnd$mUI = static_cast<unsigned>(reinterpret_cast<unsigned long>(ui));
    *reinterpret_cast<int*>(ui + 48) = 7;
    options[10 + 5] = 300;                        // window (this + 40) width at +20 -> word 15
    options[288] = 123;
    options[295] = 1;
    DrawMainMenu__12VIWndOptions(options);
    ok = ok && options[288] == -1 &&
         trace == "D;G773c7;T<S3>40,1,220,1c0f8;G778c7;T<|qS8|w>40,2,220,1c1f8;G800c7;T<S0>40,3,220,1c0f8;G482c7;T<S2>40,4,220,1c0f8;"
                  "G212c7;G753c7;X<|x S2   |t S3>40,6c0f0;";
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
