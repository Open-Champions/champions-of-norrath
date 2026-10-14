// Observed in the pinned SLUS_205.65 (addresses, sizes and initial bytes):
//   fontCharAlpha     0x00484118, 4 bytes, initial word 0xffffffff
//   cacheLoadCache    0x004845c0, 4 bytes, initial 0
//   finishFrameCount  0x00484c94, 4 bytes, initial 0
//   currScorePos      0x00484cb6, 2 bytes, initial 0
//   currActivityScore 0x00484cb8, 4 bytes, initial 0
//   curFont           0x0048698c, 4 bytes, zero-initialised BSS (local symbol)
//   coins             0x00486aa8, 4 bytes, zero-initialised BSS (local symbol)
//   countUp           0x007d0900, 16 bytes of BSS (local symbol; the next symbol
//                     starts 16 bytes later and the authored caller indexes four words)
// Types are inferred from the authored callers: curFont and coins are pointers
// (coins receives a lump resource), currActivityScore is a float, currScorePos a
// halfword, and the others are ints. ELF sizes are 4 for the global ones; the
// local objects have no ELF size in the image.
// Unknown: initial-value ownership (whether the zeros were written or implied),
// and whether fontCharAlpha is signed.
extern "C" {
int fontCharAlpha = -1;
int cacheLoadCache = 0;
int finishFrameCount = 0;
short currScorePos = 0;
float currActivityScore = 0.0f;
void* curFont = 0;
void* coins = 0;
int countUp[4];
}
