// Host contract for the HUD word objects. Not an EE plan input.
#include "hud_words.cpp"
int main() {
    if (fontCharAlpha != -1 || cacheLoadCache != 0 || finishFrameCount != 0) return 1;
    if (currScorePos != 0 || currActivityScore != 0.0f || curFont != 0 || coins != 0) return 2;
    if (sizeof(countUp) != 16 || sizeof(currScorePos) != 2 || sizeof(fontCharAlpha) != 4) return 3;
    for (int i = 0; i < 4; ++i) if (countUp[i] != 0) return 4;
    return 0;
}
