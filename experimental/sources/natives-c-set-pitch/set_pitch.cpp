// Observed SetPitchAtLevel__6Cameraii at 0x0026fb80, 32 bytes.
// The pitch is stored as a 16-bit word at camera byte 108 + level*2.
// When level equals the word at byte 84, the same pitch is also stored
// at byte 106.
extern "C" void SetPitchAtLevel__6Cameraii(void* camera, int level, int pitch) {
    auto* bytes = static_cast<unsigned char*>(camera);
    int current = 0;
    auto* current_bytes = reinterpret_cast<unsigned char*>(&current);
    current_bytes[0] = bytes[84];
    current_bytes[1] = bytes[85];
    current_bytes[2] = bytes[86];
    current_bytes[3] = bytes[87];
    const unsigned scaled = static_cast<unsigned>(level) << 1;
    const unsigned slot = 108u + scaled;
    const unsigned short narrow = static_cast<unsigned short>(pitch);
    bytes[slot] = static_cast<unsigned char>(narrow);
    bytes[slot + 1] = static_cast<unsigned char>(narrow >> 8);
    if (level == current) {
        bytes[106] = static_cast<unsigned char>(narrow);
        bytes[107] = static_cast<unsigned char>(narrow >> 8);
    }
}
