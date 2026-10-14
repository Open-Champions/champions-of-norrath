#include "../vslots-b-bytes/bytes.hpp"
// Observed Raster__13VIFloraSpriteR8VICameraR7VISceneR8VIRasterUi at 0x1006288,
// 40 bytes. Arguments: the sprite, camera, scene, raster and a flags word. The
// camera and scene are unused. It is a tail call: DrawPrimBuffer(raster, word
// at sprite+64, word at sprite+68, flags), and its result is returned.
// Unknown: what the two sprite words are (they are passed through as ints) and
// the meaning of the flags and result.
extern "C" int DrawPrimBuffer__8VIRasteriiUi(void* raster, int a, int b, unsigned flags);

extern "C" int Raster__13VIFloraSpriteR8VICameraR7VISceneR8VIRasterUi(
    void* self, void* camera, void* scene, void* raster, unsigned flags) {
    using namespace vslots_b;
    (void)camera;
    (void)scene;
    const Address base = address(self);
    return DrawPrimBuffer__8VIRasteriiUi(raster, signed_word(read_word(base + 64u)),
                                         signed_word(read_word(base + 68u)), flags);
}
