// Observed data of the rendering, water and megatile callers, from the pinned SLUS_205.65 symbol
// table (addresses and distances to the next symbol) and the file bytes. Types follow the load and
// store forms in the authored callers; placement in the original data areas is not reproduced and
// no section identity is claimed.
//
//   mt_dyndat, mt_throwaway   0x00485c58, 0x00485c5c, 4 bytes each, file 0; word-sized values the
//                             authored world free routine handles as pointers
//   scaleTex                  0x00427248, 28 bytes, file all zero (its neighbours taskBar and
//                             moneyTex are 48 and 28 bytes)
//   waterTexture              0x004841a0, 4 bytes, file 0; the authored water property setter stores
//                             a pointer into it
//   waterList, waterListBack  0x004869b8, 0x004869bc, 4 bytes each, uninitialised pointers
//   splashInfoBits            0x00601fd0, 504 bytes (distance to noiseTable at 0x006021c8),
//                             uninitialised
//   waveMainAlternate 0x00600db0, waveRandomFoam 0x00600e00, waveTrail 0x00600e50,
//   waveSplashBig 0x00600ea0, waveSplashSmall 0x00600ef0, waveFroth 0x00600f40
//                             80 bytes each (distance to the next symbol), uninitialised; the
//                             wave-maker routines pass them as particle records
// Not defined here: lightIntensity, lightStart (w10-render-b-data-light-slots) and marble
// (w10-render-b-data-inventory-textures), which already have authored definitions.
// Unknown: the element types of scaleTex and splashInfoBits and what the 504 bytes of
// splashInfoBits hold beyond being indexed by bit.
extern "C" {
void* mt_dyndat = nullptr;
void* mt_throwaway = nullptr;
unsigned char scaleTex[28] = {};
void* waterTexture = nullptr;
unsigned char* waterList;
unsigned char* waterListBack;
unsigned char splashInfoBits[504];
unsigned char waveMainAlternate[80];
unsigned char waveRandomFoam[80];
unsigned char waveTrail[80];
unsigned char waveSplashBig[80];
unsigned char waveSplashSmall[80];
unsigned char waveFroth[80];
}
