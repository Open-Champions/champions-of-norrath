// Observed: syncState is one 32-bit word at 0x00486c3c in zero-filled storage (no file bytes).
// The adjacent symbols are oldAsyncMode before it and blocksFree 4 bytes after. It is read by
// 7 and written by 8 functions of the card service group (for example asyncChangePlayers__Fv,
// asyncLoadGame__Fv, asyncSaveConfigSilent__Fv and MC_CheckPlayerImportOrChange__Fv); in
// asyncChangePlayers__Fv it selects one of 13 states, per the existing authored notes.
// Inferred: it is the current step of whichever card-service routine is running.
// Unknown: the full list of states of the other routines.

extern "C" {
int syncState;
}
static_assert(sizeof(syncState) == 4, "syncState");
