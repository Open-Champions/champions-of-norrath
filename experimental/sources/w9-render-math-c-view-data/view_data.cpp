// Data definitions for seven globals, addresses and sizes from the private image's symbol
// table; initial bytes read from the file image; access widths from the load/store
// instructions that use them. Nothing here executes.
//
//  myYaw             0x0048431c  2 bytes, file value 0xe000 (-8192 signed), halfword
//                    loads/stores (lh/sh). Neighbours myPitch (0x2710) and myRoll (0).
//  texDecodeStarted  0x0048410c  4 bytes, file value 0, word loads/stores.
//  worldViewMatrix   0x0037ef30  64 bytes, file bytes all zero. Followed at 0x0037ef70
//                    by worldWorldViewMatrix and at 0x0037efb0 by worldCharViewMatrix.
//  worldCharViewMatrix 0x0037efb0 64 bytes, file bytes all zero. Read as 16 floats.
//  scissored         0x004869a4  4 bytes, uninitialized (BSS), word access.
//  vifDirectQWordCount 0x00486990 4 bytes, uninitialized (BSS), word access.
//  yaws              0x008d5b78  24 bytes, uninitialized (BSS); written as signed
//                    halfwords (twelve elements) by the select screen setup.
// Inferred: view matrices are 4x4 float matrices. Unknown: the real element types of
// scissored and vifDirectQWordCount (only word-wide access is observed), and who
// writes worldViewMatrix and worldCharViewMatrix at run time.
extern "C" {
short myYaw = -8192;
int texDecodeStarted = 0;
float worldViewMatrix[16];
float worldCharViewMatrix[16];
int scissored;
int vifDirectQWordCount;
short yaws[12];
}
