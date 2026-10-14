#include "../w15-vi-strings-dict-a-vistring-conversion-table-1252/table.cpp"
#include "str8to16.cpp"
int main() {
    char src[260]; for (int i = 0; i < 255; ++i) src[i] = static_cast<char>(i + 1); src[255] = 0;
    // every byte value 1..255 maps as documented (low bytes unchanged, 0x80..0x9f via the table)
    for (unsigned limit = 0; limit < 600; limit += 1) {
        unsigned short buf[270]; for (auto& x : buf) x = 0x5a5a;
        if (str8to16__8VIStringPUsUiPCc(buf + 1, limit, src) != buf + 1) return 1;
        unsigned k = 0;
        while (k < 255 && 2 * (k + 1) < limit) ++k;
        for (unsigned i = 0; i < 270; ++i) {
            unsigned short want = 0x5a5a;
            if (i >= 1 && i < 1 + k) {
                unsigned c = i; // byte value = index of char = i (since src[i-1] = i)
                want = (c >= 0x80 && c < 0xa0) ? _8VIString$ConversionTable1252[c - 0x80] : c;
            } else if (i == 1 + k) want = 0;
            if (buf[i] != want) return 2;
        }
    }
    unsigned short b2[4] = {7, 7, 7, 7};
    str8to16__8VIStringPUsUiPCc(b2, 100, "");
    return b2[0] == 0 && b2[1] == 7 ? 0 : 3;
}
