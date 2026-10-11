// Observed gameStateData at 0x007d35e8 in .bss. The symbol table records
// size 0. The next symbol, postAction, is 8192 bytes later. scriptGetGameState
// walks name/value pairs until a zero byte, so the initial byte is zero.
extern "C" {
unsigned char norrath_script_game_state_data_view[8192];
}
