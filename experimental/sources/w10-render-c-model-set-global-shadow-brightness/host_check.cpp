// Host contract for modelSetGlobalShadowBrightness__Ff. Not an EE plan input.
#include <cstdio>
extern "C" float shadowBrightness;
extern "C" float globalShadowBrightness;
float shadowBrightness, globalShadowBrightness;
extern "C" void modelSetGlobalShadowBrightness__Ff(float);
int main() {
    modelSetGlobalShadowBrightness__Ff(2.0f);
    if (shadowBrightness != 0.5f || globalShadowBrightness != 0.5f) return 1;
    modelSetGlobalShadowBrightness__Ff(-6.0f);
    if (shadowBrightness != -1.5f || globalShadowBrightness != -1.5f) return 2;
    std::printf("modelSetGlobalShadowBrightness contract passed\n");
    return 0;
}
