#include "../w11-rend-a-common/rend.hpp"
// Observed fxRunWeapon__FPQ24item11descriptionP12_modelHeaderP13fxWeaponStateG6Point3sT1P14AnimationStatefiifP8Matrix34
// at 0x001dc2c0, 10448 bytes. The mangled type list gives: desc (item::description), wm (_modelHeader),
// st (fxWeaponState), a point by value (so a pointer), a short yaw, am (T1 repeats the second type: a
// second _modelHeader), anim, a float scaleA (f12), an int i1, an int bone (stack), a float scaleB
// (f13) and a Matrix34 pointer mtx (stack). Records are addressed by offset.
// desc words used: u64 at +16 and +24 (four halfwords each), and four 8-byte modifier entries from +36:
// a halfword type at +36+8k and a halfword value at +38+8k. "mod(type)" below is the value of the first
// entry whose type matches, or zero.
// st (fxWeaponState) words used: 0..47 (a 3x4 float matrix, the previous one), +12, +16, +20, +36, +40,
// +44 (floats), +48 (frame number), +52 (lightning id), +56 (trail id), bytes +60, +61, +62.
//  1. Nothing happens when levelFlags has 0x10000000 or wm is zero. st[60] is cleared when i1 is zero,
//     else incremented while its signed value is below 100.
//  2. M (3x4 floats) is the identity unless i1 != 0 or mod(24), mod(22), mod(21) or the u64 at desc+16 is
//     nonzero; then it is modelGetAttachmentMatrix(am, anim, (signed char)wm[19] + bone, M, scaleB) (identity
//     when am is zero) multiplied by matMakeTransYaw(point, yaw + 0x4000). A nonzero mtx replaces M.
//  3. levelFlags & 0x100 (i1 != 0): from the trail endpoints (modelGetWeaponTrailPos(wm, E, F)), the
//     translation M[9..11] (A) and the vector B = M[3..5], H2 = A + B * F.y; t = clamp(2 * |B - st[12..]|,
//     0, 1); with a random step, one in (6 - (int)(t * 5)) calls addEffect(17, H2). With i1 == 0 only
//     st[56], st[61] and st[62] are cleared.
//     Without 0x100 and with i1 != 0 and mtx == 0: the same endpoints give H = A + B * E.y and H2 =
//     A + B * F.y; w = min(clamp(|B - st[12..]| * 0.68, 0, 1) * 0.3, (float)st[60] * 0.05); a trail is
//     started (P_StartTrail(3, 1000, 0.2, 2, 128, 128, 128, 0)) when st[56] is zero or st[48] is not
//     frameNm - 1, else a quad between the previous and current midpoints is extended first; then
//     P_ExtendSwoosh(st[56], H2, H, w). A countdown st[62] (a spray, kind st[61] = 1 blood, 2 ecto, 3
//     slime) scatters particles between the two endpoint lines while g_BloodLevel > 0 and decrements.
//     Otherwise (mtx given or i1 == 0) st[56], st[61], st[62] are cleared.
//  4. When the u64 at desc+16 is nonzero, one halo sprite per shadow point of wm. The halfword quads at
//     desc+24 and desc+16 each move towards zero by 2 per call, not past it.
//  5. mod(24) and scaleA > 0: lightning sword particles along the weapon's movement since st's matrix, then a
//     lightning bolt between two random shadow points (P_StartLightning / P_MoveLightning, id in st[52]);
//     otherwise st[52] is cleared. mod(22): ice sword particles along the same sweep and, with i1 != 0,
//     two new-ice sweeps from the endpoint lines. mod(21): flame sword particles.
//  6. With i1 != 0 st[48] = frameNm; then st[0..47] = M.
// Nothing is returned.
// Inferred: weapon visual effects (trail, blood, halo, lightning, ice, flame). Unknown: the layouts of
// description and fxWeaponState beyond the words named, the units of the modifier values, and all
// constant meanings. Nothing here was executed on the EE.
extern "C" {
extern int levelFlags, frameNm, eRandState, g_BloodLevel;
extern unsigned char* haloTexture2;
extern unsigned char lightningSwordParticle[], iceSwordParticle[], newIceSwordParticle1[], newIceSwordParticle2[];
extern unsigned char newFlameSwordParticle1[], newFlameSwordParticle2[], newFlameSwordParticle3[];
extern unsigned char particleWeaponBlood[], particleWeaponEcto[], particleWeaponSlime[];
void modelGetAttachmentMatrix__FP12_modelHeaderP14AnimationStateiP8Matrix34f(unsigned char* model, unsigned char* state, int attach, float* matrix, float scale);
void matIdent__FP8Matrix34(float* matrix);
void matMakeTransYaw__FP8Matrix34G6Point3s(float* matrix, const float* point, short yaw);
void matMul__FP8Matrix34N20(float* destination, float* left, float* right);
void modelGetWeaponTrailPos__FP12_modelHeaderP6Point3T1(unsigned char* model, float* a, float* b);
void addEffect__FiG6Point3(int kind, const float* point);
void P_ExtendSwoosh__FiG6Point3T1f(int id, const float* a, const float* b, float width);
int P_StartTrail__Fiifiiiii(int a, int b, float c, int d, int e, int f, int g, int h);
void P_AddParticle__FP11ParticleDefP6Point3T1f(const void* definition, float* position, float* velocity, float arg);
int modelGetNmShadowPoints__FP12_modelHeader(unsigned char* model);
void modelGetShadowPos__FP12_modelHeaderP8Matrix34iP6Point3Pf(unsigned char* model, float* matrix, int index, float* position, float* height);
float isin__Fi(int angle);
void P_FaceSprite__FG6Point3ffiiiiP8_texturesi(const float* position, float a, float b, int c, int d, int e, int f, unsigned char* texture, short g, int h);
void matTransform__FP6Point3P8Matrix34T0(float* out, float* matrix, float* in);
int P_LightningIsLive__Fi(int id);
int P_StartLightning__FG6Point3T0iiffUi(const float* a, const float* b, int c, int d, float e, float f, unsigned g);
void P_MoveLightning__FiG6Point3T1ii(int id, const float* a, const float* b, int c, int d);
}
namespace {
using namespace rend;
typedef unsigned long long __attribute__((may_alias)) u64;
inline unsigned next(unsigned s) { return s * 0x19660du + 0x3c6ef35fu; }
inline int mod(const unsigned char* d, int type) {
    for (int i = 0; i < 4; ++i)
        if (at<i16>(d, 36 + 8 * i) == type) return at<i16>(d, 38 + 8 * i);
    return 0;
}
// Three draws per attempt, each (float)(state >> 16) * 2^-16 doubled and reduced by one, repeated while
// the point lies outside the unit ball. The state is advanced in place.
void unitBall(float* out, unsigned& s) {
    float x, y, z;
    do {
        s = next(s);
        const float r1 = static_cast<float>(s >> 16);
        s = next(s);
        const float r2 = static_cast<float>(s >> 16);
        s = next(s);
        const float r3 = static_cast<float>(s >> 16);
        x = r1 * 0x1p-16f;
        x = x + x;
        x = x - 1.0f;
        y = r2 * 0x1p-16f;
        y = y + y;
        y = y - 1.0f;
        z = r3 * 0x1p-16f;
        z = z + z;
        z = z - 1.0f;
    } while (1.0f < x * x + y * y + z * z);
    out[0] = x;
    out[1] = y;
    out[2] = z;
}
// Distance the point (0, 0, 36) moved between the previous matrix and the new one, limited to 30.
float sweepLength(float* matrix, float* previous) {
    float p[3] = {0.0f, 0.0f, 36.0f};
    matTransform__FP6Point3P8Matrix34T0(p, matrix, p);
    float q[3] = {0.0f, 0.0f, 36.0f};
    matTransform__FP6Point3P8Matrix34T0(q, previous, q);
    const float dx = p[0] - q[0];
    const float dy = p[1] - q[1];
    const float dz = p[2] - q[2];
    float length = __builtin_sqrtf(dx * dx + dy * dy + dz * dz);
    if (30.0f < length) length = 30.0f;
    return length;
}
void stepMatrix(float* step, const float* matrix, const float* previous, float inverse) {
    for (int a = 0; a < 4; ++a)
        for (int k = 0; k < 3; ++k) step[3 * a + k] = (matrix[3 * a + k] - previous[3 * a + k]) * inverse;
}
void addMatrix(float* to, const float* step) {
    for (int a = 0; a < 4; ++a)
        for (int k = 0; k < 3; ++k) to[3 * a + k] = to[3 * a + k] + step[3 * a + k];
}
// Pick a random shadow point of the model and place it into position/height using the given matrix.
void randomShadowPos(unsigned char* wm, float* matrix, int count, float* position, float* height) {
    const unsigned s = next(static_cast<unsigned>(eRandState));
    const int index = static_cast<int>(s >> 16) % count;
    eRandState = static_cast<int>(s);
    modelGetShadowPos__FP12_modelHeaderP8Matrix34iP6Point3Pf(wm, matrix, index, position, height);
}
// Jitter (r * 2^-16 - 0.5) * scale per axis from three draws.
void jitter(float* out, unsigned& s, float scale) {
    s = next(s);
    const float r1 = static_cast<float>(s >> 16);
    s = next(s);
    const float r2 = static_cast<float>(s >> 16);
    s = next(s);
    const float r3 = static_cast<float>(s >> 16);
    out[0] = (r1 * 0x1p-16f - 0.5f) * scale;
    out[1] = (r2 * 0x1p-16f - 0.5f) * scale;
    out[2] = (r3 * 0x1p-16f - 0.5f) * scale;
}
// One new-ice sweep: particles scattered between the endpoint lines of the weapon blade (A + 36 B) and the
// stored previous endpoints (C + 36 D), moving A towards C and B towards D.
void iceSweep(const float* a0, const float* b0, const float* C, const float* D, bool doubled, const void* particle) {
    float A[3] = {a0[0], a0[1], a0[2]};
    float B[3] = {b0[0], b0[1], b0[2]};
    const float G[3] = {B[0] * 36.0f, B[1] * 36.0f, B[2] * 36.0f};
    const float H[3] = {A[0] + G[0], A[1] + G[1], A[2] + G[2]};
    const float G2[3] = {D[0] * 36.0f, D[1] * 36.0f, D[2] * 36.0f};
    const float H2[3] = {C[0] + G2[0], C[1] + G2[1], C[2] + G2[2]};
    const float dx = H[0] - H2[0];
    const float dy = H[1] - H2[1];
    const float dz = H[2] - H2[2];
    float length = __builtin_sqrtf(dx * dx + dy * dy + dz * dz);
    if (30.0f < length) length = 30.0f;
    const int count = static_cast<int>(length * 0x1.d4fdf2p-3f);
    const float spread = length * 0x1.f0a3d6p-1f;
    const float inverse = 1.0f / static_cast<float>(count);
    const float W[3] = {(C[0] - A[0]) * inverse, (C[1] - A[1]) * inverse, (C[2] - A[2]) * inverse};
    const float W2[3] = {(D[0] - B[0]) * inverse, (D[1] - B[1]) * inverse, (D[2] - B[2]) * inverse};
    const int loops = doubled ? count * 2 : count;
    if (loops <= 0) return;
    for (int j = 0; j < loops; ++j) {
        unsigned s = static_cast<unsigned>(eRandState);
        s = next(s);
        const float r1 = static_cast<float>(s >> 16);
        s = next(s);
        const float r2 = static_cast<float>(s >> 16);
        const float u1 = r1 * 0x1p-16f * 30.0f + 6.0f;
        const float v = spread * (r2 * 0x1p-16f);
        float pt[3] = {A[0] + B[0] * u1, A[1] + B[1] * u1, A[2] + B[2] * u1};
        float vel[3] = {B[0] * v, B[1] * v, B[2] * v};
        float u[3];
        unitBall(u, s);
        eRandState = static_cast<int>(s);
        vel[0] = vel[0] + u[0] * 0x1.6a3d70p+1f;
        vel[1] = vel[1] + u[1] * 0x1.6a3d70p+1f;
        vel[2] = vel[2] + u[2] * 0x1.6a3d70p+1f;
        P_AddParticle__FP11ParticleDefP6Point3T1f(particle, pt, vel, 0.0f);
        A[0] = A[0] + W[0];
        A[1] = A[1] + W[1];
        A[2] = A[2] + W[2];
        B[0] = B[0] + W2[0];
        B[1] = B[1] + W2[1];
        B[2] = B[2] + W2[2];
    }
}
}  // namespace
extern "C" void fxRunWeapon__FPQ24item11descriptionP12_modelHeaderP13fxWeaponStateG6Point3sT1P14AnimationStatefiifP8Matrix34(
    unsigned char* desc, unsigned char* wm, unsigned char* st, const float* position, short yaw, unsigned char* am,
    unsigned char* anim, float scaleA, int i1, int bone, float scaleB, const float* mtx) {
    const float point[3] = {position[0], position[1], position[2]};
    if ((levelFlags & 0x10000000) != 0) return;
    if (wm == nullptr) return;
    if (i1 == 0) st[60] = 0;
    else if (at<i8>(st, 60) < 100) st[60] = static_cast<unsigned char>(st[60] + 1);
    float M[12];
    if (i1 != 0 || mod(desc, 24) != 0 || mod(desc, 22) != 0 || mod(desc, 21) != 0 || at<u64>(desc, 16) != 0) {
        if (am != nullptr) {
            modelGetAttachmentMatrix__FP12_modelHeaderP14AnimationStateiP8Matrix34f(am, anim, at<i8>(wm, 19) + bone, M, scaleB);
        } else {
            matIdent__FP8Matrix34(M);
        }
        float T[12];
        matMakeTransYaw__FP8Matrix34G6Point3s(T, point, static_cast<short>(yaw + 0x4000));
        matMul__FP8Matrix34N20(M, M, T);
    } else {
        matIdent__FP8Matrix34(M);
    }
    if (mtx != nullptr) {
        for (int i = 0; i < 12; ++i) M[i] = mtx[i];
    }
    float* const previous = reinterpret_cast<float*>(st);
    if ((levelFlags & 0x100) != 0) {
        if (i1 != 0) {
            const float A[3] = {M[9], M[10], M[11]};
            const float B[3] = {M[3], M[4], M[5]};
            const float C[3] = {at<f32>(st, 12), at<f32>(st, 16), at<f32>(st, 20)};
            float E[3], F[3];
            modelGetWeaponTrailPos__FP12_modelHeaderP6Point3T1(wm, E, F);
            const float f = F[1];
            const float G2[3] = {B[0] * f, B[1] * f, B[2] * f};
            const float H2[3] = {A[0] + G2[0], A[1] + G2[1], A[2] + G2[2]};
            const float I[3] = {B[0] - C[0], B[1] - C[1], B[2] - C[2]};
            float t = __builtin_sqrtf(I[0] * I[0] + I[1] * I[1] + I[2] * I[2]);
            t = t + t;
            if (1.0f < t) t = 1.0f;
            if (t < 0.0f) t = 0.0f;
            const int k = static_cast<int>(t * 5.0f);
            const unsigned s = next(static_cast<unsigned>(eRandState));
            eRandState = static_cast<int>(s);
            if (static_cast<int>(s >> 16) % (6 - k) == 0) addEffect__FiG6Point3(17, H2);
        } else {
            at<i32>(st, 56) = 0;
            st[61] = 0;
            st[62] = 0;
        }
    } else if (i1 == 0 || mtx != nullptr) {
        at<i32>(st, 56) = 0;
        st[61] = 0;
        st[62] = 0;
    } else {
        const float A[3] = {M[9], M[10], M[11]};
        const float B[3] = {M[3], M[4], M[5]};
        const float C[3] = {at<f32>(st, 12), at<f32>(st, 16), at<f32>(st, 20)};
        const float D[3] = {at<f32>(st, 36), at<f32>(st, 40), at<f32>(st, 44)};
        float E[3], F[3];
        modelGetWeaponTrailPos__FP12_modelHeaderP6Point3T1(wm, E, F);
        const float e1 = E[1];
        const float G[3] = {B[0] * e1, B[1] * e1, B[2] * e1};
        const float H[3] = {A[0] + G[0], A[1] + G[1], A[2] + G[2]};
        const float e2 = F[1];
        const float G2[3] = {B[0] * e2, B[1] * e2, B[2] * e2};
        const float H2[3] = {A[0] + G2[0], A[1] + G2[1], A[2] + G2[2]};
        const float I[3] = {B[0] - C[0], B[1] - C[1], B[2] - C[2]};
        const float cap = static_cast<float>(at<i8>(st, 60)) * 0x1.999998p-5f;
        float w = __builtin_sqrtf(I[0] * I[0] + I[1] * I[1] + I[2] * I[2]);
        w = w * 0x1.5c28f4p-1f;
        if (w < 0.0f) w = 0.0f;
        if (1.0f < w) w = 1.0f;
        w = w * 0x1.333332p-2f;
        if (cap < w) w = cap;
        if (at<i32>(st, 56) == 0 || at<i32>(st, 48) != frameNm - 1) {
            at<i32>(st, 56) = P_StartTrail__Fiifiiiii(3, 1000, 0x1.999998p-3f, 2, 128, 128, 128, 0);
        } else {
            const float P1[3] = {A[0] + D[0], A[1] + D[1], A[2] + D[2]};
            const float mid[3] = {P1[0] * 0.5f, P1[1] * 0.5f, P1[2] * 0.5f};
            const float P2[3] = {B[0] + C[0], B[1] + C[1], B[2] + C[2]};
            float n[3] = {P2[0] * 0.5f, P2[1] * 0.5f, P2[2] * 0.5f};
            const float r = 1.0f / __builtin_sqrtf(n[0] * n[0] + n[1] * n[1] + n[2] * n[2]);
            n[0] = n[0] * r;
            n[1] = n[1] * r;
            n[2] = n[2] * r;
            const float J1[3] = {n[0] * e1, n[1] * e1, n[2] * e1};
            const float K1[3] = {mid[0] + J1[0], mid[1] + J1[1], mid[2] + J1[2]};
            const float J2[3] = {n[0] * e2, n[1] * e2, n[2] * e2};
            const float K2[3] = {mid[0] + J2[0], mid[1] + J2[1], mid[2] + J2[2]};
            P_ExtendSwoosh__FiG6Point3T1f(at<i32>(st, 56), K2, K1, w);
        }
        P_ExtendSwoosh__FiG6Point3T1f(at<i32>(st, 56), H2, H, w);
        const int n = at<i8>(st, 62);
        if (n != 0) {
            const unsigned v1 = st[62];
            if (g_BloodLevel > 0) {
                float dv[3] = {H2[0] - H[0], H2[1] - H[1], H2[2] - H[2]};
                const float r = 1.0f / __builtin_sqrtf(dv[0] * dv[0] + dv[1] * dv[1] + dv[2] * dv[2]);
                dv[0] = dv[0] * r;
                dv[1] = dv[1] * r;
                dv[2] = dv[2] * r;
                dv[0] = dv[0] + dv[0];
                dv[1] = dv[1] + dv[1];
                dv[2] = dv[2] + dv[2];
                if (n > 0) {
                    int i = 0;
                    do {
                        const int count = static_cast<signed char>(st[62]) - i;
                        const float cnt = static_cast<float>(count);
                        const float dx = H2[0] - H[0];
                        const float dy = H2[1] - H[1];
                        const float dz = H2[2] - H[2];
                        const float sx = dx * 0x1.666666p-1f;
                        const float sy = dy * 0x1.666666p-1f;
                        const float sz = dz * 0x1.666666p-1f;
                        const float X1 = (H2[0] - sx) - dv[0] * cnt;
                        const float Y1 = (H2[1] - sy) - dv[1] * cnt;
                        const float Z1 = (H2[2] - sz) - dv[2] * cnt;
                        const float R[3] = {D[0] + C[0] * e2, D[1] + C[1] * e2, D[2] + C[2] * e2};
                        const int twice = i * 2;
                        const float inv = static_cast<float>(twice);
                        const float S[3] = {(R[0] - H2[0]) / inv, (R[1] - H2[1]) / inv, (R[2] - H2[2]) / inv};
                        for (int k = 0; k < twice; ++k) {
                            const float kf = static_cast<float>(k);
                            const float T[3] = {S[0] * kf, S[1] * kf, S[2] * kf};
                            float pt[3] = {X1 + T[0], Y1 + T[1], Z1 + T[2]};
                            float vel[3] = {dv[0] * 9.0f, dv[1] * 9.0f, dv[2] * 9.0f};
                            const int kind = static_cast<signed char>(st[61]);
                            if (kind == 1) P_AddParticle__FP11ParticleDefP6Point3T1f(particleWeaponBlood, pt, vel, 0.0f);
                            else if (kind < 2) {
                            } else if (kind == 2) P_AddParticle__FP11ParticleDefP6Point3T1f(particleWeaponEcto, pt, vel, 0.0f);
                            else if (kind == 3) P_AddParticle__FP11ParticleDefP6Point3T1f(particleWeaponSlime, pt, vel, 0.0f);
                        }
                        ++i;
                    } while (i < static_cast<signed char>(st[62]));
                }
            }
            st[62] = static_cast<unsigned char>(v1 - 1);
        }
    }
    if (at<u64>(desc, 16) != 0) {
        const int shadowCount = modelGetNmShadowPoints__FP12_modelHeader(wm);
        for (int j = 0; j < shadowCount; ++j) {
            float pos[3];
            float height;
            modelGetShadowPos__FP12_modelHeaderP8Matrix34iP6Point3Pf(wm, M, j, pos, &height);
            const float size = isin__Fi(frameNm << 10) * 250.0f;
            const float a = size + 0x1.388p+13f;
            P_FaceSprite__FG6Point3ffiiiiP8_texturesi(pos, -0x1.9b645ap+3f, a - 0x1.f74p+10f, at<i16>(desc, 16), at<i16>(desc, 18),
                                                      at<i16>(desc, 20), at<i16>(desc, 22), haloTexture2,
                                                      static_cast<short>((frameNm & 0x3ff) << 6), 3);
        }
    }
    for (int p = 0; p < 2; ++p) {
        i16* quad = reinterpret_cast<i16*>(desc + (p == 0 ? 24 : 16));
        for (int k = 0; k < 4; ++k) {
            short x = quad[k];
            if (x > 0) {
                x = static_cast<short>(x - 2);
                if (x < 0) x = 0;
            } else if (x < 0) {
                x = static_cast<short>(x + 2);
                if (x > 0) x = 0;
            }
            quad[k] = x;
        }
    }
    if (mod(desc, 24) == 0 || !(0.0f < scaleA)) {
        at<i32>(st, 52) = 0;
    } else {
        const float length = sweepLength(M, previous);
        const float start = length * 0x1.7d6b62p-9f;
        const int steps = static_cast<int>(static_cast<float>(static_cast<int>(length * 0x1.a428f4p+3f) + 1) * scaleA);
        const float inverse = 1.0f / static_cast<float>(steps);
        float step[12];
        stepMatrix(step, M, previous, inverse);
        float cur[12];
        for (int i = 0; i < 12; ++i) cur[i] = previous[i];
        const float stride = inverse * 0x1.555554p-5f;
        const int n = modelGetNmShadowPoints__FP12_modelHeader(wm);
        int rounds = (steps < 0 ? steps + 3 : steps) >> 2;
        if (rounds < 8) rounds = 8;
        float f22 = start;
        for (int j = 0; j < rounds; ++j) {
            float pos[3];
            float height;
            randomShadowPos(wm, cur, n, pos, &height);
            const float H = height * 0x1.4f8b58p-17f * 0x1.1accccp+5f;
            unsigned s = static_cast<unsigned>(eRandState);
            float jit[3];
            jitter(jit, s, H);
            pos[0] = pos[0] + jit[0];
            pos[1] = pos[1] + jit[1];
            pos[2] = pos[2] + jit[2];
            float vel[3] = {0.0f, 0.0f, 0x1.3bd70ap+4f};
            float u[3];
            unitBall(u, s);
            eRandState = static_cast<int>(s);
            vel[0] = vel[0] + u[0] * 3.0f;
            vel[1] = vel[1] + u[1] * 3.0f;
            vel[2] = vel[2] + u[2] * 3.0f;
            P_AddParticle__FP11ParticleDefP6Point3T1f(lightningSwordParticle, pos, vel, f22);
            addMatrix(cur, step);
            f22 = f22 + stride;
        }
        float p0[3], p1[3];
        float height;
        randomShadowPos(wm, M, n, p0, &height);
        randomShadowPos(wm, M, n, p1, &height);
        if (at<i32>(st, 52) == 0 || P_LightningIsLive__Fi(at<i32>(st, 52)) == 0) {
            at<i32>(st, 52) = P_StartLightning__FG6Point3T0iiffUi(p0, p1, 107, 4, 0x1.f1eb84p+2f, 0x1.3c28f4p+1f, 0x5e808080u);
        } else {
            P_MoveLightning__FiG6Point3T1ii(at<i32>(st, 52), p0, p1, static_cast<int>(scaleA * 94.0f), -1);
        }
    }
    if (mod(desc, 22) != 0) {
        const float length = sweepLength(M, previous);
        float f21 = length * 0x1.7d6b62p-9f;
        const int steps = static_cast<int>(static_cast<float>(static_cast<int>(length * 0x1.a428f4p+3f) + 26) * scaleA);
        const float inverse = 1.0f / static_cast<float>(steps);
        float step[12];
        stepMatrix(step, M, previous, inverse);
        float cur[12];
        for (int i = 0; i < 12; ++i) cur[i] = previous[i];
        const float stride = inverse * 0x1.555554p-5f;
        const int n = modelGetNmShadowPoints__FP12_modelHeader(wm);
        for (int j = 0; j < steps; ++j) {
            float pos[3];
            float height;
            randomShadowPos(wm, cur, n, pos, &height);
            const float H = height * 0x1.4f8b58p-17f * 0x1.1accccp+5f;
            unsigned s = static_cast<unsigned>(eRandState);
            float jit[3];
            jitter(jit, s, H);
            eRandState = static_cast<int>(s);
            pos[0] = pos[0] + jit[0];
            pos[1] = pos[1] + jit[1];
            pos[2] = pos[2] + jit[2];
            float vel[3] = {0.0f, 0.0f, 0.0f};
            P_AddParticle__FP11ParticleDefP6Point3T1f(iceSwordParticle, pos, vel, f21);
            addMatrix(cur, step);
            f21 = f21 + stride;
        }
        if (i1 != 0) {
            const float A[3] = {M[9], M[10], M[11]};
            const float B[3] = {M[3], M[4], M[5]};
            const float C[3] = {at<f32>(st, 36), at<f32>(st, 40), at<f32>(st, 44)};
            const float D[3] = {at<f32>(st, 12), at<f32>(st, 16), at<f32>(st, 20)};
            iceSweep(A, B, C, D, true, newIceSwordParticle1);
            iceSweep(A, B, C, D, false, newIceSwordParticle2);
        }
    }
    if (mod(desc, 21) != 0) {
        const float length = sweepLength(M, previous);
        float f21 = length * 0x1.7d6b62p-9f;
        const int steps = static_cast<int>(static_cast<float>(static_cast<int>(length * 0x1.a428f4p+3f) + 16) * scaleA);
        const float inverse = 1.0f / static_cast<float>(steps);
        float step[12];
        stepMatrix(step, M, previous, inverse);
        float cur[12];
        for (int i = 0; i < 12; ++i) cur[i] = previous[i];
        const float stride = inverse * 0x1.555554p-5f;
        const int n = modelGetNmShadowPoints__FP12_modelHeader(wm);
        for (int j = 0; j < steps; ++j) {
            float pos[3];
            float height;
            randomShadowPos(wm, cur, n, pos, &height);
            const float H = height * 0x1.4f8b58p-17f * 0x1.1accccp+5f;
            unsigned s = static_cast<unsigned>(eRandState);
            float jit[3];
            jitter(jit, s, H);
            pos[0] = pos[0] + jit[0];
            pos[1] = pos[1] + jit[1];
            pos[2] = pos[2] + jit[2];
            float vel[3] = {0.0f, 0.0f, 0x1.3bd70ap+4f};
            float u[3];
            unitBall(u, s);
            eRandState = static_cast<int>(s);
            vel[0] = vel[0] + u[0] * 0x1.a5b22cp+2f;
            vel[1] = vel[1] + u[1] * 0x1.a5b22cp+2f;
            vel[2] = vel[2] + u[2] * 0x1.a5b22cp+2f;
            P_AddParticle__FP11ParticleDefP6Point3T1f(newFlameSwordParticle2, pos, vel, f21);
            if ((j & 15) == 0) {
                P_AddParticle__FP11ParticleDefP6Point3T1f(newFlameSwordParticle1, pos, vel, f21);
                pos[2] = pos[2] + 6.0f;
                unsigned s2 = static_cast<unsigned>(eRandState);
                float u2[3];
                unitBall(u2, s2);
                eRandState = static_cast<int>(s2);
                vel[0] = vel[0] + u2[0] * 3.5f;
                vel[1] = vel[1] + u2[1] * 3.5f;
                vel[2] = vel[2] + u2[2] * 3.5f;
                P_AddParticle__FP11ParticleDefP6Point3T1f(newFlameSwordParticle3, pos, vel, f21);
            }
            addMatrix(cur, step);
            f21 = f21 + stride;
        }
    }
    if (i1 != 0) at<i32>(st, 48) = frameNm;
    for (int i = 0; i < 12; ++i) previous[i] = M[i];
}
