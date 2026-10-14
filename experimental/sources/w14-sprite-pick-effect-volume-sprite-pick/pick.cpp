// Observed: Pick__20VIEffectVolumeSpriteR13VIPickSegmentR13VISceneFilterR7VISceneR8VIRaster at 0x0114e780, 248 bytes. Arguments: the sprite (S), the pick segment (G), the scene filter (F), the
// scene (V) and the raster (R). If GetStreamAudioUISprite__7VIScene(V) returns -1 the result is 0. Otherwise a zeroed
// 16-float matrix M is made, filled by sceVu0CopyMatrix from GetModelView__C8VIRaster(R). Does not limit the scale of M; calls
// SetModelView__8VIRasterRC10VIMatrix44(R, M), calls PickSprite__7VISceneR13VIPickSegmentR13VISceneFilteri(V, G, F, X) with X
// the second result of GetStreamAudioUISprite__7VIScene(V), destroys M with _$_10VIMatrix44(M, 2) and returns the pick result.
// Inferred: M temporarily replaces the raster's model-view matrix while the sprite is picked, and the UI sprite index selects what is drawn
// for picking. Observed: nothing restores the previous model-view matrix afterwards.
// Unknown: the names of the UI sprite selector and of what ClampScale limits.
extern "C" {
struct VIMatrixLocal {
    float m[16];
};
int GetStreamAudioUISprite__7VIScene(unsigned char* scene);
unsigned char* GetModelView__C8VIRaster(unsigned char* raster);
void sceVu0CopyMatrix(void* destination, const void* source);
void SetModelView__8VIRasterRC10VIMatrix44(unsigned char* raster, VIMatrixLocal* matrix);
int PickSprite__7VISceneR13VIPickSegmentR13VISceneFilteri(unsigned char* scene, unsigned char* segment, unsigned char* filter, int ui);
void _$_10VIMatrix44(VIMatrixLocal* matrix, int flags);

int Pick__20VIEffectVolumeSpriteR13VIPickSegmentR13VISceneFilterR7VISceneR8VIRaster(unsigned char*, unsigned char* segment, unsigned char* filter, unsigned char* scene, unsigned char* raster) {
    if (GetStreamAudioUISprite__7VIScene(scene) == -1) return 0;
    alignas(16) VIMatrixLocal local;
    for (int index = 0; index < 16; ++index) local.m[index] = 0.0f;
    VIMatrixLocal* matrix = &local;
    sceVu0CopyMatrix(matrix, GetModelView__C8VIRaster(raster));
    SetModelView__8VIRasterRC10VIMatrix44(raster, matrix);
    int result = PickSprite__7VISceneR13VIPickSegmentR13VISceneFilteri(scene, segment, filter, GetStreamAudioUISprite__7VIScene(scene));
    _$_10VIMatrix44(matrix, 2);
    return result;
}
}
