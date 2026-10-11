#include <cstring>
#include <cstdint>

extern "C" {
unsigned char stream[1360];
void soundFadeStream__Fi(int index);
}

static unsigned read_word(unsigned offset) {
    return static_cast<unsigned>(stream[offset]) |
           (static_cast<unsigned>(stream[offset + 1u]) << 8) |
           (static_cast<unsigned>(stream[offset + 2u]) << 16) |
           (static_cast<unsigned>(stream[offset + 3u]) << 24);
}

int main() {
    std::memset(stream, 0xa5, sizeof stream);
    stream[0x1c0] = stream[0x1c1] = stream[0x1c2] = stream[0x1c3] = 0;
    const unsigned second = 452u + 0x1c0u;
    stream[second] = stream[second + 1u] = stream[second + 2u] = stream[second + 3u] = 0;
    soundFadeStream__Fi(0);
    if (stream[64] != 0) return 1;
    if (read_word(108) != 2u) return 2;
    if (read_word(0x50) != 0u || read_word(0x54) != 0u || read_word(0x58) != 0u ||
        read_word(0x5c) != 0u)
        return 3;
    if (read_word(0x1c0) != 1u) return 4;
    if (stream[63] != 0xa5 || stream[65] != 0xa5) return 5;
    if (stream[107] != 0xa5 || stream[112] != 0xa5) return 6;

    for (int n = 0; n < 7; ++n) soundFadeStream__Fi(1);
    if (read_word(452u + 0x1c0u) != 7u) return 7;
    const unsigned inner = 6u * 48u + 452u;
    if (stream[inner + 64u] != 0 || read_word(inner + 108u) != 2u) return 8;
    soundFadeStream__Fi(1);
    if (read_word(452u + 0x1c0u) != 8u) return 9;
    stream[452u + 64u] = 0xa5;
    soundFadeStream__Fi(1);
    if (read_word(452u + 0x1c0u) != 8u || stream[452u + 64u] != 0xa5) return 10;
    return 0;
}
