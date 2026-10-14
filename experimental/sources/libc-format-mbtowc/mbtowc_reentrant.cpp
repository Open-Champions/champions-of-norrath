// Observed _mbtowc_r at 0x0030b2e0. The reentrancy pointer and shift state
// are unused. A null input returns 0. A zero maximum returns -1 without a
// store. Otherwise the first byte is stored as a zero-extended 32-bit word
// and the result is 0 for a zero byte, 1 for every other byte.
extern "C" int norrath_factory_mbtowc_reentrant(unsigned char*, unsigned char* wide_character_word, const char* input, unsigned maximum_bytes, unsigned char*) {
    if (input == nullptr) return 0;
    if (maximum_bytes == 0u) return -1;
    const unsigned char byte = static_cast<unsigned char>(*input);
    unsigned char scratch[4] = {0, 0, 0, 0};
    unsigned char* destination = wide_character_word != nullptr ? wide_character_word : scratch;
    destination[0] = byte;
    destination[1] = 0;
    destination[2] = 0;
    destination[3] = 0;
    return byte != 0 ? 1 : 0;
}
