#include "../vslots-b-bytes/bytes.hpp"
// Observed Raster__13VIGroupSpriteR8VICameraR7VISceneR8VIRasterUi at 0x101cb18,
// 536 bytes. Arguments: the sprite, camera, scene, raster and flags. Three
// 64-byte 16-aligned VIMatrix44 locals are zero-filled: "saved", "member" and
// "product". The saved local is loaded by sceVu0CopyMatrix from the raster's
// model view (GetModelView). Then, for each of the sprite's members (count is
// the signed word at +64, read again after every member; records of 32 bytes
// from +68; a count of 0 or less runs none):
//   - Transform(member, record+4 as a vector, record+20 as a vector, float at
//     record+16) fills the member local;
//   - Mul(product, member, saved) is the product, SetModelView(raster, product)
//     installs it;
//   - RasterSprite(scene, camera, word at record+0, flags) is called, and the
//     function's result becomes 1 once any call answers non-zero.
// The three locals are destroyed (product, member, saved; flag 2 each) and the
// 0/1 result is returned. The raster's model view is left at the last product.
// Inferred: the Transform arguments are two three-float vectors and a scale,
// because the image loads the float into the first float argument register
// before the call and the callee's first use of it is to copy it aside (the
// mangled name lists two arguments only). Unknown: what the vectors mean
// (rotation and translation by shape only) and what the rest of a record holds.
extern "C" const void* GetModelView__C8VIRaster(const void* raster);
extern "C" void sceVu0CopyMatrix(void* dest, const void* source);
extern "C" void Transform__10VIMatrix44RC7VIVect3fT1(void* self, const void* first, const void* second, float scale);
extern "C" void Mul__10VIMatrix44RC10VIMatrix44T1(void* self, const void* a, const void* b);
extern "C" void SetModelView__8VIRasterRC10VIMatrix44(void* raster, const void* matrix);
extern "C" int RasterSprite__7VISceneR8VICameraiUi(void* scene, void* camera, int index, unsigned flags);
extern "C" void _$_10VIMatrix44(void* self, int flags);

extern "C" int Raster__13VIGroupSpriteR8VICameraR7VISceneR8VIRasterUi(
    void* self, void* camera, void* scene, void* raster, unsigned flags) {
    using namespace vslots_b;
    alignas(16) float saved[16];
    alignas(16) float member[16];
    alignas(16) float product[16];
    for (int i = 0; i < 16; ++i) saved[i] = 0.0f;
    for (int i = 0; i < 16; ++i) member[i] = 0.0f;
    for (int i = 0; i < 16; ++i) product[i] = 0.0f;
    const Address base = address(self);
    int result = 0;
    int index = 0;
    sceVu0CopyMatrix(saved, GetModelView__C8VIRaster(raster));
    if (signed_word(read_word(base + 64u)) > 0) {
        do {
            const Address record = base + 68u + 32u * static_cast<unsigned>(index);
            const float scale = read_single(record + 16u);
            ++index;
            Transform__10VIMatrix44RC7VIVect3fT1(member, bytes(record + 4u), bytes(record + 20u), scale);
            Mul__10VIMatrix44RC10VIMatrix44T1(product, member, saved);
            SetModelView__8VIRasterRC10VIMatrix44(raster, product);
            if (RasterSprite__7VISceneR8VICameraiUi(scene, camera, signed_word(read_word(record)), flags) != 0) result = 1;
        } while (index < signed_word(read_word(base + 64u)));
    }
    _$_10VIMatrix44(product, 2);
    _$_10VIMatrix44(member, 2);
    _$_10VIMatrix44(saved, 2);
    return result;
}
