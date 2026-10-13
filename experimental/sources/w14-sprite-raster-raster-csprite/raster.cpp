// Observed Raster__9VICSpriteR8VICameraR7VISceneR8VIRasterUi at 0x10b2d90,
// 260 bytes. Arguments: the sprite, camera, scene, raster and flags. A 64-byte
// 16-aligned VIMatrix44 local is zero-filled and loaded by sceVu0CopyMatrix from
// the raster's model view (GetModelView). Then, in this order:
//   - the VIHSprite raster function is called directly with all five arguments;
//     its result is kept;
//   - ProcessWeaponTrails(sprite, camera, scene, local) is called;
//   - UpdateParticleEmitters(sprite, camera, scene, raster, local) is called.
// The results of the last two are dropped. The local is destroyed (second
// argument 2) and the result of the VIHSprite call is returned.
// Unknown: what a weapon trail or particle emitter update does to the local or
// the raster, and why the VIHSprite part is shared.
extern "C" int Raster__9VIHSpriteR8VICameraR7VISceneR8VIRasterUi(
    void* self, void* camera, void* scene, void* raster, unsigned flags);
extern "C" const void* GetModelView__C8VIRaster(const void* raster);
extern "C" void sceVu0CopyMatrix(void* dest, const void* source);
extern "C" int ProcessWeaponTrails__9VICSpriteR8VICameraR7VISceneR10VIMatrix44(
    void* self, void* camera, void* scene, void* matrix);
extern "C" void UpdateParticleEmitters__9VICSpriteR8VICameraR7VISceneR8VIRasterR10VIMatrix44(
    void* self, void* camera, void* scene, void* raster, void* matrix);
extern "C" void _$_10VIMatrix44(void* self, int flags);

extern "C" int Raster__9VICSpriteR8VICameraR7VISceneR8VIRasterUi(
    void* self, void* camera, void* scene, void* raster, unsigned flags) {
    alignas(16) float matrix[16];
    for (int i = 0; i < 16; ++i) matrix[i] = 0.0f;
    sceVu0CopyMatrix(matrix, GetModelView__C8VIRaster(raster));
    const int result = Raster__9VIHSpriteR8VICameraR7VISceneR8VIRasterUi(self, camera, scene, raster, flags);
    ProcessWeaponTrails__9VICSpriteR8VICameraR7VISceneR10VIMatrix44(self, camera, scene, matrix);
    UpdateParticleEmitters__9VICSpriteR8VICameraR7VISceneR8VIRasterR10VIMatrix44(self, camera, scene, raster, matrix);
    _$_10VIMatrix44(matrix, 2);
    return result;
}
