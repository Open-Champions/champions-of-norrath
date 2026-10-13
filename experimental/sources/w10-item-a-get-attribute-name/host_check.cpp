// Host check for getAttributeName__4itemiPc. Not an EE plan input.
#include "get_attribute_name.cpp"
#include <string>
signed char language;
int _4item$undEnable;
static std::basic_string<unsigned short> source;
static std::string ev;
static std::basic_string<unsigned short> U(const char* s) { std::basic_string<unsigned short> r; while (*s) r += static_cast<unsigned char>(*s++); return r; }
static std::string A(const unsigned short* s) { std::string r; while (*s) r += static_cast<char>(*s++); return r; }
extern "C" {
unsigned short* textGetString__FPc(char*) { return const_cast<unsigned short*>(source.c_str()); }
void usprintf__FPUsT0ffffffff(unsigned short* d, unsigned short* s, float, float, float, float, float, float, float, float) { ev += "P"; do *d++ = *s; while (*s++); }
void ustrcpy__FPUsT0(unsigned short* d, unsigned short* s) { ev += "C"; do *d++ = *s; while (*s++); }
void ustrcpy__FPUsPc(unsigned short* d, char* s) { ev += "c"; do *d++ = static_cast<unsigned char>(*s); while (*s++); }
void ustrcat__FPUsT0(unsigned short* d, unsigned short* s) { ev += "T"; while (*d) d++; do *d++ = *s; while (*s++); }
int ustrlen__FPUs(unsigned short* s) { int n = 0; while (s[n]) n++; return n; }
}
static std::string run(int idx, const char* text, int lang, int und) {
    source = U(text); language = lang; _4item$undEnable = und; ev.clear();
    return A(getAttributeName__4itemiPc(idx, const_cast<char*>("k")));
}
int main() {
    if (run(-1, "a|b", 0, 0) != "a|b" || ev != "P" "P") return 1;
    if (run(0, "plain", 0, 0) != "plain" || ev != "PC") return 2;
    if (run(0, "ab|cd|ef", 0, 0) != "ab") return 3;
    if (run(1, "ab|cd|ef", 0, 0) != "cd") return 4;
    if (run(2, "ab|cd|ef", 0, 0) != "ef") return 5;
    if (run(2, "ab|cd|", 0, 0) != "") return 6;
    if (run(1, "abcd|efghij", 3, 0) != "efg") return 7;      // language 3 drops three characters
    if (run(0, "abcdef", 3, 0) != "abcdef") return 8;        // ...only for bar text
    if (run(0, "der x", 0, 1) != "der x" || _4item$undEnable != 2) return 9;
    if (run(0, "der x", 0, 2) != "und der x" || ev != "PCcT") return 10;
    if (run(0, "den", 0, 2) != "den") return 11;
    return 0;
}
