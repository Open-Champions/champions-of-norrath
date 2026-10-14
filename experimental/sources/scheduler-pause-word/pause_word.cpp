// Observed gamePause at 0x00483f14, global object, 4 file bytes, all zero.
// GP 0x004879f0, displacement -15068. gameIsPaused reads this word.
// The authored import name is norrath_game_pause_word_view.
// The reviewed -G0 profile does not recreate .sdata. The value is zero.
extern "C" {
unsigned char norrath_game_pause_word_view[4];
}
