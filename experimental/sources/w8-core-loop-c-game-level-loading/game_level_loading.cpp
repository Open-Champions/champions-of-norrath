// gameLevelLoading at 0x00484cd4, 4 bytes, in SLUS_205.65.
// Observed: zero-initial word in .data accessed with word loads and stores.
extern "C" {
int gameLevelLoading = 0;
}
