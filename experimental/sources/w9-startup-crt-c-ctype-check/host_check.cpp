// Host check for the _ctype_ table authored in w8-startup-sce-a-ctype-table (that unit has no
// committed check). The table is compared, character by character, with the host's own
// classification in the "C" locale: byte 0 is zero; byte 1 + c carries 0x01 upper, 0x02 lower,
// 0x04 digit, 0x08 white space, 0x10 punctuation, 0x20 control, 0x40 hexadecimal letter (A-F, a-f; the digits carry only 0x04) and 0x80
// for the space character only; characters 128..255 are all zero. The same expected bytes
// were also compared against the executable image once, privately, and matched.
#include "../w8-startup-sce-a-ctype-table/ctype_table.cpp"
#include <cctype>
#include <clocale>

int main() {
    std::setlocale(LC_ALL, "C");
    static_assert(sizeof(CharacterTable) == 257, "table size");
    if (_ctype_.bytes[0] != 0) return 1;
    for (int c = 0; c < 256; ++c) {
        unsigned expected = 0;
        if (c < 128) {
            if (std::isupper(c)) expected |= 0x01;
            if (std::islower(c)) expected |= 0x02;
            if (std::isdigit(c)) expected |= 0x04;
            if (std::isspace(c)) expected |= 0x08;
            if (std::ispunct(c)) expected |= 0x10;
            if (std::iscntrl(c)) expected |= 0x20;
            if (std::isxdigit(c) && !std::isdigit(c)) expected |= 0x40;  // letters A-F, a-f only
            if (c == ' ') expected |= 0x80;
        }
        if (_ctype_.bytes[1 + c] != expected) return 2;
    }
    return 0;
}
