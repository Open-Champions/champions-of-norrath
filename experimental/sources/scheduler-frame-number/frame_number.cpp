// Observed frameNm at 0x004844a4, global object, 4 file bytes, all zero.
// GP 0x004879f0, displacement -13644. gameIsPaused compares this word with 60.
// The authored import name is norrath_game_frame_number_word_view.
// The reviewed -G0 profile does not recreate .sdata. The value is zero.
extern "C" {
unsigned char norrath_game_frame_number_word_view[4];
}
