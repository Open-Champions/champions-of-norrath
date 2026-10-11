// Observed amxTakePlayerGold__FP5__amxPi at 0x0013bb88, 92 bytes. nmPlayers
// is the count. A non-positive count leaves player gold unchanged and
// returns 0. Otherwise cell 1 is divided by that count for every record.
// Gold is the word at player byte 232, then every 23984 bytes. The stored
// word is the unsigned difference, replaced with 0 when that difference is
// negative. The native returns 0.
extern "C" int nmPlayers;
extern "C" unsigned char player[];

extern "C" int amxTakePlayerGold__FP5__amxPi(void*, int* params) {
    const int count = nmPlayers;
    if (count <= 0) return 0;
    unsigned char* slot = player + 232;
    int index = 0;
    do {
        const int amount = params[1];
        index += 1;
        unsigned previous = 0;
        auto* previous_bytes = reinterpret_cast<unsigned char*>(&previous);
        previous_bytes[0] = slot[0];
        previous_bytes[1] = slot[1];
        previous_bytes[2] = slot[2];
        previous_bytes[3] = slot[3];
        const int share = amount / count;
        const unsigned difference = previous - static_cast<unsigned>(share);
        const unsigned stored = static_cast<int>(difference) < 0 ? 0u : difference;
        auto* stored_bytes = reinterpret_cast<const unsigned char*>(&stored);
        slot[0] = stored_bytes[0];
        slot[1] = stored_bytes[1];
        slot[2] = stored_bytes[2];
        slot[3] = stored_bytes[3];
        slot += 23984;
    } while (index < count);
    return 0;
}
