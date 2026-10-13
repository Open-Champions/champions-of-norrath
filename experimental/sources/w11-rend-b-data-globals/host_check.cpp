// Host contract for the data definitions. Not an EE plan input.
#include "data_globals.cpp"
#include <cstdio>
static_assert(sizeof scaleTex == 28 && sizeof splashInfoBits == 504 && sizeof waveFroth == 80, "sizes");
int main() {
    if (mt_dyndat || mt_throwaway || waterTexture || waterList || waterListBack) return 1;
    for (unsigned char b : scaleTex) if (b) return 2;
    for (unsigned char b : splashInfoBits) if (b) return 3;
    unsigned char* waves[] = {waveMainAlternate, waveRandomFoam, waveTrail, waveSplashBig, waveSplashSmall, waveFroth};
    for (unsigned char* w : waves) for (int i = 0; i < 80; i++) if (w[i]) return 4;
    if (waves[0] == waves[1] || waves[2] == waves[3]) return 5;
    std::printf("data globals contract passed\n");
    return 0;
}
