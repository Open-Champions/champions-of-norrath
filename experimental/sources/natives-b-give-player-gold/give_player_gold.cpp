// Observed amxGivePlayerGold at 0x0013bb40, 72 bytes. For each of the
// nmPlayers records it adds the first cell divided by that count to
// the word at player+232. The record stride is 23984. The add wraps.
// player is the existing 95936-byte progression object.
extern "C" unsigned char nmPlayers[4];
extern "C" unsigned char norrath_player_progression_records_view[];

static int signed_word(const unsigned char* bytes) {
    const unsigned word = static_cast<unsigned>(bytes[0])
        | (static_cast<unsigned>(bytes[1]) << 8)
        | (static_cast<unsigned>(bytes[2]) << 16)
        | (static_cast<unsigned>(bytes[3]) << 24);
    return static_cast<int>(word);
}
static unsigned word_at(const unsigned char* bytes) {
    return static_cast<unsigned>(signed_word(bytes));
}
static void store_word(unsigned char* bytes, unsigned word) {
    bytes[0] = static_cast<unsigned char>(word);
    bytes[1] = static_cast<unsigned char>(word >> 8);
    bytes[2] = static_cast<unsigned char>(word >> 16);
    bytes[3] = static_cast<unsigned char>(word >> 24);
}

extern "C" int amxGivePlayerGold__FP5__amxPi(void*, int* params) {
    const int count = signed_word(nmPlayers);
    if (count <= 0) return 0;
    unsigned char* gold = norrath_player_progression_records_view + 232;
    int index = 0;
    do {
        const int amount = params[1];
        index += 1;
        const unsigned sum = word_at(gold) + static_cast<unsigned>(amount / count);
        store_word(gold, sum);
        gold += 23984;
    } while (index < count);
    return 0;
}
