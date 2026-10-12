// Host contract for the core loop data definitions. Not an EE plan input.
extern "C" unsigned char layer0Dir[24580] = {};
#include "core_loop_data.cpp"

static_assert(sizeof drawBustTexName == 32, "drawBustTexName");
static_assert(sizeof dummyTex == 64, "dummyTex");
static_assert(sizeof drawCompassPoint == 16, "drawCompassPoint");
static_assert(sizeof drawBustTitle == 64 && sizeof drawCompassTitle == 64, "titles");
static_assert(sizeof(float) == 4 && sizeof(int) == 4, "word size");

int main() {
    if (currentLayerDir != layer0Dir) return 1;
    if (currentRunningObject != nullptr) return 2;
    int words = disableVolumetricLighting | curFocus | endInit | coreSwitchScene | coreSwitchSceneStart |
                coreLevelCommentTimer | coreLoadingStart | drawBustCur | drawBustMax | delayCount | curPlaySeconds;
    if (words != 0) return 3;
    if (drawBustOn != 0 || drawCompassOn != 0 || correction != 0.0f) return 4;
    for (unsigned char b : drawBustTexName) if (b) return 5;
    for (unsigned char b : dummyTex) if (b) return 6;
    for (unsigned char b : drawCompassPoint) if (b) return 7;
    for (unsigned char b : drawBustTitle) if (b) return 8;
    for (unsigned char b : drawCompassTitle) if (b) return 9;
    return 0;
}
