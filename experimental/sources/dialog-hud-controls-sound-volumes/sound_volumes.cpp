// Observed soundSfxVolume and soundMusicVolume at 0x00485930 and 0x00485934.
// Both words are the float bits 0x3f800000. soundStereoEnable at 0x00485938
// is the word 1.
extern "C" unsigned soundSfxVolume = 0x3f800000u;
extern "C" unsigned soundMusicVolume = 0x3f800000u;
extern "C" unsigned soundStereoEnable = 1u;
