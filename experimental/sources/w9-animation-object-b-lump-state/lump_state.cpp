// Observed lump loader state words, both in .sdata with initial value 0:
//   myLumpBuffer     0x484c88, 4 bytes. A pointer; the next symbol, askedForConfigLoad,
//                    begins at 0x484c8c. The config lump is loaded into it.
//   initializedLump  0x484c90, 4 bytes (the next symbol, finishFrameCount, begins at
//                    0x484c94). A flag set once the config lump has been added.
// Inferred: the pointer is to a 256-byte-aligned load buffer (readers align the
// address they store). Unknown: whether other lumps use these words.
extern "C" {
void* myLumpBuffer = nullptr;
int initializedLump = 0;
}
