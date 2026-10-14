// Host contract for modelSetFade__Fc. Not an EE plan input.
#include <cstdio>
#include <initializer_list>
extern "C" {
signed char modelDrawFade;
void modelSetFade__Fc(signed char);
}
int main() {
    int failures = 0;
    for (int v : {0, 1, 63, 127, -1, -128}) {
        modelDrawFade = 55;
        modelSetFade__Fc(static_cast<signed char>(v));
        if (modelDrawFade != v) { std::fprintf(stderr, "fail: %d got %d\n", v, modelDrawFade); ++failures; }
    }
    if (failures) return 1;
    std::printf("modelSetFade contract passed\n");
    return 0;
}
