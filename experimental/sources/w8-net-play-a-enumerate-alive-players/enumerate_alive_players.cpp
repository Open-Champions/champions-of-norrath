// Observed enumerateAlivePlayers__Fv at 0x001cd528, 292 bytes. Saves the old
// local-alive count into lastNmLocalPlayersAlive, resets validLocalPlayer to -1
// and both alive counts to zero, then walks player slots 0..nmPlayers-1 (the
// bound is re-read each pass). A slot is skipped when the pointer at record
// offset 21872 (record stride 23984 in `player`) is null or the word at
// pointee+12 has bit 3 set. Otherwise the slot index is appended to
// playersAlive; when netPlayerIsLocal(index) is nonzero it is also appended to
// localPlayersAlive, and validLocalPlayer takes it if still -1.
extern "C" {
extern unsigned char player[];
extern int nmPlayers;
extern int nmPlayersAlive;
extern int localNmPlayersAlive;
extern int lastNmLocalPlayersAlive;
extern int validLocalPlayer;
extern int playersAlive[4];
extern int localPlayersAlive[4];
int netPlayerIsLocal__Fi(int index);

void enumerateAlivePlayers__Fv() {
    lastNmLocalPlayersAlive = localNmPlayersAlive;
    validLocalPlayer = -1;
    nmPlayersAlive = 0;
    localNmPlayersAlive = 0;
    for (int i = 0; i < nmPlayers; ++i) {
        const unsigned char* record = player + i * 23984;
        const unsigned char* object = *reinterpret_cast<unsigned char* const*>(record + 21872);
        if (!object) continue;
        if (*reinterpret_cast<const unsigned*>(object + 12) & 8u) continue;
        playersAlive[nmPlayersAlive++] = i;
        if (netPlayerIsLocal__Fi(i)) {
            localPlayersAlive[localNmPlayersAlive++] = i;
            if (validLocalPlayer == -1) validLocalPlayer = i;
        }
    }
}
}
