// Observed: IsIopStreamingEnabled__6VIFile at 0x010cade8, 12 bytes. It returns the
// 32-bit word held in the data symbol _6VIFile$sIopStream (0x01192c84, 4 bytes) and
// has no other effect; its sibling EnableIopStreaming__6VIFilei (0x010cadd8) sits just
// before it. Inferred: the word is the class-wide flag the two functions share.
// Unknown: who initialises the word, and whether values other than 0 and 1 occur.
extern "C" {
extern int norrath_file_iop_stream __asm__("_6VIFile$sIopStream");

int IsIopStreamingEnabled__6VIFile() {
    return norrath_file_iop_stream;
}
}
