// Observed nmPlayers at 0x484538, 4 bytes, value 1. The next symbol, activePlayer,
// begins at 0x48453c. norrath_player_active_count_word_view is this same object.
extern "C" unsigned char nmPlayers[4] = {0x01, 0x00, 0x00, 0x00};
#if defined(__APPLE__)
asm(
    ".globl _norrath_player_active_count_word_view\n"
    ".set _norrath_player_active_count_word_view, _nmPlayers\n");
#else
extern "C" unsigned char norrath_player_active_count_word_view[4] __attribute__((alias("nmPlayers")));
#endif
