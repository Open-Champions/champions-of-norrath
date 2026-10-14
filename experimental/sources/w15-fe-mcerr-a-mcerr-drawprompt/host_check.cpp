// Host check only; not a build input. Stubs record the text each TextBox call receives; the label strings are
// short ASCII in 16-bit units.
#include "drawprompt.cpp"
#include <cstdio>
#include <cstring>
#include <string>

static std::string trace;
static std::string narrow(const unsigned short* t) {
    std::string r;
    while (*t) r += static_cast<char>(*t++);
    return r;
}
extern "C" {
int scePrintf(const char* format, ...) { trace += std::string("P[") + format + "]"; return 0; }
void TextBox__10VIUIWindowPCUsiiii9VIUICOLORUi(void* window, const unsigned short* text, int a, int b, int c, int d, int color, unsigned e) {
    trace += std::string("box(") + (window != nullptr ? "w" : "?") + "," + narrow(text) + "," + std::to_string(a) + "," + std::to_string(b) + "," + std::to_string(c) + "," +
             std::to_string(d) + "," + std::to_string(color) + "," + std::to_string(e) + ");";
}
void str8to16__8VIStringPUsUiPCc(unsigned short* d, unsigned, const char* s) {
    unsigned i = 0;
    for (; s[i]; ++i) d[i] = static_cast<unsigned char>(s[i]);
    d[i] = 0;
}
void strcat16__8VIStringPUsUiPCUs(unsigned short* d, unsigned, const unsigned short* s) {
    unsigned k = 0;
    while (d[k]) ++k;
    for (unsigned i = 0; s[i]; ++i) d[k++] = s[i];
    d[k] = 0;
}
void strcat16__8VIStringPUsUiPCc(unsigned short* d, unsigned, const char* s) {
    unsigned k = 0;
    while (d[k]) ++k;
    for (unsigned i = 0; s[i]; ++i) d[k++] = static_cast<unsigned char>(s[i]);
    d[k] = 0;
}
unsigned short* strcpy16__8VIStringPUsUiPCUs(unsigned short* d, unsigned, const unsigned short* s) {
    unsigned i = 0;
    for (; s[i]; ++i) d[i] = s[i];
    d[i] = 0;
    return d;
}
}

static int check(const char* label, const std::string& expected) {
    if (trace == expected) return 0;
    std::fprintf(stdout, "MISMATCH %s\n  got:  %s\n  want: %s\n", label, trace.c_str(), expected.c_str());
    return 1;
}

int main() {
    int bad = 0;
    int object[160];
    const unsigned short one[3] = {'O', 'K', 0}, two[3] = {'N', 'O', 0};
    const std::string head = "P[VIWndMcErr::DrawPrompt()\n]";
    const int width = 481;  // thirds 160, half 240
    for (int selected : {0, 1, 2}) {
        std::memset(object, 0, sizeof object);
        object[102] = selected;
        object[15] = width;
        trace.clear();
        DrawPrompt__10VIWndMcErriPCUsT2(object, 4, one, nullptr);
        bad += check("single", head + "box(w,|qOK|w,40,4,400,1,1,8);");
        trace.clear();
        DrawPrompt__10VIWndMcErriPCUsT2(object, 5, one, two);
        const std::string a = selected == 0 ? "|qOK|w" : "OK", b = selected == 1 ? "|qNO|w" : "NO";
        bad += check("double", head + "box(w," + a + ",-40,5,400,1," + std::to_string(selected == 0) + ",8);box(w," + b + ",120,5,400,1," +
                                   std::to_string(selected == 1) + ",8);");
    }
    object[15] = -10;  // negative width: signed division and arithmetic shift
    object[102] = 0;
    trace.clear();
    DrawPrompt__10VIWndMcErriPCUsT2(object, 0, one, nullptr);
    bad += check("negative single", head + "box(w,|qOK|w,-205,0,400,1,1,8);");
    trace.clear();
    DrawPrompt__10VIWndMcErriPCUsT2(object, 0, one, two);
    bad += check("negative double", head + "box(w,|qOK|w,-203,0,400,1,1,8);box(w,NO,-206,0,400,1,0,8);");
    std::puts(bad ? "FAIL" : "ok");
    return bad != 0;
}
