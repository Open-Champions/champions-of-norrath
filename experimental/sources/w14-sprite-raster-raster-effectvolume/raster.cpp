#include "../vslots-b-bytes/bytes.hpp"
// Observed Raster__20VIEffectVolumeSpriteR8VICameraR7VISceneR8VIRasterUi at
// 0x114e0a8, 1280 bytes. Arguments: the sprite, camera, scene, raster and
// flags. It always returns 1. Sprite fields used: a bounding box at +8
// (VIBBox, 24 bytes: two three-float points at +8 and +20), a second copy of it
// at +64, and an embedded VIEffectVolume at +88.
//  1. Size(box at +8) and Size(box at +64) are three-float sizes. If any
//     component of the first minus the second is above 1.0 or below -1.0 the
//     six floats at +8 are copied over +64 and OnResize(volume at +88, box at
//     +64) is called. The comparisons are strict, so a difference of exactly
//     1.0 or -1.0 does nothing.
//  2. A zero-filled 64-byte matrix local "m0" is made. When
//     GetStreamAudioUISprite(scene) is not -1 the raster's model view is copied
//     into m0 (sceVu0CopyMatrix of GetModelView), installed with SetModelView,
//     the scene is asked again and RasterUISprite(scene, camera, that answer,
//     flags) is called. (The UI index comes from the stream-audio query.)
//  3. Only when bit 1 of flags is set: three more zero-filled locals m1, m2,
//     m3 are used. m1 is sceVu0CopyMatrix from the matrix at camera+48; m2 =
//     Transpose of m1, and then the fourth row of m2 is replaced: for j in
//     0..2, m2[12+j] = -((m2[3]*m2[j] + m2[7]*m2[4+j]) + m2[11]*m2[8+j]) in
//     float indices, after which m2[3], m2[7] and m2[11] are zeroed. m1 is then
//     replaced by the raster's model view; m3 = Mul(m1, m2); m2 = Transpose of
//     m3; the same fourth-row replacement and zeroing is applied to m2 again.
//     Process(volume at +88, 0.0333333 as the float 0x3d088888) and
//     Render(volume at +88, camera, m2) follow.
//  4. The locals are destroyed (flag 2 each): m3, m2, m1 if used, then m0.
// The model view set in step 2 is not put back. Unknown: the meaning of the
// two boxes beyond their sizes, what the fourth-row replacement computes
// (inverse of a rigid transform by its shape), and why the UI sprite query is
// the stream-audio one.
extern "C" void Size__C6VIBBox(void* result, const void* box);
extern "C" void OnResize__14VIEffectVolumeRC6VIBBox(void* volume, const void* box);
extern "C" int GetStreamAudioUISprite__7VIScene(void* scene);
extern "C" const void* GetModelView__C8VIRaster(const void* raster);
extern "C" void sceVu0CopyMatrix(void* dest, const void* source);
extern "C" void SetModelView__8VIRasterRC10VIMatrix44(void* raster, const void* matrix);
extern "C" int RasterUISprite__7VISceneR8VICameraiUi(void* scene, void* camera, int index, unsigned flags);
extern "C" void Transpose__10VIMatrix44RC10VIMatrix44(void* self, const void* source);
extern "C" void Mul__10VIMatrix44RC10VIMatrix44T1(void* self, const void* a, const void* b);
extern "C" void Process__14VIEffectVolumef(void* volume, float step);
extern "C" void Render__14VIEffectVolumeR8VICameraR10VIMatrix44(void* volume, void* camera, void* matrix);
extern "C" void _$_10VIMatrix44(void* self, int flags);

namespace {
void replace_fourth_row(float* m) {
    const float a = -((m[3] * m[0] + m[7] * m[4]) + m[11] * m[8]);
    const float b = -((m[3] * m[1] + m[7] * m[5]) + m[11] * m[9]);
    const float c = -((m[3] * m[2] + m[7] * m[6]) + m[11] * m[10]);
    m[12] = a;
    m[13] = b;
    m[14] = c;
    m[11] = 0.0f;
    m[7] = 0.0f;
    m[3] = 0.0f;
}
}

extern "C" int Raster__20VIEffectVolumeSpriteR8VICameraR7VISceneR8VIRasterUi(
    void* self, void* camera, void* scene, void* raster, unsigned flags) {
    using namespace vslots_b;
    const Address base = address(self);
    float difference[3];
    float first[4];
    float second[4];
    alignas(16) float m0[16];
    Size__C6VIBBox(first, bytes(base + 8u));
    Size__C6VIBBox(second, bytes(base + 64u));
    difference[0] = first[0] - second[0];
    difference[1] = first[1] - second[1];
    difference[2] = first[2] - second[2];
    if (1.0f < difference[0] || 1.0f < difference[1] || 1.0f < difference[2] || difference[0] < -1.0f ||
        difference[1] < -1.0f || difference[2] < -1.0f) {
        write_single(base + 64u, read_single(base + 8u));
        write_single(base + 68u, read_single(base + 12u));
        write_single(base + 72u, read_single(base + 16u));
        write_single(base + 76u, read_single(base + 20u));
        write_single(base + 80u, read_single(base + 24u));
        write_single(base + 84u, read_single(base + 28u));
        OnResize__14VIEffectVolumeRC6VIBBox(bytes(base + 88u), bytes(base + 64u));
    }
    for (int i = 0; i < 16; ++i) m0[i] = 0.0f;
    if (GetStreamAudioUISprite__7VIScene(scene) != -1) {
        sceVu0CopyMatrix(m0, GetModelView__C8VIRaster(raster));
        SetModelView__8VIRasterRC10VIMatrix44(raster, m0);
        RasterUISprite__7VISceneR8VICameraiUi(scene, camera, GetStreamAudioUISprite__7VIScene(scene), flags);
    }
    if ((flags & 2u) != 0u) {
        alignas(16) float m1[16];
        alignas(16) float m2[16];
        alignas(16) float m3[16];
        for (int i = 0; i < 16; ++i) m1[i] = 0.0f;
        for (int i = 0; i < 16; ++i) m2[i] = 0.0f;
        for (int i = 0; i < 16; ++i) m3[i] = 0.0f;
        sceVu0CopyMatrix(m1, bytes(address(camera) + 48u));
        Transpose__10VIMatrix44RC10VIMatrix44(m2, m1);
        replace_fourth_row(m2);
        sceVu0CopyMatrix(m1, GetModelView__C8VIRaster(raster));
        Mul__10VIMatrix44RC10VIMatrix44T1(m3, m1, m2);
        Transpose__10VIMatrix44RC10VIMatrix44(m2, m3);
        replace_fourth_row(m2);
        Process__14VIEffectVolumef(bytes(base + 88u), __builtin_bit_cast(float, 0x3d088888u));
        Render__14VIEffectVolumeR8VICameraR10VIMatrix44(bytes(base + 88u), camera, m2);
        _$_10VIMatrix44(m3, 2);
        _$_10VIMatrix44(m2, 2);
        _$_10VIMatrix44(m1, 2);
    }
    _$_10VIMatrix44(m0, 2);
    return 1;
}
