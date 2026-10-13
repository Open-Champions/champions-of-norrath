// Host contract for lightDisableVolumetricLighting__Fv. Not an EE plan input.
#include <cstdio>
#include <initializer_list>
extern "C" {
int disableVolumetricLighting;
void lightDisableVolumetricLighting__Fv();
}
int main() {
    int failures = 0;
    for (int v : {0, 1, 2, -1}) {
        disableVolumetricLighting = v;
        lightDisableVolumetricLighting__Fv();
        if (disableVolumetricLighting != 1) { std::fprintf(stderr, "fail: from %d got %d\n", v, disableVolumetricLighting); ++failures; }
    }
    if (failures) return 1;
    std::printf("lightDisableVolumetricLighting contract passed\n");
    return 0;
}
