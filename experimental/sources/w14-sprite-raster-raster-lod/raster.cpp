#include "../vslots-b-bytes/bytes.hpp"
// Observed Raster__11VILODSpriteR8VICameraR7VISceneR8VIRasterUi at 0x10762e8,
// 388 bytes. Arguments: the sprite, camera, scene, raster and flags.
// If bit 0 of the sprite word at +64 is set it returns 0 and does nothing else.
// Otherwise the squared length of the three floats at offsets 48, 52 and 56 of
// the raster's current model-view matrix (GetModelView) is summed as
// (x*x + y*y) + z*z. The word at +68 is a signed level count; level i is a pair
// at +72+8*i: a float threshold, then a sprite index word. Level 0 is chosen
// when threshold0^2 < distance^2 is false. Otherwise levels are passed one at
// a time: for each one VIScene::GetSpriteType is asked about its sprite index,
// and if the answer is exactly 1 the result of GetNextColorBuffer(scene) is
// obtained and dropped; the next level is chosen as soon as its threshold^2 is
// not below the distance^2, or when the count runs out. A level equal to the
// count draws nothing and the function returns 0; otherwise it returns the
// result of RasterSprite(scene, camera, chosen sprite index, flags). The
// threshold after the last level is read by the image and its outcome never
// matters; it is not read here. A negative count is not caught: level 0 would
// be drawn.
// Unknown: whether this is a level-of-detail switch by name, what the bit 0 flag
// is, and why a color buffer is taken for type-1 levels that are passed over.
extern "C" const void* GetModelView__C8VIRaster(const void* raster);
extern "C" int GetSpriteType__7VIScenei(void* scene, int index);
extern "C" int GetNextColorBuffer__7VIScene(void* scene);
extern "C" int RasterSprite__7VISceneR8VICameraiUi(void* scene, void* camera, int index, unsigned flags);

extern "C" int Raster__11VILODSpriteR8VICameraR7VISceneR8VIRasterUi(
    void* self, void* camera, void* scene, void* raster, unsigned flags) {
    using namespace vslots_b;
    const Address base = address(self);
    float position[3] = {0.0f, 0.0f, 0.0f};
    const Address view = address(GetModelView__C8VIRaster(raster));
    if ((read_word(base + 64u) & 1u) != 0u) return 0;
    position[0] = read_single(view + 48u);
    position[1] = read_single(view + 52u);
    position[2] = read_single(view + 56u);
    const float t0 = read_single(base + 72u);
    const float distance_squared = position[0] * position[0] + position[1] * position[1] + position[2] * position[2];
    int level = 0;
    if (t0 * t0 < distance_squared) {
        if (signed_word(read_word(base + 68u)) > 0) {
            do {
                if (GetSpriteType__7VIScenei(scene, signed_word(read_word(base + 76u + 8u * static_cast<unsigned>(level)))) == 1) {
                    GetNextColorBuffer__7VIScene(scene);
                }
                ++level;
                const float threshold = read_single(base + 72u + 8u * static_cast<unsigned>(level));
                if (!(threshold * threshold < distance_squared)) break;
            } while (level < signed_word(read_word(base + 68u)));
        }
    }
    if (level == signed_word(read_word(base + 68u))) return 0;
    return RasterSprite__7VISceneR8VICameraiUi(scene, camera, signed_word(read_word(base + 76u + 8u * static_cast<unsigned>(level))), flags);
}
