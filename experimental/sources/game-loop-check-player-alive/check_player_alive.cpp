// Observed gameCheckPlayerAlive at 0x001d0fa8, 76 bytes.
// The signed word nmPlayers is loaded once. A nonpositive count returns 0
// without reading a record. Otherwise each of that many records is visited.
// player is the base. The stride is 23984 bytes. The float at byte 172 is
// counted when it compares greater than 0. Equal, negative, and unordered
// values are not counted. The return is that count.
extern "C" int nmPlayers;
extern "C" unsigned char player[];

namespace {
float load_f32(const unsigned char* address) {
    unsigned bits = static_cast<unsigned>(address[0]) |
                    (static_cast<unsigned>(address[1]) << 8) |
                    (static_cast<unsigned>(address[2]) << 16) |
                    (static_cast<unsigned>(address[3]) << 24);
    union {
        unsigned as_bits;
        float as_float;
    } value;
    value.as_bits = bits;
    return value.as_float;
}
}

extern "C" int gameCheckPlayerAlive__Fv() {
    int remaining = nmPlayers;
    if (remaining <= 0) return 0;
    int count = 0;
    const unsigned char* slot = player + 172;
    while (remaining > 0) {
        if (0.0f < load_f32(slot)) ++count;
        slot += 23984;
        --remaining;
    }
    return count;
}
