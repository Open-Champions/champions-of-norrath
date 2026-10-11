// Observed coreClearColor at 0x0048667c, 4 bytes in .sdata.
// The file bytes are 00 00 00 80, the word 0x80000000.
// simpleDrawLoop passes this word to engineFrameStart.
extern "C" {
unsigned coreClearColor = 0x80000000u;
}
static_assert(sizeof(coreClearColor) == 4, "coreClearColor is one word");
