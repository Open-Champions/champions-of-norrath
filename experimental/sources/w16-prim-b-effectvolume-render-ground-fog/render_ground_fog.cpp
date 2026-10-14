// Observed: RenderGroundFog__14VIEffectVolumeR8VICameraR10VIMatrix44 at 0x01148f30 is 1260 bytes.
// Arguments: the object (a0), the camera (a1) and a matrix (a2). Offsets are bytes into the object.
//   1. Returns 0 when the float at 92 or the float at 96 equals 0.0. Reads the particle pointer at
//      88 and the raster (the pointer at scene + 19356, scene being the word
//      _14VIEffectVolume$mScene at 0x011b1a98); returns -1 when the particle pointer is null.
//      count = the int at 84; the material word at 76 selects SetSolidFillUIAVGMaterial__8VIRaster
//      (when -1) or SetMaterial__8VIRasteri(raster, word).
//   2. A local matrix M is zero filled (four-float stores, then a VU0 quadword zero as for any
//      VIMatrix44 constructor), then PrepareBillboardTransform__14VIEffectVolumeRC10VIMatrix44T1P10VIMatrix44
//      runs with a hidden return matrix (destroyed straight away with _$_10VIMatrix44(r, 2)), the
//      object, the 64 bytes at camera + 176, the matrix argument and &M. Two local arrays of four
//      float triples (corner positions C and card corners L) are zeroed.
//   3. phase = (1.0 - float44) * 0.5 + 0.5. If count <= 0 it skips to step 5. For each of the
//      count 36-byte particles p (advancing by 36), skipping those whose word p[0] is zero:
//      * s = sinf((p.f[28] / p.f[32]) * 3.14159274) * phase.
//      * the 48-byte card at sFogCard (0x011b0dd8) + p.int[16] * 48 is copied into L (four triples);
//        the x of each triple is multiplied by p.f[20] and the y of each by p.f[24]; z is unchanged.
//      * M's translation row (floats 12, 13, 14) is set to (p.f[4], p.f[8] + s * 1.5, p.f[12]).
//      * C[i] = Mul__C10VIMatrix44RC7VIVect3R7VIVect3(M, L[i]) for i = 0..3.
//      * colour (1.0, 1.0, 1.0, (float40 * float92) * (float96 * s)) via Color__8VIRasterRC10VIColor32F,
//        BeginTriFan__8VIRaster, then UV and Vertex pairs (UV__8VIRasterRC7VIVect2,
//        Vertex__8VIRasterRC7VIVect3) over C[0], C[1], C[2], C[3], C[2], C[1] with texture coordinates
//        (0,0), (1,0), (1,1), (0,1), (1,1), (1,0), then EndTriFan__8VIRaster.
//   4. (loop end)
//   5. _$_10VIMatrix44(&M, 2), then return 0.
// Inferred: a fog card per particle, scaled by two per-particle factors, lifted by a sine bounce and
// placed by the billboard matrix, faded by the object's float96 and float40; the return value 0 is
// ok, -1 means no particle array. Unknown: what the matrix argument and the camera offset 176
// mean beyond their use as inputs to PrepareBillboardTransform, the card table's size, the meaning
// of floats 40, 44, 92 and 96, and the argument roles of the hidden-return call (read from its
// mangled name and the register use).
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
static inline unsigned char* at(unsigned address) { return norrath_host_pointer(address); }
#else
static inline unsigned char* at(unsigned address) { return reinterpret_cast<unsigned char*>(static_cast<unsigned long>(address)); }
#endif

extern unsigned _14VIEffectVolume$mScene;
extern unsigned char sFogCard[];

extern "C" {
float sinf(float x);
void SetMaterial__8VIRasteri(void* raster, int material);
void SetSolidFillUIAVGMaterial__8VIRaster(void* raster);
void Color__8VIRasterRC10VIColor32F(void* raster, const float* color);
void BeginTriFan__8VIRaster(void* raster);
void UV__8VIRasterRC7VIVect2(void* raster, const float* uv);
void Vertex__8VIRasterRC7VIVect3(void* raster, const float* vertex);
void EndTriFan__8VIRaster(void* raster);
void PrepareBillboardTransform__14VIEffectVolumeRC10VIMatrix44T1P10VIMatrix44(float* result, void* self, const float* cameraMatrix,
                                                                             const float* matrix, float* out);
void Mul__C10VIMatrix44RC7VIVect3R7VIVect3(const float* matrix, const float* in, float* out);
void _$_10VIMatrix44(float* matrix, int flags);
}

namespace {
inline void zeroMatrix(float* m) {
    for (int i = 0; i < 4; ++i) {
        m[4 * i] = 0.0f;
        m[4 * i + 1] = 0.0f;
        m[4 * i + 2] = 0.0f;
        m[4 * i + 3] = 0.0f;
    }
#if defined(__mips__)
    __asm__ volatile(
        "vsub.xyzw $vf4,$vf0,$vf0\n\t"
        "sqc2 $vf4,48(%0)\n\t"
        "sqc2 $vf4,32(%0)\n\t"
        "sqc2 $vf4,16(%0)\n\t"
        "sqc2 $vf4,0(%0)"
        :
        : "r"(m)
        : "memory");
#else
    for (int i = 0; i < 16; ++i)
        m[i] = 0.0f;
#endif
}
}

extern "C" {
int RenderGroundFog__14VIEffectVolumeR8VICameraR10VIMatrix44(unsigned char* self, unsigned char* camera, const float* matrix) {
    if (*reinterpret_cast<float*>(self + 92) == 0.0f)
        return 0;
    if (*reinterpret_cast<float*>(self + 96) == 0.0f)
        return 0;
    unsigned char* particle = at(*reinterpret_cast<unsigned*>(self + 88));
    unsigned scene = _14VIEffectVolume$mScene;
    if (*reinterpret_cast<unsigned*>(self + 88) == 0)
        return -1;
    unsigned char* raster = at(*reinterpret_cast<unsigned*>(at(scene) + 19356));
    int material = *reinterpret_cast<int*>(self + 76);
    int count = *reinterpret_cast<int*>(self + 84);
    if (material != -1)
        SetMaterial__8VIRasteri(raster, material);
    else
        SetSolidFillUIAVGMaterial__8VIRaster(raster);
    alignas(16) float m[16];
    alignas(16) float discarded[16];
    zeroMatrix(m);
    PrepareBillboardTransform__14VIEffectVolumeRC10VIMatrix44T1P10VIMatrix44(discarded, self, reinterpret_cast<const float*>(camera + 176), matrix, m);
    _$_10VIMatrix44(discarded, 2);
    float corner[12];
    float card[12];
    for (int i = 0; i < 12; ++i)
        corner[i] = 0.0f;
    for (int i = 0; i < 12; ++i)
        card[i] = 0.0f;
    float phase = (1.0f - *reinterpret_cast<float*>(self + 44)) * 0.5f + 0.5f;
    if (count > 0) {
        for (int n = count; n != 0; --n, particle += 36) {
            if (*reinterpret_cast<int*>(particle) == 0)
                continue;
            float s = sinf(*reinterpret_cast<float*>(particle + 28) / *reinterpret_cast<float*>(particle + 32) * 3.14159274f) * phase;
            const float* src = reinterpret_cast<const float*>(sFogCard + *reinterpret_cast<int*>(particle + 16) * 48);
            for (int i = 0; i < 12; ++i)
                card[i] = src[i];
            for (int c = 0; c < 4; ++c)
                card[3 * c] = card[3 * c] * *reinterpret_cast<float*>(particle + 20);
            for (int c = 0; c < 4; ++c)
                card[3 * c + 1] = card[3 * c + 1] * *reinterpret_cast<float*>(particle + 24);
            m[12] = *reinterpret_cast<float*>(particle + 4);
            m[13] = *reinterpret_cast<float*>(particle + 8) + s * 1.5f;
            m[14] = *reinterpret_cast<float*>(particle + 12);
            for (int c = 0; c < 4; ++c)
                Mul__C10VIMatrix44RC7VIVect3R7VIVect3(m, card + 3 * c, corner + 3 * c);
            float color[4];
            color[0] = 1.0f;
            color[1] = 1.0f;
            color[2] = 1.0f;
            color[3] = *reinterpret_cast<float*>(self + 40) * *reinterpret_cast<float*>(self + 92) * (*reinterpret_cast<float*>(self + 96) * s);
            Color__8VIRasterRC10VIColor32F(raster, color);
            BeginTriFan__8VIRaster(raster);
            static const int order[6] = {0, 1, 2, 3, 2, 1};
            static const float uv[6][2] = {{0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}, {1.0f, 1.0f}, {1.0f, 0.0f}};
            for (int i = 0; i < 6; ++i) {
                float coordinate[2] = {uv[i][0], uv[i][1]};
                UV__8VIRasterRC7VIVect2(raster, coordinate);
                Vertex__8VIRasterRC7VIVect3(raster, corner + 3 * order[i]);
            }
            EndTriFan__8VIRaster(raster);
        }
    }
    _$_10VIMatrix44(m, 2);
    return 0;
}
}
