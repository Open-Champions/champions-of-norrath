// Host check only; not a build input.
#include "prompttext.cpp"
#include <cstdio>
#include <string>

static std::string out;
extern "C" void str8to16__8VIStringPUsUiPCc(unsigned short* d, unsigned n, const char* s) {
    out += "set(" + std::string(s) + "," + std::to_string(n) + ")";
    int i = 0; for (; s[i]; ++i) d[i] = s[i]; d[i] = 0;
}
extern "C" void strcat16__8VIStringPUsUiPCUs(unsigned short* d, unsigned n, const unsigned short* s) {
    out += "cat16(" + std::to_string(n) + ")";
    int e = 0; while (d[e]) ++e; int i = 0; for (; s[i]; ++i) d[e + i] = s[i]; d[e + i] = 0;
}
extern "C" void strcat16__8VIStringPUsUiPCc(unsigned short* d, unsigned n, const char* s) {
    out += "cat8(" + std::string(s) + "," + std::to_string(n) + ")";
    int e = 0; while (d[e]) ++e; int i = 0; for (; s[i]; ++i) d[e + i] = s[i]; d[e + i] = 0;
}

int main() {
    static unsigned char obj[2048];
    static const unsigned short txt[3] = {'h', 'i', 0};
    bool ok = true;
    unsigned short* r = PromptText__12VIWndMessagePCUsi(obj, txt, 1);
    ok = ok && r == reinterpret_cast<unsigned short*>(obj + 1008) && out == "set(|q,128)cat16(128)cat8(|w,128)";
    ok = ok && r[0] == '|' && r[1] == 'q' && r[2] == 'h' && r[4] == '|' && r[5] == 'w' && r[6] == 0;
    out.clear(); obj[1008] = 7;
    r = PromptText__12VIWndMessagePCUsi(obj, txt, 0);
    ok = ok && out == "cat16(128)" && r[0] == 'h' && r[1] == 'i' && r[2] == 0;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
