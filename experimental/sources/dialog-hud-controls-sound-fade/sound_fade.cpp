// Observed soundFadeStream__Fi at 0x0024a5d0, 144 bytes.
// stream begins at 0x007dd290. The next symbol, gCd, is 1360 bytes later.
// The ELF size of stream is 0. For stream index i the counter word is at
// byte i * 452 + 0x1c0. A count of 8 or more returns. A smaller count n
// uses byte n * 48 + i * 452: it clears the byte at +64, stores 2 in the
// word at +108, clears the words at +0x50, +0x54, +0x58 and +0x5c, then
// adds one to the counter. stream is not defined here.
extern "C" unsigned char stream[];

static unsigned read_word(unsigned offset) {
    return static_cast<unsigned>(stream[offset]) |
           (static_cast<unsigned>(stream[offset + 1u]) << 8) |
           (static_cast<unsigned>(stream[offset + 2u]) << 16) |
           (static_cast<unsigned>(stream[offset + 3u]) << 24);
}

static void write_word(unsigned offset, unsigned value) {
    stream[offset] = static_cast<unsigned char>(value);
    stream[offset + 1u] = static_cast<unsigned char>(value >> 8);
    stream[offset + 2u] = static_cast<unsigned char>(value >> 16);
    stream[offset + 3u] = static_cast<unsigned char>(value >> 24);
}

extern "C" void soundFadeStream__Fi(int index) {
    const unsigned base = static_cast<unsigned>(index) * 452u;
    const unsigned counter = base + 0x1c0u;
    const unsigned count = read_word(counter);
    if (count >= 8u) return;
    const unsigned inner = count * 48u + base;
    stream[inner + 64u] = 0;
    write_word(inner + 108u, 2u);
    write_word(inner + 0x50u, 0u);
    write_word(inner + 0x54u, 0u);
    write_word(inner + 0x58u, 0u);
    write_word(inner + 0x5cu, 0u);
    write_word(counter, count + 1u);
}
