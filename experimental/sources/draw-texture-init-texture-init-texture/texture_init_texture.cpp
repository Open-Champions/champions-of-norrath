// Observed textureInitTexture__FP8_texture at 0x0016d160, 276 bytes.
// A nonzero word at offset 28 returns before any store. Otherwise the halfword
// at offset 10 becomes 0xa7fb, the word at offset 28 becomes 0xffffffff, and
// the word at offset 16 is replaced by that word added to the texture address.
// Bit 0 of the halfword at offset 8 returns after those three stores. Bit 8
// of that halfword is the only path that reads the relocated address. That
// path adds the texture address to the first word, then reads 4-byte records
// until a byte of 0xff. Each record's following words, (byte2-byte0+1) by
// (byte3-byte1+1) when byte3 >= byte1, are likewise converted from relative
// words. The halfword at offset 6 becomes 72 plus, per record,
// trunc_div(((byte3-byte1)<<11)+2048)*(byte2-byte0+1), 128) + 6.
// Fresh compilation is not an original-byte match. No EE path has run.

extern "C" void textureInitTexture__FP8_texture(unsigned char* texture) {
    auto word_at = [](const unsigned char* address) {
        return static_cast<unsigned>(address[0]) | (static_cast<unsigned>(address[1]) << 8) |
               (static_cast<unsigned>(address[2]) << 16) | (static_cast<unsigned>(address[3]) << 24);
    };
    auto store_word = [](unsigned char* address, unsigned value) {
        address[0] = static_cast<unsigned char>(value);
        address[1] = static_cast<unsigned char>(value >> 8);
        address[2] = static_cast<unsigned char>(value >> 16);
        address[3] = static_cast<unsigned char>(value >> 24);
    };
    auto signed_byte = [](unsigned char value) {
        return static_cast<int>(static_cast<signed char>(value));
    };
    if (word_at(texture + 28) != 0u) return;

    const unsigned relative = word_at(texture + 16);
    const unsigned flags = static_cast<unsigned>(texture[8]) | (static_cast<unsigned>(texture[9]) << 8);
    const unsigned base = static_cast<unsigned>(reinterpret_cast<unsigned long>(texture));
    texture[10] = static_cast<unsigned char>(0xa7fb);
    texture[11] = static_cast<unsigned char>(0xa7fb >> 8);
    store_word(texture + 28, 0xffffffffu);
    store_word(texture + 16, base + relative);
    if ((flags & 0x1u) != 0u) return;
    if ((flags & 0x100u) == 0u) return;

    int accumulated = 72;
    unsigned char* cursor = texture + relative;
    store_word(cursor, word_at(cursor) + base);
    cursor += 4;
    if (signed_byte(cursor[0]) == -1) {
        texture[6] = static_cast<unsigned char>(accumulated);
        texture[7] = static_cast<unsigned char>(static_cast<unsigned>(accumulated) >> 8);
        return;
    }

    while (true) {
        const int byte0 = signed_byte(cursor[0]);
        const int byte1 = signed_byte(cursor[1]);
        const int byte2 = signed_byte(cursor[2]);
        const int byte3 = signed_byte(cursor[3]);
        cursor += 4;
        const int span_y = byte3 - byte1;
        const int span_x = (byte2 - byte0) + 1;
        int product = static_cast<int>((static_cast<long long>((span_y << 11) + 2048) * span_x) & 0xffffffffll);
        if (product < 0) product += 127;
        if (product < 0) product = static_cast<int>((static_cast<unsigned>(product) >> 7) | 0xfe000000u);
        else product = product >> 7;
        accumulated = (accumulated + product) + 6;
        if (byte3 >= byte1) {
            int row = byte1;
            do {
                int column = byte0;
                if (byte2 >= column) {
                    do {
                        store_word(cursor, word_at(cursor) + base);
                        cursor += 4;
                        column += 1;
                    } while (byte2 >= column);
                }
                row += 1;
            } while (byte3 >= row);
        }
        if (signed_byte(cursor[0]) == -1) break;
    }
    texture[6] = static_cast<unsigned char>(accumulated);
    texture[7] = static_cast<unsigned char>(static_cast<unsigned>(accumulated) >> 8);
}
