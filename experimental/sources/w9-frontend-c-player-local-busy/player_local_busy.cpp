// Observed playerLocalPlayerBusy__Fv at 0x001ac3e8, 164 bytes (pinned
// SLUS_205.65). Walks indexes 0..nmPlayersAlive-1 (count re-read each pass).
// For the first player number in playersAlive[index] that netPlayerIsLocal
// accepts, it follows the pointer at byte 21872 of that player record (stride
// 23984 in `player`, no null check) and returns whether bit 22 (0x400000) of
// the word at pointee+12 is set, as 0 or 1. With no local player it returns 0.
// Unknown: what bit 22 of that word means; the original code gives it no name.
extern "C" {
extern unsigned char player[];
extern int nmPlayersAlive;
extern int playersAlive[4];
int netPlayerIsLocal__Fi(int index);

int playerLocalPlayerBusy__Fv() {
    for (int i = 0; i < nmPlayersAlive; ++i) {
        int who = playersAlive[i];
        if (netPlayerIsLocal__Fi(who)) {
            const unsigned char* object =
                *reinterpret_cast<unsigned char* const*>(player + who * 23984 + 21872);
            return (*reinterpret_cast<const unsigned*>(object + 12) & 0x400000u) != 0;
        }
    }
    return 0;
}
}
