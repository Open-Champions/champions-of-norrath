// Host contract for the light and halo data objects authored under w10-render-b-data-*. Not an EE
// plan input; it includes those sources unchanged so that they have a committed check. Expected
// values and extents are the ones written in those sources' notes: haloColor (64 bytes, two preset
// rows), halos (a pointer to defaultHalos), globalShadowBrightness (0.25f), lightInc and
// lightIntensity (16 bytes, zero), lightStart (36 bytes, zero), lightInfo (384 bytes, zero),
// lockAlpha (4 bytes, zero), invHeader and marble (pointer words, zero).
// The compiled sizes and contents were also compared with the pinned image in a private run:
// the haloColor and globalShadowBrightness bytes and the halos relocation target match; the zero
// objects are zero-filled bss here but sit in .data in the image.
#include <cstdio>
#include <cstring>
extern "C" {
char defaultHalos[16];
}
#include "../w10-render-b-data-halo-color/data_halo_color.cpp"
#include "../w10-render-b-data-model-globals/data_model_globals.cpp"
#include "../w10-render-b-data-light-slots/data_light_slots.cpp"
#include "../w10-render-b-data-light-info/data_light_info.cpp"
#include "../w10-render-b-data-lock-alpha/data_lock_alpha.cpp"
#include "../w10-render-b-data-inventory-textures/data_inventory_textures.cpp"
static_assert(sizeof(haloColor) == 64u, "haloColor extent");
static_assert(sizeof(lightInc) == 16u && sizeof(lightIntensity) == 16u, "lightInc and lightIntensity extents");
static_assert(sizeof(lightStart) == 36u, "lightStart extent");
static_assert(sizeof(lightInfo) == 384u, "lightInfo extent");
static_assert(sizeof(lockAlpha) == 4u, "lockAlpha extent");
int main() {
    int failures = 0;
    auto expect = [&](bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } };
    const int want[16] = {255, 200, 100, 0, 255, 200, 100, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    expect(std::memcmp(haloColor, want, sizeof want) == 0, "haloColor words");
    expect(halos == defaultHalos, "halos points at defaultHalos");
    unsigned bits;
    std::memcpy(&bits, &globalShadowBrightness, 4);
    expect(bits == 0x3e800000u, "globalShadowBrightness is 0.25f");
    for (float v : lightInc) expect(v == 0.0f, "lightInc zero");
    for (int v : lightIntensity) expect(v == 0, "lightIntensity zero");
    for (int v : lightStart) expect(v == 0, "lightStart zero");
    for (float v : lightInfo) expect(v == 0.0f, "lightInfo zero");
    expect(lockAlpha == 0 && invHeader == nullptr && marble == nullptr, "single words zero");
    if (failures) return 1;
    std::printf("light and halo data contract passed\n");
    return 0;
}
