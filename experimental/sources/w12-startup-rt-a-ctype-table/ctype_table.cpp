// Observed _ctype_ at 0x0047e1f0, pinned SLUS_205.65: 257 bytes of read-only data, symbol size 257.
// Byte 0 is zero; byte 1 + c describes character c for c = 0..255. Each byte is a mask: 0x01 upper case,
// 0x02 lower case, 0x04 decimal digit, 0x08 white space, 0x10 punctuation, 0x20 control, 0x40 hexadecimal
// letter (A-F and a-f only; the digits carry just 0x04), 0x80 the blank (only the space, together with
// 0x08). Characters 9..13 are 0x28 (white space and control), the other controls and 127 are 0x20, and
// every character from 128 up is 0. The table below is generated from those rules; the object check
// compares it with the host's own C-locale classification and a private run compared it byte for byte
// with the image.
// Inferred: the 0x80 bit name and which library the table came from. Callers that index below the
// object (negative characters) read the bytes in front of it; this unit does not define them.
// Supersedes the unselected w8-startup-sce-a-ctype-table, which had no committed object check.
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
