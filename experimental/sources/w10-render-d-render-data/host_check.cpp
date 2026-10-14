// Host check for the render data objects: sizes and initial values.
#include <cstdio>
extern "C" {
extern int spellAlpha[4];
extern int spellAlphaInc[4];
extern int texLastDecompressionTime;
extern float worldTileEntityLightScale;
extern unsigned char* waterPatch;
}
int main() {
    int bad = 0;
    for (int i = 0; i < 4; ++i) bad |= spellAlpha[i] != 0 || spellAlphaInc[i] != 0;
    bad |= sizeof spellAlpha != 16 || sizeof spellAlphaInc != 16;
    bad |= texLastDecompressionTime != 0 || worldTileEntityLightScale != 1.0f || waterPatch != 0;
    if (bad) { puts("FAIL"); return 1; }
    puts("render data ok");
    return 0;
}
