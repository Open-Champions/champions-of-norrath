// Observed gameStateData at 0x007d35e8, BSS in SLUS_205.65. The symbol table records size 0;
// the next symbol (postAction, 0x007d55e8) begins 8192 bytes later, so the block is at most
// 8192 bytes. Users walk it as packed zero-terminated entries until a zero byte, so it starts
// zeroed. This unit supplies the symbol name that the importers use. The independently
// authored script-amx-game-state unit defines the same 8192 bytes under the name
// norrath_script_game_state_data_view; an integrator that links both owns two copies and
// should keep one.
// Unknown: the true size (8192 is the distance to the next symbol, not a recorded length).
extern "C" {
char gameStateData[8192];
}
