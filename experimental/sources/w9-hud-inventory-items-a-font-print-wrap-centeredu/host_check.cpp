// Host contract for fontPrintWrapCenteredu__FiiPUsiiii. Not an EE plan input.
#include "font_print_wrap_centeredu.cpp"
#include <string>
#include <vector>
static std::string calls;
void* curFont = 0;
signed char language = 0;
extern "C" {
// Width of a segment: ten pixels per character.
int _fontStringSizeu__FP4FontPUsi(void*, const unsigned short*, int count) { return count * 10; }
void _fontPrintu__FiiPUsi(int x, int y, const unsigned short* text, int count) {
    calls += "(" + std::to_string(x) + "," + std::to_string(y) + ",";
    for (int i = 0; i < count; ++i) calls += static_cast<char>(text[i]);
    calls += ")";
}
}
static std::vector<unsigned short> w(const char* s) {
    std::vector<unsigned short> v;
    while (*s) v.push_back(static_cast<unsigned char>(*s++));
    v.push_back(0);
    return v;
}
int main() {
    auto t = w("aa bb cc");
    calls.clear();
    // width 50: "aa " (30) + "bb " (30) breaks; "bb cc" is the next line.
    int r = fontPrintWrapCenteredu__FiiPUsiiii(100, 10, t.data(), 50, 0, 1000, 8);
    if (calls != "(85,10,aa )(85,18,bb )(90,26,cc)" || r != 24) return 1;
    // lines outside the bounds are skipped but still advance y
    calls.clear();
    r = fontPrintWrapCenteredu__FiiPUsiiii(100, 10, t.data(), 50, 12, 1000, 8);
    if (calls != "(85,18,bb )(90,26,cc)" || r != 24) return 2;
    calls.clear();
    r = fontPrintWrapCenteredu__FiiPUsiiii(100, 10, t.data(), 50, 0, 20, 8);
    if (calls != "(85,10,aa )(85,18,bb )" || r != 24) return 3;
    // a newline forces a break
    auto n = w("ab\ncd");
    calls.clear();
    r = fontPrintWrapCenteredu__FiiPUsiiii(0, 0, n.data(), 1000, -1, 1000, 5);
    if (calls != "(-15,0,ab\n)(-10,5,cd)" || r != 10) return 4;
    // empty text
    auto e = w("");
    calls.clear();
    if (fontPrintWrapCenteredu__FiiPUsiiii(0, 7, e.data(), 100, 0, 100, 5) != 0 || !calls.empty()) return 5;
    // language 5 splits into single characters
    language = 5;
    auto s = w("xyz");
    calls.clear();
    r = fontPrintWrapCenteredu__FiiPUsiiii(50, 1, s.data(), 15, 0, 100, 2);
    if (calls != "(45,1,x)(45,3,y)(45,5,z)" || r != 6) return 6;
    return 0;
}
