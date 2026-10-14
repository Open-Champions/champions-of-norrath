extern "C" {
char defaultHalos[16];
const void* halos;
float modelShadowScale;
float shadowBrightness;
float globalShadowBrightness;
unsigned char modelDrawFade;
int nmHalos;
int modelOption;
void modelResetOptionalParams__Fv();
}

int main() {
    halos = nullptr;
    modelShadowScale = 7.0f;
    shadowBrightness = 9.0f;
    globalShadowBrightness = 0.75f;
    modelDrawFade = 1;
    nmHalos = 5;
    modelOption = 6;
    modelResetOptionalParams__Fv();
    if (halos != defaultHalos) return 1;
    if (modelShadowScale != 1.0f) return 2;
    if (shadowBrightness != 0.75f) return 3;
    if (modelDrawFade != 0) return 4;
    if (nmHalos != 0) return 5;
    if (modelOption != 0) return 6;
    if (globalShadowBrightness != 0.75f) return 7;
    return 0;
}
