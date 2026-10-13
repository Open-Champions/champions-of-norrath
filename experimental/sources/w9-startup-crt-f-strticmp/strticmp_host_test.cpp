// Host check only; not an EE build input. The ctype view is supplied here with
// the observed upper-case marking for letters only; the 384-byte view is checked
// by libc-format-ctype.
#include <cstring>
extern "C" unsigned char norrath_factory_ctype_minimum_view[384] = {};
extern "C" int strticmp__FPCcT0(const char*, const char*);
int main() {
    for (int c = 'A'; c <= 'Z'; ++c) norrath_factory_ctype_minimum_view[c + 128] = 1;
    if (strticmp__FPCcT0("Hello", "hELLO") != 0) return 1;
    if (strticmp__FPCcT0("Hello", "Help") != 1) return 2;
    if (strticmp__FPCcT0("", "") != 0) return 3;
    if (strticmp__FPCcT0("abc", "abcd") != 1) return 4;   // first ends before second
    if (strticmp__FPCcT0("abcd", "abc") != 0) return 5;   // second ends first
    if (strticmp__FPCcT0("a_b", "A_B") != 0) return 6;
    if (strticmp__FPCcT0("a[b", "A{b") != 1) return 7;   // only letters fold
    if (strticmp__FPCcT0("\xe1", "\xe1") != 0) return 8;
    return 0;
}
