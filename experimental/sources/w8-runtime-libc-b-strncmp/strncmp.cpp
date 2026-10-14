// Observed strncmp at 0x00304f60, 440 bytes. It compares at most count bytes
// as unsigned values and returns the difference of the first mismatched pair.
// It returns 0 when the count is zero, when count bytes match, or when the
// matching bytes reach a zero. Aligned inputs compare 8- or 16-byte blocks
// first; the authored function is the byte result. The same original is also
// authored as norrath_factory_strncmp with its block scans.
extern "C" int strncmp(const char* left, const char* right, unsigned count) {
    const auto* a = reinterpret_cast<const unsigned char*>(left);
    const auto* b = reinterpret_cast<const unsigned char*>(right);
    while (count != 0u) {
        const unsigned char left_byte = *a++;
        const unsigned char right_byte = *b++;
        if (left_byte != right_byte) return int(left_byte) - int(right_byte);
        if (left_byte == 0u) return 0;
        --count;
    }
    return 0;
}
