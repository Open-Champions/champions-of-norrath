// Host contract for the _ctype_ table of w8-startup-sce-a-ctype-table. Not an EE plan input.
// The table is indexed by character + 1. For the 128 ASCII characters its class masks must agree
// with the host's own classification in the C locale; characters 128..255 and slot 0 are zero.
// Mask bits: 0x01 upper, 0x02 lower, 0x04 digit, 0x08 white space, 0x10 punctuation, 0x20 control,
// 0x40 hexadecimal letter (A-F, a-f; the decimal digits carry only 0x04), 0x80 the blank (only the space character carries it, beside white space).
#include <cctype>
#include <clocale>
#include <cstdio>
#include "../w8-startup-sce-a-ctype-table/ctype_table.cpp"

int main() {
    std::setlocale(LC_ALL, "C");
    int failures = 0;
    if (sizeof(_ctype_) != 257) { std::printf("size %zu\n", sizeof(_ctype_)); return 1; }
    if (_ctype_.bytes[0] != 0) { std::printf("slot 0\n"); ++failures; }
    for (int c = 0; c < 128; ++c) {
        unsigned want = 0;
        if (std::isupper(c)) want |= 0x01;
        if (std::islower(c)) want |= 0x02;
        if (std::isdigit(c)) want |= 0x04;
        if (std::isspace(c)) want |= 0x08;
        if (std::ispunct(c)) want |= 0x10;
        if (std::iscntrl(c)) want |= 0x20;
        if (std::isxdigit(c) && !std::isdigit(c)) want |= 0x40;   // the 0x40 bit is set for A-F and a-f only
        if (c == ' ') want |= 0x80;
        if (_ctype_.bytes[1 + c] != want) {
            std::printf("char %d: table %02x host %02x\n", c, _ctype_.bytes[1 + c], want);
            ++failures;
        }
    }
    for (int c = 128; c < 256; ++c)
        if (_ctype_.bytes[1 + c] != 0) { std::printf("char %d not zero\n", c); ++failures; }
    if (failures) return 2;
    std::puts("ctype table agrees with the host C locale");
    return 0;
}
