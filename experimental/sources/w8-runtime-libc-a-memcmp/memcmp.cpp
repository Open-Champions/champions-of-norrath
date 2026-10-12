// Observed memcmp at 0x00301f68, 148 bytes. When the count is at least 16 and
// both pointers are 16-byte aligned, 16-byte blocks are compared first; a
// differing block is rewound and handled by the byte loop. The result is the
// unsigned difference of the first mismatching bytes, or 0 when the count runs
// out. The count is unsigned. Only that byte result is authored here; the
// quadword scan is not reproduced.
extern "C" int memcmp(const void* left, const void* right, unsigned count) {
    const auto* a = static_cast<const unsigned char*>(left);
    const auto* b = static_cast<const unsigned char*>(right);
    while (count != 0u) {
        const unsigned char left_byte = *a++;
        const unsigned char right_byte = *b++;
        if (left_byte != right_byte) return int(left_byte) - int(right_byte);
        --count;
    }
    return 0;
}
