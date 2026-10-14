// Observed: Pick__13VIPointSpriteR13VIPickSegmentR13VISceneFilterR7VISceneR8VIRaster at 0x010cb9a0, 268 bytes. Arguments: the sprite (S), the pick segment (G), the scene filter (F), the
// scene (V) and the raster (R). If GetPointUISprite__7VIScene(V) returns -1 the result is 0. Otherwise a zeroed
// 16-float matrix M is made, filled by sceVu0CopyMatrix from GetModelView__C8VIRaster(R). Calls ClampScale__13VIPointSpriteR10VIMatrix44(S, M) so the sprite can limit the scale of M, then calls
// SetModelView__8VIRasterRC10VIMatrix44(R, M), calls PickSprite__7VISceneR13VIPickSegmentR13VISceneFilteri(V, G, F, X) with X
// the second result of GetPointUISprite__7VIScene(V), destroys M with _$_10VIMatrix44(M, 2) and returns the pick result.
// Inferred: M temporarily replaces the raster's model-view matrix while the sprite is picked, and the UI sprite index selects what is drawn
// for picking. Observed: nothing restores the previous model-view matrix afterwards.
// Unknown: the names of the UI sprite selector and of what ClampScale limits.
extern "C" {
struct VIMatrixLocal {
    float m[16];
};
int GetPointUISprite__7VIScene(unsigned char* scene);
unsigned char* GetModelView__C8VIRaster(unsigned char* raster);
void sceVu0CopyMatrix(void* destination, const void* source);
void ClampScale__13VIPointSpriteR10VIMatrix44(unsigned char* self, VIMatrixLocal* matrix);
void SetModelView__8VIRasterRC10VIMatrix44(unsigned char* raster, VIMatrixLocal* matrix);
int PickSprite__7VISceneR13VIPickSegmentR13VISceneFilteri(unsigned char* scene, unsigned char* segment, unsigned char* filter, int ui);
void _$_10VIMatrix44(VIMatrixLocal* matrix, int flags);

int Pick__13VIPointSpriteR13VIPickSegmentR13VISceneFilterR7VISceneR8VIRaster(unsigned char* self, unsigned char* segment, unsigned char* filter, unsigned char* scene, unsigned char* raster) {
    if (GetPointUISprite__7VIScene(scene) == -1) return 0;
    alignas(16) VIMatrixLocal local;
    for (int index = 0; index < 16; ++index) local.m[index] = 0.0f;
    VIMatrixLocal* matrix = &local;
    sceVu0CopyMatrix(matrix, GetModelView__C8VIRaster(raster));
    ClampScale__13VIPointSpriteR10VIMatrix44(self, matrix);
    SetModelView__8VIRasterRC10VIMatrix44(raster, matrix);
    int result = PickSprite__7VISceneR13VIPickSegmentR13VISceneFilteri(scene, segment, filter, GetPointUISprite__7VIScene(scene));
    _$_10VIMatrix44(matrix, 2);
    return result;
}
}
