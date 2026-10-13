// Observed gameLineGetPtr__Fi at 0x279bf8, extent 24.
// Returns the pointer stored at index `line` of the table gameLines (0x8d57a8).
// There is no bounds check and the index is not compared with gameLineCount, so a
// negative or large index reads outside the table. norrath_game_line_get_ptr is
// the name the wave-maker constructor imports; it is defined as a second symbol
// for the same code.
extern "C" void* gameLines[];

extern "C" void* gameLineGetPtr__Fi(int line) {
    return gameLines[line];
}

#if defined(__mips__)
extern "C" void* norrath_game_line_get_ptr(int line) __attribute__((alias("gameLineGetPtr__Fi")));
#endif
