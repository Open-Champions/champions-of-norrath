#include <cstdio>

extern "C" {
float globalShadowBrightness;
float shadowBrightness;
void modelSetShadowBrightness__Ff(float scale);
}

int main() {
    globalShadowBrightness = 0.5f;
    shadowBrightness = -1.0f;
    modelSetShadowBrightness__Ff(3.0f);
    if (shadowBrightness != 1.5f) return 1;
    // The stored value is not accumulated: a second call overwrites it.
    globalShadowBrightness = 4.0f;
    modelSetShadowBrightness__Ff(0.25f);
    if (shadowBrightness != 1.0f) return 2;
    if (globalShadowBrightness != 4.0f) return 3;
    return 0;
}
