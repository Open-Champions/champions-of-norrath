#include "ascii_tables.cpp"

#include <cstdio>

int main() {
    static_assert(sizeof ascii_special == 132 && sizeof ascii_table == 12, "image sizes");
    // The three rows start the digit, upper and lower case runs at their full-width codes.
    if (ascii_table[0].base != 0x824f || ascii_table[0].first != '0') return 1;
    if (ascii_table[1].base != 0x8260 || ascii_table[1].first != 'A') return 2;
    if (ascii_table[2].base != 0x8281 || ascii_table[2].first != 'a') return 3;
    // The special entries list the punctuation bytes in ascending order, 33 of them, each once.
    int expected = 0x20;
    for (int i = 0; i < 33; ++i) {
        if (expected == 0x30) expected = 0x3a;
        if (expected == 0x41) expected = 0x5b;
        if (expected == 0x61) expected = 0x7b;
        if (ascii_special[i].ascii != expected) return 10 + i;
        ++expected;
    }
    // Known full-width forms: ideographic space, '!', ',', '.', '?', '~' (overline form).
    if (ascii_special[0].sjis != 0x8140 || ascii_special[1].sjis != 0x8149) return 4;
    if (ascii_special[12].sjis != 0x8143 || ascii_special[14].sjis != 0x8144) return 5;
    if (ascii_special[21].sjis != 0x8148 || ascii_special[32].sjis != 0x8150) return 6;
    // every code is a double-byte code in the 0x81 or 0x82 lead range
    for (const AsciiSpecial& e : ascii_special) {
        if ((e.sjis >> 8) != 0x81) return 7;
    }
    std::puts("ascii tables passed");
    return 0;
}
