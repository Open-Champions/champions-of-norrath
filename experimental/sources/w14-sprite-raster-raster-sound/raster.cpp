// Observed Raster__13VISoundSpriteR8VICameraR7VISceneR8VIRasterUi at 0x117bc58,
// 692 bytes. Arguments: the sprite, camera, scene, raster and flags; the sprite
// is unused. The scene is asked for GetSoundUISprite; -1 returns 0 with nothing
// else done. Otherwise a 64-byte 16-aligned VIMatrix44 local is zero-filled
// and loaded by sceVu0CopyMatrix from the raster's model view (GetModelView).
// For each of the first three rows (four floats each, 16 bytes apart) the
// reciprocal length s = rsqrt.s(1.0, (x*x + y*y) + z*z) is taken from that row's
// first three floats. When s < 0.25 (length above 4) the row's x, y and z are
// each replaced by (value * s) * 4.0, which brings the length to 4; the
// fourth float and the fourth row are untouched. A row of length exactly 4,
// shorter, or zero (s infinite) is left alone. The local is then installed with
// SetModelView, GetSoundUISprite is asked a second time, and
// RasterUISprite(scene, camera, that second answer, flags) is called. Last the
// VIMatrix44 destructor runs on the local with its second argument 2, and 0 is
// returned. RasterUISprite's result is dropped; the previous model view is not
// put back.
// Unknown: the purpose of limiting row length to 4, and the sound UI index.
extern "C" int GetSoundUISprite__7VIScene(void* scene);
extern "C" const void* GetModelView__C8VIRaster(const void* raster);
extern "C" void sceVu0CopyMatrix(void* dest, const void* source);
extern "C" void SetModelView__8VIRasterRC10VIMatrix44(void* raster, const void* matrix);
extern "C" int RasterUISprite__7VISceneR8VICameraiUi(void* scene, void* camera, int index, unsigned flags);
extern "C" void _$_10VIMatrix44(void* self, int flags);

namespace {
float reciprocal_sqrt(float length_squared) {
#if defined(__mips__)
    float result;
    float one = 1.0f;
    __asm__ volatile("rsqrt.s %0, %1, %2" : "=f"(result) : "f"(one), "f"(length_squared));
    return result;
#else
    return 1.0f / __builtin_sqrtf(length_squared);
#endif
}
}

extern "C" int Raster__13VISoundSpriteR8VICameraR7VISceneR8VIRasterUi(
    void* self, void* camera, void* scene, void* raster, unsigned flags) {
    (void)self;
    if (GetSoundUISprite__7VIScene(scene) == -1) return 0;
    alignas(16) float matrix[16];
    float scale[3] = {0.0f, 0.0f, 0.0f};
    for (int i = 0; i < 16; ++i) matrix[i] = 0.0f;
    sceVu0CopyMatrix(matrix, GetModelView__C8VIRaster(raster));
    for (int row = 0; row < 3; ++row) {
        float* const r = matrix + 4 * row;
        scale[row] = reciprocal_sqrt(r[0] * r[0] + r[1] * r[1] + r[2] * r[2]);
    }
    for (int row = 0; row < 3; ++row) {
        if (scale[row] < 0.25f) {
            float* const r = matrix + 4 * row;
            r[0] = r[0] * scale[row] * 4.0f;
            r[1] = r[1] * scale[row] * 4.0f;
            r[2] = r[2] * scale[row] * 4.0f;
        }
    }
    SetModelView__8VIRasterRC10VIMatrix44(raster, matrix);
    RasterUISprite__7VISceneR8VICameraiUi(scene, camera, GetSoundUISprite__7VIScene(scene), flags);
    _$_10VIMatrix44(matrix, 2);
    return 0;
}
