// Host contract for addpmlist__FPc. Not an EE plan input. strlen/strcpy are the host's own.
#include <cstring>
#define strlen w_strlen
#define strcpy w_strcpy
#include "add_pm_list.cpp"
#undef strlen
#undef strcpy
extern "C" {
char pmlistchars[0x1000];
int pmlistcharsused;
unsigned w_strlen(const char* s) { return (unsigned)std::strlen(s); }
char* w_strcpy(char* d, const char* s) { return std::strcpy(d, s); }
}
int main() {
    char a[] = "abc", b[] = "de", c[] = "bc", d[] = "", e[] = "abcd";
    // empty pool: first string is appended at 0
    if (addpmlist__FPc(a) != pmlistchars || pmlistcharsused != 4) return 1;
    if (std::strcmp(pmlistchars, "abc") != 0) return 2;
    if (addpmlist__FPc(b) != pmlistchars + 4 || pmlistcharsused != 7) return 3;
    // the same string is found again and nothing grows
    if (addpmlist__FPc(a) != pmlistchars || pmlistcharsused != 7) return 4;
    if (addpmlist__FPc(b) != pmlistchars + 4 || pmlistcharsused != 7) return 5;
    // a tail of an earlier entry matches (every pool byte is tried)
    if (addpmlist__FPc(c) != pmlistchars + 1 || pmlistcharsused != 7) return 6;
    // the empty string matches the first zero byte at offset 3 with length 0
    if (addpmlist__FPc(d) != pmlistchars + 3 || pmlistcharsused != 7) return 7;
    // a longer string with a known prefix is appended as a new entry
    if (addpmlist__FPc(e) != pmlistchars + 7 || pmlistcharsused != 12) return 8;
    if (std::strcmp(pmlistchars + 7, "abcd") != 0) return 9;
    return 0;
}
