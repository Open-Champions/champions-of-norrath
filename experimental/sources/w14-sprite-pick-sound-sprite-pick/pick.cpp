// Observed: Pick__13VISoundSpriteR13VIPickSegmentR13VISceneFilterR7VISceneR8VIRaster at 0x0117bf10, 708 bytes. Arguments: the sprite (not read), the pick segment (G), the scene
// filter (F), the scene (V) and the raster (R). If GetSoundUISprite__7VIScene(V) returns -1 the result is 0. Otherwise a zeroed 16-float
// matrix M is filled by sceVu0CopyMatrix from GetModelView__C8VIRaster(R). For each of the three rows (floats 0-2, 4-6 and 8-10 of M) it
// computes 1/sqrt of the sum of squares (the rsqrt.s form); then each row whose value is below 0.25 has its three floats multiplied by
// that value and then by 4.0 (so a row longer than 4 is brought to length 4). Floats 3, 7 and 11 are not touched. It calls
// SetModelView__8VIRasterRC10VIMatrix44(R, M), calls
// PickSprite__7VISceneR13VIPickSegmentR13VISceneFilteri(V, G, F, X) with X the second result of GetSoundUISprite__7VIScene(V), destroys M with
// _$_10VIMatrix44(M, 2) and returns the pick result. Same shape as the point-light, point-sprite and stream-audio picks, with the
// scale limit written inline rather than through a ClampScale call.
// Inferred: the row limit caps the scale of the temporary model-view; the clamp value 4.0 is the observed constant. Nothing restores
// the previous model-view matrix afterwards. Unknown: the names of the UI sprite selector and the constants' meaning.
extern "C" {
struct VIMatrixLocal {
    float m[16];
};
int GetSoundUISprite__7VIScene(unsigned char* scene);
unsigned char* GetModelView__C8VIRaster(unsigned char* raster);
void sceVu0CopyMatrix(void* destination, const void* source);
void SetModelView__8VIRasterRC10VIMatrix44(unsigned char* raster, VIMatrixLocal* matrix);
int PickSprite__7VISceneR13VIPickSegmentR13VISceneFilteri(unsigned char* scene, unsigned char* segment, unsigned char* filter, int ui);
void _$_10VIMatrix44(VIMatrixLocal* matrix, int flags);

int Pick__13VISoundSpriteR13VIPickSegmentR13VISceneFilterR7VISceneR8VIRaster(unsigned char*, unsigned char* segment, unsigned char* filter, unsigned char* scene, unsigned char* raster) {
    if (GetSoundUISprite__7VIScene(scene) == -1) return 0;
    alignas(16) VIMatrixLocal local;
    for (int index = 0; index < 16; ++index) local.m[index] = 0.0f;
    VIMatrixLocal* matrix = &local;
    float inverse[3] = {0.0f, 0.0f, 0.0f};
    sceVu0CopyMatrix(matrix, GetModelView__C8VIRaster(raster));
    for (int row = 0; row < 3; ++row) {
        const float* r = matrix->m + 4 * row;
        inverse[row] = 1.0f / __builtin_sqrtf(r[0] * r[0] + r[1] * r[1] + r[2] * r[2]);
    }
    for (int row = 0; row < 3; ++row) {
        if (inverse[row] < 0.25f) {
            float* r = matrix->m + 4 * row;
            r[0] = r[0] * inverse[row] * 4.0f;
            r[1] = r[1] * inverse[row] * 4.0f;
            r[2] = r[2] * inverse[row] * 4.0f;
        }
    }
    SetModelView__8VIRasterRC10VIMatrix44(raster, matrix);
    int result = PickSprite__7VISceneR13VIPickSegmentR13VISceneFilteri(scene, segment, filter, GetSoundUISprite__7VIScene(scene));
    _$_10VIMatrix44(matrix, 2);
    return result;
}
}
