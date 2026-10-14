// Observed: RenderLightShafts__14VIEffectVolumeR8VICameraR10VIMatrix44 at 0x011484d8 is 1992
// bytes. Arguments: the object (a0), the camera (a1) and a matrix (a2). Offsets are bytes into
// the object unless stated. Order of work:
//   1. Returns 0 when the float at 92 or the float at 96 equals 0.0. Reads the scene word
//      _14VIEffectVolume$mScene (0x011b1a98); "now" is the float at scene + 8160 and the raster is the
//      pointer at scene + 19356.
//   2. When bit 2 of the flag word at 60 is set: dir = -1.0; if now < 9.0 the float at 92 becomes 0;
//      otherwise dir = (now - 9.0) * 0.33333331 - 1.0 for now < 15.0 (else 1.0), and the float at 92
//      is set from the envelope: now < 9.0 -> 0.0 (a repeat test), < 10.0 -> now - 9.0, < 14.0 -> 1.0,
//      < 15.0 -> 15.0 - now, otherwise 0.0. A float at 92 that then equals 0.0 returns 0. Without
//      the flag bit dir is 0.0 and 92 is left alone.
//   3. A null particle pointer at 88 returns -1. count = the int at 84; the material word at 76
//      selects SetSolidFillUIAVGMaterial__8VIRaster (-1) or SetMaterial__8VIRasteri.
//   4. Locals: a triple c = the camera's floats at offsets 8, 12, 16; matrices M1 and M2 zero
//      filled (four-float stores then a VU0 quadword zero, the VIMatrix44 constructor); M2 is set
//      with Identity__10VIMatrix44; with x = c[0]: angle = atanf(c[2] / (-x)) - 1.5707962513 (bits 0x3fc90fda) when
//      0 < x, atanf(c[2] / (-x)) + 1.5707962513 otherwise; RotateY__10VIMatrix44f(M2, angle).
//      PrepareBillboardTransform__14VIEffectVolumeRC10VIMatrix44T1P10VIMatrix44 runs with a returned
//      matrix R (kept until the end), the object, M2, the matrix argument and &M1.
//   5. T = (0, 0, 0); with the flag bit set, T = Mul__C10VIMatrix44RC7VIVect3R7VIVect3(R, (dir, 0, 0)).
//      Four triples Q are zeroed; scale = (float96 * float92) * ((1.0 - float44) * 0.5 + 0.5).
//      If count <= 0 it goes to step 7.
//   6. For each 32-byte particle p (advancing by 32) with a non-zero word p[0], with w = p.f[16]
//      and h = p.f[20]: Q[0..3] = Mul(M1, (-w,0,0)), Mul(M1, (w,0,0)), Mul(M1, (w,-h,0)), Mul(M1,
//      (-w,-h,0)); then P = p.f[4..12] is added to every Q; with the flag bit set, T * h is added to
//      Q[2] and Q[3]. Colour (1.0, 1.0, 1.0, sinf((p.f[24] / p.f[28]) * 3.1415925026 (bits 0x40490fda)) * scale), then
//      Color__8VIRasterRC10VIColor32F, BeginTriFan__8VIRaster and UV__8VIRasterRC7VIVect2 /
//      Vertex__8VIRasterRC7VIVect3 pairs over Q[0], Q[1], Q[2], Q[3], Q[2], Q[1] with coordinates
//      (0,0), (1,0), (1,1), (0,1), (1,1), (1,0), then EndTriFan__8VIRaster.
//   7. _$_10VIMatrix44(R, 2), _$_10VIMatrix44(M2, 2), _$_10VIMatrix44(M1, 2); returns 0.
// Inferred: a sunbeam quad per particle hanging from a point, slanted toward T (the billboard
// matrix's image of the horizontal direction dir) when bit 2 is set; the envelope fades the
// beams in over 9..10, holds until 14, and out by 15 on the scene clock (the unit of "now" is
// unknown, hours is a guess from the numbers). Unknown: the meaning of the camera floats at 8..16,
// of floats 44, 92 and 96, particle words 24 and 28 and the rest of the 32-byte particle, and
// the argument roles of PrepareBillboardTransform's hidden return.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
static inline unsigned char* at(unsigned address) { return norrath_host_pointer(address); }
#else
static inline unsigned char* at(unsigned address) { return reinterpret_cast<unsigned char*>(static_cast<unsigned long>(address)); }
#endif

extern unsigned _14VIEffectVolume$mScene;

extern "C" {
float sinf(float x);
float atanf(float x);
void Identity__10VIMatrix44(float* matrix);
void RotateY__10VIMatrix44f(float* matrix, float angle);
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
int RenderLightShafts__14VIEffectVolumeR8VICameraR10VIMatrix44(unsigned char* self, unsigned char* camera, const float* matrix) {
    if (*reinterpret_cast<float*>(self + 92) == 0.0f)
        return 0;
    if (*reinterpret_cast<float*>(self + 96) == 0.0f)
        return 0;
    float dir = 0.0f;
    unsigned scene = _14VIEffectVolume$mScene;
    bool beams = (*reinterpret_cast<unsigned*>(self + 60) & 2) != 0;
    float now = *reinterpret_cast<float*>(at(scene) + 8160);
    if (beams) {
        dir = -1.0f;
        float* intensity = reinterpret_cast<float*>(self + 92);
        if (now < 9.0f) {
            *intensity = 0.0f;
        } else {
            if (now < 15.0f)
                dir = (now - 9.0f) * 0x1.555554p-2f + -1.0f;
            else
                dir = 1.0f;
            if (now < 9.0f)
                *intensity = 0.0f;
            else if (now < 10.0f)
                *intensity = now - 9.0f;
            else if (now < 14.0f)
                *intensity = 1.0f;
            else if (now < 15.0f)
                *intensity = 15.0f - now;
            else
                *intensity = 0.0f;
        }
        if (*intensity == 0.0f)
            return 0;
        scene = _14VIEffectVolume$mScene;
    }
    unsigned char* particle = at(*reinterpret_cast<unsigned*>(self + 88));
    if (*reinterpret_cast<unsigned*>(self + 88) == 0)
        return -1;
    unsigned char* raster = at(*reinterpret_cast<unsigned*>(at(scene) + 19356));
    int material = *reinterpret_cast<int*>(self + 76);
    int count = *reinterpret_cast<int*>(self + 84);
    if (material != -1)
        SetMaterial__8VIRasteri(raster, material);
    else
        SetSolidFillUIAVGMaterial__8VIRaster(raster);
    float view[3] = {0.0f, 0.0f, 0.0f};
    alignas(16) float m1[16];
    alignas(16) float m2[16];
    alignas(16) float m3[16];
    zeroMatrix(m1);
    zeroMatrix(m2);
    const float* cam = reinterpret_cast<const float*>(camera + 8);
    view[0] = cam[0];
    view[1] = cam[1];
    view[2] = cam[2];
    Identity__10VIMatrix44(m2);
    float negX = -view[0];
#if defined(__mips__)
    __asm__("" : "+f"(negX));  // keep the negation before the divide (a/-x must not become -(a/x): NaN sign differs)
#endif
    if (0.0f < view[0]) {
        float angle = atanf(view[2] / negX) - 0x1.921fb4p+0f;
        RotateY__10VIMatrix44f(m2, angle);
    } else {
        float angle = atanf(view[2] / negX) + 0x1.921fb4p+0f;
        RotateY__10VIMatrix44f(m2, angle);
    }
    PrepareBillboardTransform__14VIEffectVolumeRC10VIMatrix44T1P10VIMatrix44(m3, self, m2, matrix, m1);
    float shift[3] = {0.0f, 0.0f, 0.0f};
    if ((*reinterpret_cast<unsigned*>(self + 60) & 2) != 0) {
        float along[3] = {dir, 0.0f, 0.0f};
        Mul__C10VIMatrix44RC7VIVect3R7VIVect3(m3, along, shift);
    }
    float q[12];
    for (int i = 0; i < 12; ++i)
        q[i] = 0.0f;
    float scale = (*reinterpret_cast<float*>(self + 96) * *reinterpret_cast<float*>(self + 92)) *
                  ((1.0f - *reinterpret_cast<float*>(self + 44)) * 0.5f + 0.5f);
    if (count > 0) {
        for (int n = count; n != 0; --n, particle += 32) {
            if (*reinterpret_cast<int*>(particle) == 0)
                continue;
            float c0[3] = {-*reinterpret_cast<float*>(particle + 16), 0.0f, 0.0f};
            Mul__C10VIMatrix44RC7VIVect3R7VIVect3(m1, c0, q);
            float c1[3] = {*reinterpret_cast<float*>(particle + 16), 0.0f, 0.0f};
            Mul__C10VIMatrix44RC7VIVect3R7VIVect3(m1, c1, q + 3);
            float c2[3] = {*reinterpret_cast<float*>(particle + 16), -*reinterpret_cast<float*>(particle + 20), 0.0f};
            Mul__C10VIMatrix44RC7VIVect3R7VIVect3(m1, c2, q + 6);
            float c3[3] = {-*reinterpret_cast<float*>(particle + 16), -*reinterpret_cast<float*>(particle + 20), 0.0f};
            Mul__C10VIMatrix44RC7VIVect3R7VIVect3(m1, c3, q + 9);
            for (int c = 0; c < 4; ++c) {
                q[3 * c] = q[3 * c] + *reinterpret_cast<float*>(particle + 4);
                q[3 * c + 1] = q[3 * c + 1] + *reinterpret_cast<float*>(particle + 8);
                q[3 * c + 2] = q[3 * c + 2] + *reinterpret_cast<float*>(particle + 12);
            }
            if ((*reinterpret_cast<unsigned*>(self + 60) & 2) != 0) {
                float drop[3];
                drop[0] = shift[0] * *reinterpret_cast<float*>(particle + 20);
                drop[1] = shift[1] * *reinterpret_cast<float*>(particle + 20);
                drop[2] = shift[2] * *reinterpret_cast<float*>(particle + 20);
                q[6] = q[6] + drop[0];
                q[7] = q[7] + drop[1];
                q[8] = q[8] + drop[2];
                float drop2[3];
                drop2[2] = shift[2] * *reinterpret_cast<float*>(particle + 20);
                drop2[0] = shift[0] * *reinterpret_cast<float*>(particle + 20);
                drop2[1] = shift[1] * *reinterpret_cast<float*>(particle + 20);
                q[9] = q[9] + drop2[0];
                q[10] = q[10] + drop2[1];
                q[11] = q[11] + drop2[2];
            }
            float color[4];
            color[0] = 1.0f;
            color[1] = 1.0f;
            color[2] = 1.0f;
            color[3] = sinf(*reinterpret_cast<float*>(particle + 24) / *reinterpret_cast<float*>(particle + 28) * 0x1.921fb4p+1f) * scale;
            Color__8VIRasterRC10VIColor32F(raster, color);
            BeginTriFan__8VIRaster(raster);
            static const int order[6] = {0, 1, 2, 3, 2, 1};
            static const float uv[6][2] = {{0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}, {1.0f, 1.0f}, {1.0f, 0.0f}};
            for (int i = 0; i < 6; ++i) {
                float coordinate[2] = {uv[i][0], uv[i][1]};
                UV__8VIRasterRC7VIVect2(raster, coordinate);
                Vertex__8VIRasterRC7VIVect3(raster, q + 3 * order[i]);
            }
            EndTriFan__8VIRaster(raster);
        }
    }
    _$_10VIMatrix44(m3, 2);
    _$_10VIMatrix44(m2, 2);
    _$_10VIMatrix44(m1, 2);
    return 0;
}
}
