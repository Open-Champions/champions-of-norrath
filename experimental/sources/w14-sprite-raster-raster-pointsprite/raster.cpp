// Observed Raster__13VIPointSpriteR8VICameraR7VISceneR8VIRasterUi at 0x10cb890, 256 bytes.
// Arguments: the sprite, camera, scene, raster and flags. The scene is asked
// for GetPointUISprite__7VIScene; if that is -1 the function returns 0 with nothing else done.
// Otherwise a 64-byte 16-aligned matrix local is zero-filled, then loaded by
// sceVu0CopyMatrix from the raster's current model-view matrix (GetModelView),
// ClampScale of this sprite is called on the local, the local is installed with
// SetModelView, the scene is asked for GetPointUISprite__7VIScene a second time, and
// RasterUISprite(scene, camera, that second answer, flags) is called. Last, the
// VIMatrix44 destructor runs on the local with its second argument 2, and 0 is
// returned. RasterUISprite's result is dropped. The original model-view matrix
// is not put back by this function.
// Unknown: the meaning of the UI sprite index and what ClampScale does beyond
// taking the local matrix.
extern "C" int GetPointUISprite__7VIScene(void* scene);
extern "C" const void* GetModelView__C8VIRaster(const void* raster);
extern "C" void sceVu0CopyMatrix(void* dest, const void* source);
extern "C" void ClampScale__13VIPointSpriteR10VIMatrix44(void* self, void* matrix);
extern "C" void SetModelView__8VIRasterRC10VIMatrix44(void* raster, const void* matrix);
extern "C" int RasterUISprite__7VISceneR8VICameraiUi(void* scene, void* camera, int index, unsigned flags);
extern "C" void _$_10VIMatrix44(void* self, int flags);

extern "C" int Raster__13VIPointSpriteR8VICameraR7VISceneR8VIRasterUi(
    void* self, void* camera, void* scene, void* raster, unsigned flags) {
    if (GetPointUISprite__7VIScene(scene) != -1) {
        alignas(16) float matrix[16];
        for (int i = 0; i < 16; ++i) matrix[i] = 0.0f;
        sceVu0CopyMatrix(matrix, GetModelView__C8VIRaster(raster));
        ClampScale__13VIPointSpriteR10VIMatrix44(self, matrix);
        SetModelView__8VIRasterRC10VIMatrix44(raster, matrix);
        RasterUISprite__7VISceneR8VICameraiUi(scene, camera, GetPointUISprite__7VIScene(scene), flags);
        _$_10VIMatrix44(matrix, 2);
    }
    return 0;
}
