// Observed: RenderDustStorm__14VIEffectVolumeR8VICameraR10VIMatrix44 at 0x01149800 is 1432 bytes.
// Arguments: the object (a0), the camera (a1) and a matrix (a2). It has the same frame as
// RenderGroundFog__14VIEffectVolumeR8VICameraR10VIMatrix44 (guards on floats 92 and 96, -1 for a
// null particle array at 88, the material word at 76, the scene raster, a zeroed local matrix M
// filled by PrepareBillboardTransform__14VIEffectVolumeRC10VIMatrix44T1P10VIMatrix44 with a discarded
// hidden-return matrix destroyed at once, zeroed arrays of four float triples C and L, and a final
// _$_10VIMatrix44(&M, 2)), with these differences. Particles are 60 bytes. phase = (1.0 - float44)
// * 0.5 + 0.5 as there. For each particle p whose word p[0] is non-zero:
//   * s = sinf((p.f[32] * p.f[44]) * 3.14159274) * phase.
//   * with P = p.f[4..12] and E = p.f[48..56], k = p.f[32] * p.f[44]: T = ((E - P) * k) + P,
//     component by component (T is a local triple).
//   * the 48-byte card at sDustCard (0x011b0f58) + p.int[16] * 48 is copied into L; the x of each
//     triple is multiplied by p.f[24] and the y of each by p.f[28]; z is unchanged.
//   * M's translation row (floats 12, 13, 14) is set to T (no sine term).
//   * C[i] = Mul__C10VIMatrix44RC7VIVect3R7VIVect3(M, L[i]) for i = 0..3, then the colour
//     (1.0, 1.0, 1.0, (float40 * float92) * (float96 * s)), BeginTriFan, six UV/Vertex pairs over
//     C[0], C[1], C[2], C[3], C[2], C[1] with texture coordinates (0,0), (1,0), (1,1), (0,1), (1,1),
//     (1,0), and EndTriFan, all as in the ground-fog routine.
// It returns 0 (or -1 when there is no particle array).
// Inferred: a dust card per particle that slides from its start position P toward its end position E
// as k grows, scaled per axis and placed by the billboard matrix; s only drives the alpha here.
// Unknown: the meaning of particle words 20, 36..44 and 60, of floats 40, 44, 92 and 96 on the
// object, the card table's size, and what the matrix argument means to PrepareBillboardTransform.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
static inline unsigned char* at(unsigned address) { return norrath_host_pointer(address); }
#else
static inline unsigned char* at(unsigned address) { return reinterpret_cast<unsigned char*>(static_cast<unsigned long>(address)); }
#endif

extern unsigned _14VIEffectVolume$mScene;
extern unsigned char sDustCard[];

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
int RenderDustStorm__14VIEffectVolumeR8VICameraR10VIMatrix44(unsigned char* self, unsigned char* camera, const float* matrix) {
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
        for (int n = count; n != 0; --n, particle += 60) {
            if (*reinterpret_cast<int*>(particle) == 0)
                continue;
            float k = *reinterpret_cast<float*>(particle + 32) * *reinterpret_cast<float*>(particle + 44);
            float s = sinf(k * 3.14159274f) * phase;
            float t[3];
            t[0] = *reinterpret_cast<float*>(particle + 48);
            t[1] = *reinterpret_cast<float*>(particle + 52);
            t[2] = *reinterpret_cast<float*>(particle + 56);
            t[0] = t[0] - *reinterpret_cast<float*>(particle + 4);
            t[1] = t[1] - *reinterpret_cast<float*>(particle + 8);
            t[2] = t[2] - *reinterpret_cast<float*>(particle + 12);
            k = *reinterpret_cast<float*>(particle + 32) * *reinterpret_cast<float*>(particle + 44);
            t[0] = t[0] * k;
            t[1] = t[1] * k;
            t[2] = t[2] * k;
            t[0] = t[0] + *reinterpret_cast<float*>(particle + 4);
            t[1] = t[1] + *reinterpret_cast<float*>(particle + 8);
            t[2] = t[2] + *reinterpret_cast<float*>(particle + 12);
            const float* src = reinterpret_cast<const float*>(sDustCard + *reinterpret_cast<int*>(particle + 16) * 48);
            for (int i = 0; i < 12; ++i)
                card[i] = src[i];
            for (int c = 0; c < 4; ++c)
                card[3 * c] = card[3 * c] * *reinterpret_cast<float*>(particle + 24);
            for (int c = 0; c < 4; ++c)
                card[3 * c + 1] = card[3 * c + 1] * *reinterpret_cast<float*>(particle + 28);
            m[12] = t[0];
            m[13] = t[1];
            m[14] = t[2];
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
