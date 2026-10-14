// Host check only; not a build input.
#include "str16to8.cpp"
#include <cstdio>
#include <cstring>
extern "C" {
const unsigned short _8VIString$ConversionTable1252[] __asm__("_8VIString$ConversionTable1252") = {0x20ac, 0x201a, 0x0192};
const int ConversionTableSize_180 __asm__("ConversionTableSize.180") = 3;
}
int main() {
    char buf[16];
    bool ok = true;
    const unsigned short s1[] = {0x41, 0x20ac, 0x0192, 0x1234, 0x62, 0};
    std::memset(buf, 0x5a, sizeof buf);
    if (str16to8__8VIStringPcUiPCUs(buf, 16, s1) != buf) ok = false;
    if (std::memcmp(buf, "A\x80\x82?b", 6) != 0 || buf[6] != 0x5a) ok = false;
    // limit truncation: limit 3 writes two bytes and a terminator
    std::memset(buf, 0x5a, sizeof buf);
    str16to8__8VIStringPcUiPCUs(buf, 3, s1);
    if (std::memcmp(buf, "A\x80", 3) != 0 || buf[3] != 0x5a) ok = false;
    // limit 1 and 0: only a terminator
    for (unsigned lim = 0; lim < 2; ++lim) {
        std::memset(buf, 0x5a, sizeof buf);
        str16to8__8VIStringPcUiPCUs(buf, lim, s1);
        if (buf[0] != 0 || buf[1] != 0x5a) ok = false;
    }
    // empty source
    const unsigned short e[] = {0};
    std::memset(buf, 0x5a, sizeof buf);
    str16to8__8VIStringPcUiPCUs(buf, 8, e);
    if (buf[0] != 0 || buf[1] != 0x5a) ok = false;
    // low bytes pass through unchanged, including 128..255
    const unsigned short hi[] = {0xe9, 0xff, 0};
    std::memset(buf, 0x5a, sizeof buf);
    str16to8__8VIStringPcUiPCUs(buf, 8, hi);
    if (std::memcmp(buf, "\xe9\xff", 3) != 0) ok = false;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
