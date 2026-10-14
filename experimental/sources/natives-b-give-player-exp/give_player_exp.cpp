// Observed amxGivePlayerExp at 0x0013bae0, 92 bytes. While the
// re-read nmPlayers word stays above the index, it calls
// givePlayerExperience with that index and a freshly read first cell.
extern "C" unsigned char nmPlayers[4];
extern "C" void givePlayerExperience__Fii(int index, int amount);

static int signed_word(const unsigned char* bytes) {
    const unsigned word = static_cast<unsigned>(bytes[0])
        | (static_cast<unsigned>(bytes[1]) << 8)
        | (static_cast<unsigned>(bytes[2]) << 16)
        | (static_cast<unsigned>(bytes[3]) << 24);
    return static_cast<int>(word);
}

extern "C" int amxGivePlayerExp__FP5__amxPi(void*, int* params) {
    int count = signed_word(nmPlayers);
    if (count <= 0) return 0;
    int index = 0;
    do {
        const int amount = params[1];
        givePlayerExperience__Fii(index, amount);
        index += 1;
        count = signed_word(nmPlayers);
    } while (index < count);
    return 0;
}
