// Observed strcmp at 0x003044fc, 324 bytes. Aligned inputs use 16-byte and
// 8-byte equality scans, then this byte result. The difference is the
// unsigned value of the first mismatched byte, or zero when both end.
extern "C" int strcmp(const char* left, const char* right) {
    const auto* a = reinterpret_cast<const unsigned char*>(left);
    const auto* b = reinterpret_cast<const unsigned char*>(right);
    for (;;) {
        const unsigned char left_byte = *a++;
        const unsigned char right_byte = *b++;
        if (left_byte != right_byte || left_byte == 0u) return int(left_byte) - int(right_byte);
    }
}
