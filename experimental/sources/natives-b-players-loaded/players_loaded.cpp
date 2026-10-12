// Observed amxPlayersLoaded at 0x0013c478, 80 bytes. It returns 1
// when any of the nmPlayers creature words at player+21872 is
// nonzero. The record stride is 23984. An empty count returns 0.
extern "C" unsigned char nmPlayers[4];
extern "C" unsigned char norrath_player_progression_records_view[];

static int signed_word(const unsigned char* bytes) {
    const unsigned word = static_cast<unsigned>(bytes[0])
        | (static_cast<unsigned>(bytes[1]) << 8)
        | (static_cast<unsigned>(bytes[2]) << 16)
        | (static_cast<unsigned>(bytes[3]) << 24);
    return static_cast<int>(word);
}

extern "C" int amxPlayersLoaded__FP5__amxPi(void*, int*) {
    const int count = signed_word(nmPlayers);
    if (count <= 0) return 0;
    unsigned char* slot = norrath_player_progression_records_view + 21872;
    for (int index = 0; index < count; ++index) {
        void* creature = *reinterpret_cast<void**>(slot);
        slot += 23984;
        if (creature != nullptr) return 1;
    }
    return 0;
}
