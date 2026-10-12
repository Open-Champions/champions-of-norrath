// Observed playerNetDead__Fv at 0x001ac380, 104 bytes (pinned SLUS_205.65).
// Walks indexes 0..nmPlayersAlive-1 (the count is re-read each pass) and asks
// netPlayerIsLocal for the player number stored in playersAlive[index]. It
// returns 0 at the first local player and 1 when none is local, including the
// case of an empty alive list. Inferred: the result means "no local player is
// alive". Unknown: callers' use of the value beyond the nonzero test.
extern "C" {
extern int nmPlayersAlive;
extern int playersAlive[4];
int netPlayerIsLocal__Fi(int index);

int playerNetDead__Fv() {
    for (int i = 0; i < nmPlayersAlive; ++i) {
        if (netPlayerIsLocal__Fi(playersAlive[i])) return 0;
    }
    return 1;
}
}
