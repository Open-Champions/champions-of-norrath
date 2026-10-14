// Host contract for fontPrintWrapCentered__FiiPciiii. Not an EE plan input.
#include "font_print_wrap_centered.cpp"
#include <string>
static std::string calls;
void* curFont = 0;
signed char language = 0;
extern "C" {
int _fontStringSize__FP4FontPci(void*, const char*, int count) { return count * 10; }
void _fontPrintu__FiiPUsi(int x, int y, const unsigned short* text, int count) {
    calls += "(" + std::to_string(x) + "," + std::to_string(y) + ",";
    for (int i = 0; i < count; ++i) calls += static_cast<char>(text[i]);
    calls += ")";
}
unsigned short* ustrcpy__FPUsPc(unsigned short* dest, const char* src) {
    unsigned short* d = dest;
    while ((*d++ = static_cast<unsigned char>(*src++)) != 0) {}
    return dest;
}
}
int main() {
    calls.clear();
    int r = fontPrintWrapCentered__FiiPciiii(100, 10, "aa bb cc", 50, 0, 1000, 8);
    if (calls != "(85,10,aa )(85,18,bb )(90,26,cc)" || r != 24) return 1;
    calls.clear();
    r = fontPrintWrapCentered__FiiPciiii(100, 10, "aa bb cc", 50, 12, 1000, 8);
    if (calls != "(85,18,bb )(90,26,cc)" || r != 24) return 2;
    calls.clear();
    r = fontPrintWrapCentered__FiiPciiii(100, 10, "aa bb cc", 50, 0, 20, 8);
    if (calls != "(85,10,aa )(85,18,bb )" || r != 24) return 3;
    calls.clear();
    r = fontPrintWrapCentered__FiiPciiii(0, 0, "ab\ncd", 1000, -1, 1000, 5);
    if (calls != "(-15,0,ab\n)(-10,5,cd)" || r != 10) return 4;
    calls.clear();
    if (fontPrintWrapCentered__FiiPciiii(0, 7, "", 100, 0, 100, 5) != 0 || !calls.empty()) return 5;
    language = 5;
    calls.clear();
    r = fontPrintWrapCentered__FiiPciiii(50, 1, "xyz", 15, 0, 100, 2);
    if (calls != "(45,1,x)(45,3,y)(45,5,z)" || r != 6) return 6;
    return 0;
}
