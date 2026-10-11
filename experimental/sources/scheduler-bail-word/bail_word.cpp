// Observed gameBailFromGameLoop at 0x00483f28, global object, 4 file bytes,
// all zero. GP 0x004879f0, displacement -15048. amxLeaveGame stores 1 or 101
// here. The authored import name is norrath_game_bail_from_loop_view.
// The reviewed -G0 profile does not recreate .sdata. The value is zero.
extern "C" {
unsigned char norrath_game_bail_from_loop_view[4];
}
