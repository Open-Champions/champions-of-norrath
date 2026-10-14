#include "../vslots-b-bytes/bytes.hpp"
// Observed Raster__16VIParticleSpriteR8VICameraR7VISceneR8VIRasterUi at
// 0x10a9b98, 1656 bytes. Arguments: the sprite, camera, scene, raster and
// flags. It always returns 0. Sprite fields used: a VIParticleEmitter embedded
// at +80 and a word at +1072 (mode, tested against zero). Camera fields used:
// three floats at +28 ("camera point") and a matrix at +48.
// The camera point is copied first. Three zero-filled 64-byte locals follow:
// "a", "b", "c".
//  1. a = the matrix at camera+48 (sceVu0CopyMatrix); b = Transpose(a); b's
//     fourth row is replaced by the formula
//       b[12+j] = -((b[3]*b[j] + b[7]*b[4+j]) + b[11]*b[8+j]) for j = 0..2
//     (float indices), then b[3], b[7], b[11] are zeroed. a = the raster's model
//     view; c = Mul(a, b). The "location" is the float triple c[12], c[13], c[14].
//  2. Mode word zero: emitter.SetLocation(location), then SetDirection with a
//     VIMatrix33 built by its constructor and then filled with c[0], c[1], c[2],
//     c[4], c[5], c[6], c[8], c[9], c[10] in that order, then the VIMatrix33
//     destructor with flag 2. Mode word non-zero: SetLocation((0,0,0)), then
//     SetDirection((0,0,1)) as a VIVect3, then SetSpace(c). The mode word is
//     read twice.
//  3. The camera point minus the location is made; when its Magnitude is above
//     100.0 the emitter is Purge()d, otherwise Update()d.
//  4. When GetParticleUISprite(scene) is not -1, a fourth local "d" is zero-filled
//     and loaded from the raster's model view; each of its first three rows is
//     limited to length 4 exactly as the sound sprite does (reciprocal length
//     r = rsqrt.s(1.0, (x*x+y*y)+z*z); rows with r < 0.25 get x, y, z replaced by
//     (value * r) * 4.0); d is installed with SetModelView, the scene is asked a
//     second time, RasterUISprite(scene, camera, that answer, flags) is called
//     and d is destroyed.
//  5. c, b, a are destroyed in that order (flag 2 each).
// The camera point is read before anything else is called; the emitter calls
// see the location from step 1. Unknown: the meaning of the mode word and of the
// emitter's space/direction/location, and the camera field names.
extern "C" const void* GetModelView__C8VIRaster(const void* raster);
extern "C" void sceVu0CopyMatrix(void* dest, const void* source);
extern "C" void Transpose__10VIMatrix44RC10VIMatrix44(void* self, const void* source);
extern "C" void Mul__10VIMatrix44RC10VIMatrix44T1(void* self, const void* a, const void* b);
extern "C" void SetLocation__17VIParticleEmitterRC7VIVect3(void* emitter, const void* point);
extern "C" void SetDirection__17VIParticleEmitterRC10VIMatrix33(void* emitter, const void* matrix);
extern "C" void SetDirection__17VIParticleEmitterRC7VIVect3(void* emitter, const void* vector);
extern "C" void SetSpace__17VIParticleEmitterRC10VIMatrix44(void* emitter, const void* matrix);
extern "C" void __10VIMatrix33(void* self);
extern "C" void _$_10VIMatrix33(void* self, int flags);
extern "C" float Magnitude__C7VIVect3(const void* vector);
extern "C" void Purge__17VIParticleEmitter(void* emitter);
extern "C" void Update__17VIParticleEmitter(void* emitter);
extern "C" int GetParticleUISprite__7VIScene(void* scene);
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

extern "C" int Raster__16VIParticleSpriteR8VICameraR7VISceneR8VIRasterUi(
    void* self, void* camera, void* scene, void* raster, unsigned flags) {
    using namespace vslots_b;
    const Address base = address(self);
    const Address cam = address(camera);
    float camera_point[3];
    float location[3] = {0.0f, 0.0f, 0.0f};
    alignas(16) float a[16];
    alignas(16) float b[16];
    alignas(16) float c[16];
    camera_point[0] = read_single(cam + 28u);
    camera_point[1] = read_single(cam + 32u);
    camera_point[2] = read_single(cam + 36u);
    for (int i = 0; i < 16; ++i) a[i] = 0.0f;
    for (int i = 0; i < 16; ++i) b[i] = 0.0f;
    for (int i = 0; i < 16; ++i) c[i] = 0.0f;
    void* const emitter = bytes(base + 80u);
    sceVu0CopyMatrix(a, bytes(cam + 48u));
    Transpose__10VIMatrix44RC10VIMatrix44(b, a);
    replace_fourth_row(b);
    sceVu0CopyMatrix(a, GetModelView__C8VIRaster(raster));
    Mul__10VIMatrix44RC10VIMatrix44T1(c, a, b);
    location[0] = c[12];
    location[1] = c[13];
    location[2] = c[14];
    if (read_word(base + 1072u) == 0u) {
        SetLocation__17VIParticleEmitterRC7VIVect3(emitter, location);
    } else {
        float zero[3] = {0.0f, 0.0f, 0.0f};
        SetLocation__17VIParticleEmitterRC7VIVect3(emitter, zero);
    }
    if (read_word(base + 1072u) != 0u) {
        float direction[3] = {0.0f, 0.0f, 1.0f};
        SetDirection__17VIParticleEmitterRC7VIVect3(emitter, direction);
        SetSpace__17VIParticleEmitterRC10VIMatrix44(emitter, c);
    } else {
        float rotation[9];
        __10VIMatrix33(rotation);
        rotation[0] = c[0];
        rotation[1] = c[1];
        rotation[2] = c[2];
        rotation[3] = c[4];
        rotation[4] = c[5];
        rotation[5] = c[6];
        rotation[6] = c[8];
        rotation[7] = c[9];
        rotation[8] = c[10];
        SetDirection__17VIParticleEmitterRC10VIMatrix33(emitter, rotation);
        _$_10VIMatrix33(rotation, 2);
    }
    camera_point[0] = camera_point[0] - location[0];
    camera_point[1] = camera_point[1] - location[1];
    camera_point[2] = camera_point[2] - location[2];
    if (100.0f < Magnitude__C7VIVect3(camera_point)) {
        Purge__17VIParticleEmitter(emitter);
    } else {
        Update__17VIParticleEmitter(emitter);
    }
    if (GetParticleUISprite__7VIScene(scene) != -1) {
        alignas(16) float d[16];
        float scale[3] = {0.0f, 0.0f, 0.0f};
        for (int i = 0; i < 16; ++i) d[i] = 0.0f;
        sceVu0CopyMatrix(d, GetModelView__C8VIRaster(raster));
        for (int row = 0; row < 3; ++row) {
            float* const r = d + 4 * row;
            scale[row] = reciprocal_sqrt(r[0] * r[0] + r[1] * r[1] + r[2] * r[2]);
        }
        for (int row = 0; row < 3; ++row) {
            if (scale[row] < 0.25f) {
                float* const r = d + 4 * row;
                r[0] = r[0] * scale[row] * 4.0f;
                r[1] = r[1] * scale[row] * 4.0f;
                r[2] = r[2] * scale[row] * 4.0f;
            }
        }
        SetModelView__8VIRasterRC10VIMatrix44(raster, d);
        RasterUISprite__7VISceneR8VICameraiUi(scene, camera, GetParticleUISprite__7VIScene(scene), flags);
        _$_10VIMatrix44(d, 2);
    }
    _$_10VIMatrix44(c, 2);
    _$_10VIMatrix44(b, 2);
    _$_10VIMatrix44(a, 2);
    return 0;
}
