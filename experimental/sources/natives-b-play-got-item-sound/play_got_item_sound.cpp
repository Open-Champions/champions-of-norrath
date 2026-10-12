// Observed amxPlayGotItemSound at 0x0013bc28, 128 bytes. The sound
// player starts as activePlayer at 0x0048453c. A negative value stores
// 0 there, and a value of 2 or more stores 1. When nmPlayers is at
// least 2 and the hit-point float at player+172 for the original
// index is less than or equal to zero, the sound player becomes 1
// when the updated activePlayer is not 1. The sound id is 9.
extern "C" int activePlayer;
extern "C" unsigned char nmPlayers[4];
extern "C" unsigned char norrath_player_progression_records_view[];
extern "C" void playerPlaySound__Fii(int player, int sound);

static int signed_word(const unsigned char* bytes) {
    const unsigned word = static_cast<unsigned>(bytes[0])
        | (static_cast<unsigned>(bytes[1]) << 8)
        | (static_cast<unsigned>(bytes[2]) << 16)
        | (static_cast<unsigned>(bytes[3]) << 24);
    return static_cast<int>(word);
}
static float float_at(const unsigned char* bytes) {
    union Bits { unsigned word; float value; } bits;
    bits.word = static_cast<unsigned>(signed_word(bytes));
    return bits.value;
}

extern "C" int amxPlayGotItemSound__FP5__amxPi(void*, int*) {
    int sound_player = activePlayer;
    if (sound_player < 0) activePlayer = 0;
    else if (sound_player >= 2) activePlayer = 1;
    if (signed_word(nmPlayers) >= 2) {
        const float hp = float_at(norrath_player_progression_records_view
            + sound_player * 23984 + 172);
        if (hp <= 0.f) sound_player = (activePlayer ^ 1) != 0 ? 1 : 0;
    }
    playerPlaySound__Fii(sound_player, 9);
    return 0;
}
