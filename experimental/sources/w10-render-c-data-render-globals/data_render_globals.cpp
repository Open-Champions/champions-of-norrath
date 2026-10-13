// Observed data of the render package, from the pinned SLUS_205.65 symbol table and the file
// bytes. Types come from the load and store forms in the authored callers; placement in the
// original data areas is not reproduced.
//
//   modelDrawFade        0x0048407d, 1 byte,  file 0          (byte access)
//   modelShadowScale     0x00484080, 4 bytes, file 1.0f       (float access)
//   modelOption          0x0048408c, 4 bytes, file 0          (word access)
//   shadowBrightness     0x00484094, 4 bytes, file 0.25f      (float access)
//   scissor              0x0046ccf0, 48 bytes, read-only, twelve floats: 320 224 120 100 0 0
//                        followed by the same six again (two groups of six)
//   scx1 scy1 scx2 scy2  0x00486938, 0x0048693c, 0x00486940, 0x00486944, 4 bytes each,
//                        uninitialised (word access)
//   rainSplashParticle   0x006ebaf0, 80 bytes, uninitialised (distance to the next symbol); the
//                        file-local objects of the same name at 0x007d7440 are not this one
//   slideDustParticle    0x008d55b8, slideDustParticleDark 0x008d5608, slideDustParticleBig
//   slideDustParticleBig 0x008d5658, slideDustParticleBigDark 0x008d56a8, 80 bytes each,
//                        uninitialised
// Not defined here: norrath_container_msg_run_fuseParticle_bytes and
// norrath_container_msg_run_smokeParticle_bytes, the 80-byte objects at 0x007d7720
// (fuseParticle) and 0x007d7770 (smokeParticle) that msg_run__9Container passes to
// P_AddParticle; the existing source w9-render-math-c-switch-static-init already defines them
// (the names are the authored caller's placeholders, not image symbols), so defining them again
// would clash once both units are in a plan.
// Not defined here: globalShadowBrightness (0x00484090, file 0.25f) and halos, which the
// w10-render-b data unit defines. The meanings of the scissor values, scx1..scy2 and
// modelOption are unknown; the particle objects are filled by static-initialiser code outside
// this unit.
extern "C" {
extern const float scissor[12];   // const objects have internal linkage unless declared extern first
unsigned char modelDrawFade = 0;
float modelShadowScale = 1.0f;
int modelOption = 0;
float shadowBrightness = 0.25f;
const float scissor[12] = {320.0f, 224.0f, 120.0f, 100.0f, 0.0f, 0.0f, 320.0f, 224.0f, 120.0f, 100.0f, 0.0f, 0.0f};
int scx1;
int scy1;
int scx2;
int scy2;
unsigned char rainSplashParticle[80];
unsigned char slideDustParticle[80];
unsigned char slideDustParticleDark[80];
unsigned char slideDustParticleBig[80];
unsigned char slideDustParticleBigDark[80];
}
