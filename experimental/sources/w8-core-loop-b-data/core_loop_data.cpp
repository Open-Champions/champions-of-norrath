// Observed data of the core loop package. Addresses and sizes are the pinned SLUS_205.65 symbol table; initial
// values are the file bytes (all zero unless noted). Where a symbol is in the uninitialised area the original
// holds no file bytes and zero here is the loader's state. Types come from the load and store forms in the
// instruction stream: a word access is declared int, a byte access unsigned char, and a float access float.
// Placement in the original data areas is not reproduced.
//
//   disableVolumetricLighting 0x00483aa8, 4 bytes, file 0       (word load and store)
//   currentRunningObject      0x00483acc, 4 bytes, file 0       (pointer; word load and store)
//   currentLayerDir           0x00483ff0, 4 bytes, file 0x006cc6a0 = layer0Dir (pointer)
//   drawBustOn                0x00484b0c, 1 byte,  file 0
//   drawCompassOn             0x00484b0d, 1 byte,  file 0
//   curFocus                  0x00484be0, 4 bytes, file 0
//   endInit                   0x00484bf0, 4 bytes, file 0
//   correction                0x00485058, 4 bytes, file 0       (float load and store)
//   coreSwitchScene           0x00486684, 4 bytes, file 0
//   coreSwitchSceneStart      0x00486688, 4 bytes, file 0
//   coreLevelCommentTimer     0x0048668c, 4 bytes, file 0
//   coreLoadingStart          0x00486698, 4 bytes, file 0
//   drawBustTexName           0x004286c0, 32 bytes, file all zero
//   drawBustCur, drawBustMax  0x00486ae4, 0x00486ae8, 4 bytes each, uninitialised
//   delayCount                0x00486c5c, 4 bytes, uninitialised
//   curPlaySeconds            0x00486ca4, 4 bytes, uninitialised
//   dummyTex                  0x006e5e40, 64 bytes, uninitialised (the next symbol starts 64 bytes later)
//   drawCompassPoint          0x007d0578, 16 bytes, uninitialised
//   drawBustTitle             0x007d0718, 64 bytes, uninitialised
//   drawCompassTitle          0x007d0758, 64 bytes, uninitialised
// Unknown: element types of the arrays (only their sizes are known from the symbol spacing) and the meaning of
// the values in them. Sizes of the uninitialised arrays are the distance to the next symbol, which bounds but
// does not prove the object size.
extern "C" unsigned char layer0Dir[];

extern "C" {
int disableVolumetricLighting = 0;
unsigned char* currentRunningObject = 0;
unsigned char* currentLayerDir = layer0Dir;
unsigned char drawBustOn = 0;
unsigned char drawCompassOn = 0;
int curFocus = 0;
int endInit = 0;
float correction = 0.0f;
int coreSwitchScene = 0;
int coreSwitchSceneStart = 0;
int coreLevelCommentTimer = 0;
int coreLoadingStart = 0;
unsigned char drawBustTexName[32] = {};
int drawBustCur = 0;
int drawBustMax = 0;
int delayCount = 0;
int curPlaySeconds = 0;
unsigned char dummyTex[64] = {};
unsigned char drawCompassPoint[16] = {};
unsigned char drawBustTitle[64] = {};
unsigned char drawCompassTitle[64] = {};
}
