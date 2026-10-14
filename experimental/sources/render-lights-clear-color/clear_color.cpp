// Observed coreClearColor at 0x48667c, 4 bytes in .sdata.
// The file bytes are 00 00 00 80. coreLoading begins at the next address.
// The factory stores an 0x80000000-based word here. This object is the
// initial word only; no writer is defined in this unit.
extern "C" {
unsigned char norrath_factory_clear_color_word[4] = {0x00, 0x00, 0x00, 0x80};
}
