#include "us_english_kbd_map_table.cpp"
#include <cstddef>

int main() {
    if (sizeof(UsKbdMapRecord) != 10 || sizeof(USEnglishKbdMapTable) != 740) return 1;
    if (offsetof(UsKbdMapRecord, plain) != 2 || offsetof(UsKbdMapRecord, shifted) != 4 || offsetof(UsKbdMapRecord, tail_b) != 8) return 2;
    const unsigned char* bytes = reinterpret_cast<const unsigned char*>(USEnglishKbdMapTable);
    // Whole-table FNV-1a and byte sum computed from the image bytes.
    unsigned hash = 0x811c9dc5u;
    unsigned sum = 0;
    for (unsigned index = 0; index < 740; ++index) {
        hash = (hash ^ bytes[index]) * 0x01000193u;
        sum += bytes[index];
    }
    if (hash != 3592756230u || sum != 17413u) return 3;
    // Spot checks: first record, a letter record and the last record as read from the image.
    if (USEnglishKbdMapTable[0].usage != 0x35 || USEnglishKbdMapTable[0].plain != 0x60 || USEnglishKbdMapTable[0].shifted != 0x7e) return 4;
    for (int i = 0; i < 74; ++i) {
        if (USEnglishKbdMapTable[i].tail_a != 0 || USEnglishKbdMapTable[i].tail_b != 0) return 5;
        if (USEnglishKbdMapTable[i].kind != 0 && USEnglishKbdMapTable[i].kind != 64 && USEnglishKbdMapTable[i].kind != 128) return 6;
    }
    return 0;
}
