#include "../vslots-b-bytes/bytes.hpp"
// Observed Raster__12VISkinSpriteR8VICameraR7VISceneR8VIRasterUi at 0x116d4f0,
// 72 bytes. Camera and scene are unused. When bit 0 of the sprite word at +64
// is set the function returns 0 without drawing. Otherwise it tail-calls
// DrawPrimBuffer(raster, word at +68, word at +72, flags) and returns its result.
// Unknown: what bit 0 of the word at +64 means, and the two other words.
extern "C" int DrawPrimBuffer__8VIRasteriiUi(void* raster, int a, int b, unsigned flags);

extern "C" int Raster__12VISkinSpriteR8VICameraR7VISceneR8VIRasterUi(
    void* self, void* camera, void* scene, void* raster, unsigned flags) {
    using namespace vslots_b;
    (void)camera;
    (void)scene;
    const Address base = address(self);
    if ((read_word(base + 64u) & 1u) != 0u) return 0;
    return DrawPrimBuffer__8VIRasteriiUi(raster, signed_word(read_word(base + 68u)),
                                         signed_word(read_word(base + 72u)), flags);
}
