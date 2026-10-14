// Observed gameSomeoneInShop__Fv at 0x001d1158, 116 bytes. The signed byte
// netNetworkMode (0x00483829) and the word nmPlayersAlive (0x00484f44) are read. A zero
// mode or a nonpositive count returns 0. Otherwise each of the first
// nmPlayersAlive entries of playersAlive (0x0042a3e8) is used as an index into
// player (0x00408600, stride 23984). The word at +21872 of that record is
// a pointer, and bit 0x800000 of the word at +12 of what it points to is
// tested. The first set bit returns 1; if none is set the result is 0.
// Unknown: what the pointer at +21872 refers to and what bit 0x800000 means;
// the name only suggests a shop state.
extern "C" signed char netNetworkMode;
extern "C" int nmPlayersAlive;
extern "C" int playersAlive[];
extern "C" unsigned char player[];

extern "C" int gameSomeoneInShop__Fv() {
    if (netNetworkMode == 0) return 0;
    int count = nmPlayersAlive;
    for (int i = 0; i < count; ++i) {
        const unsigned char* record = player + playersAlive[i] * 23984;
        const unsigned char* object = *reinterpret_cast<const unsigned char* const*>(record + 21872);
        if (*reinterpret_cast<const unsigned*>(object + 12) & 0x800000u) return 1;
    }
    return 0;
}
