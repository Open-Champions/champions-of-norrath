// Observed: _ctype_ at 0x0047e1f0, 257 bytes of read-only data. Byte 0 is
// zero; byte 1 + c describes character c for c in 0..255. The values are
// class bit masks: 0x01 upper, 0x02 lower, 0x04 digit, 0x08 white space,
// 0x10 punctuation, 0x20 control, 0x40 hexadecimal digit, 0x80 the blank
// (the space character carries it together with white space). Characters 0
// through 127 follow the usual ASCII classes and characters 128 through 255
// are all zero. This source builds the table from those rules instead of
// listing bytes; the host check compares the result with the executable.
// Callers also read up to 127 bytes before the object (libc-format keeps a
// separate view for that); this object does not cover them.
// Unknown: which library the original table came from.
struct CharacterTable {
    unsigned char bytes[257];
    constexpr CharacterTable() : bytes() {
        for (int c = 0; c < 128; ++c) {
            unsigned char mask = 0;
            if (c >= 'A' && c <= 'Z') mask = 0x01;
            else if (c >= 'a' && c <= 'z') mask = 0x02;
            else if (c >= '0' && c <= '9') mask = 0x04;
            else if (c == ' ') mask = 0x88;
            else if (c >= 9 && c <= 13) mask = 0x28;
            else if (c < 32 || c == 127) mask = 0x20;
            else mask = 0x10;
            if ((c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f')) mask |= 0x40;
            bytes[1 + c] = mask;
        }
    }
};

extern "C" {
extern const CharacterTable _ctype_;
const CharacterTable _ctype_ = CharacterTable();
}
