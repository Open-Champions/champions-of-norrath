// Observed decode_uleb128 at 0x002fbd18, 68 bytes (a local routine of the C++
// exception runtime). It reads bytes from the cursor in a0: the low seven bits
// of each byte are or-ed into a 32-bit result at shift 0, 7, 14, ... and the
// byte's top bit says another byte follows. The result is stored as one 32-bit
// word through a1 and the advanced cursor (one past the last byte read) is
// returned. There is no limit on the shift; at shift 32 and above the variable
// shift uses only the low five bits (the same as the compiled C++ shift).
extern "C" const unsigned char* decode_uleb128(const unsigned char* cursor, unsigned* value) {
    unsigned result = 0;
    unsigned shift = 0;
    unsigned char byte;
    do {
        byte = *cursor++;
        result |= static_cast<unsigned>(byte & 0x7f) << (shift & 31);
        shift += 7;
    } while (byte & 0x80);
    *value = result;
    return cursor;
}
