// Observed actionCoolOff at 0x00486c58: 4 bytes of zero-filled storage (small BSS) between errorCondition and delayCount. mcHeartBeat and
// MC_LoadGameBrowser/MC_SaveGameBrowser read it as a word; MC_NewGame, asyncLoadGame, asyncSaveGame and the
// config save routines store to it.
// Inferred: nothing beyond those accesses. No field meaning is claimed.
// Unknown: what the value represents and its full range.
extern "C" {
int actionCoolOff;
}
