#include "../vslots-b-bytes/bytes.hpp"
// Observed Raster__14VISimpleSpriteR8VICameraR7VISceneR8VIRasterUi at 0x1166ff0,
// 132 bytes. The camera is unused. VIScene::GetNextColorBuffer is called with
// the scene first. If it returns a non-negative value c, the five-argument
// DrawPrimBuffer(raster, word at sprite+64, c, word at sprite+72, flags) is
// tail-called; if it returns a negative value the four-argument
// DrawPrimBuffer(raster, word at +64, word at +72, flags) is tail-called
// instead. The callee's result is returned.
// Unknown: what the color buffer index selects, and what the two sprite words
// and the flags mean.
extern "C" int GetNextColorBuffer__7VIScene(void* scene);
extern "C" int DrawPrimBuffer__8VIRasteriiUi(void* raster, int a, int b, unsigned flags);
extern "C" int DrawPrimBuffer__8VIRasteriiiUi(void* raster, int a, int b, int c, unsigned flags);

extern "C" int Raster__14VISimpleSpriteR8VICameraR7VISceneR8VIRasterUi(
    void* self, void* camera, void* scene, void* raster, unsigned flags) {
    using namespace vslots_b;
    (void)camera;
    const Address base = address(self);
    const int color = GetNextColorBuffer__7VIScene(scene);
    if (color >= 0) {
        return DrawPrimBuffer__8VIRasteriiiUi(raster, signed_word(read_word(base + 64u)), color,
                                              signed_word(read_word(base + 72u)), flags);
    }
    return DrawPrimBuffer__8VIRasteriiUi(raster, signed_word(read_word(base + 64u)),
                                         signed_word(read_word(base + 72u)), flags);
}
