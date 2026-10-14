#include "kbd_map_table.cpp"

#include <cstddef>

int main() {
    if (sizeof(KbdMapRecord) != 10 || sizeof(KbdMapTable) != 750) return 1;
    if (offsetof(KbdMapRecord, plain) != 2 || offsetof(KbdMapRecord, shifted) != 4 ||
        offsetof(KbdMapRecord, tail_a) != 6 || offsetof(KbdMapRecord, tail_b) != 8) return 2;
    const unsigned char* bytes = reinterpret_cast<const unsigned char*>(KbdMapTable);
    // Whole-table check against values computed from the image bytes (FNV-1a and byte sum).
    unsigned hash = 0x811c9dc5u;
    unsigned sum = 0;
    for (unsigned index = 0; index < 750; ++index) {
        hash = (hash ^ bytes[index]) * 0x01000193u;
        sum += bytes[index];
    }
    if (hash != 2996888148u || sum != 12487u) return 3;
    // Spot checks: first, second and last record, as read from the image.
    const unsigned char first[10] = {0x00, 0x35, 0x60, 0x00, 0x7e, 0x00, 0, 0, 0, 0};
    const unsigned char second[10] = {0x00, 0x1e, 0x31, 0x00, 0x21, 0x00, 0, 0, 0, 0};
    const unsigned char last[10] = {0x02, 0x63, 0x2e, 0x00, 0x00, 0x00, 0, 0, 0, 0};
    for (int index = 0; index < 10; ++index) {
        if (bytes[index] != first[index]) return 4;
        if (bytes[10 + index] != second[index]) return 5;
        if (bytes[740 + index] != last[index]) return 6;
    }
    return 0;
}
