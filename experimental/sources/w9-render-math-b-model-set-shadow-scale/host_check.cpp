// Host contract for modelSetShadowScale__Ff. Not an EE plan input.
#include <cstdio>
#include <cstring>
extern "C" {
float modelShadowScale;
void modelSetShadowScale__Ff(float);
}
int main() {
    int failures = 0;
    const float values[] = {0.0f, -0.0f, 1.0f, 0.375f, -2.5f, 1e30f};
    for (float v : values) {
        modelShadowScale = 99.0f;
        modelSetShadowScale__Ff(v);
        unsigned a, b; std::memcpy(&a, &modelShadowScale, 4); std::memcpy(&b, &v, 4);
        if (a != b) { std::fprintf(stderr, "fail: %g\n", v); ++failures; }
    }
    if (failures) return 1;
    std::printf("modelSetShadowScale contract passed\n");
    return 0;
}
