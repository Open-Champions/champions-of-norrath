// gameDeathTimerSingle at 0x00484f88, 8 bytes, in SLUS_205.65.
// Observed: eight zero bytes in .data. The image indexes it with halfword loads scaled by two from a player index (doNetCamToggle, gameCheckForDeathScreen), so it is four halfwords. Authored code elsewhere may declare it as eight bytes; both are the same 8 bytes.
extern "C" {
short gameDeathTimerSingle[4] = {};
}
