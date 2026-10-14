#include "../w11-rend-a-common/rend.hpp"
// Observed fxWeaponToDust__FP12_modelHeaderT0P14AnimationStateG6Point3sffi at 0x001df298, 712
// bytes. Same arguments and set-up as fxWeaponToSparkles (models m1 and m2, state, a Point3 by
// address, a short yaw, floats f12 and f13, an int bone): the attachment matrix of m1 is
// multiplied by the yaw/translation matrix, then for each shadow point index i of m2
// (modelGetNmShadowPoints__FP12_modelHeader(m2) of them) modelGetShadowPos__FP12_modelHeaderP8
// Matrix34iP6Point3Pf(m2, &a, i, &position, &height) gives a position. Then three times:
//  * a point v inside the unit ball is drawn by rejection sampling: three generator steps
//    (state = state * 0x19660d + 0x3c6ef35f, 32-bit; value = state >> 16) each give
//    (float)value * 2^-16, doubled and reduced by 1.0, for x, y and z in that order; the draw
//    is repeated while 1.0f < x*x + y*y + z*z (sums in x, y, z order);
//  * v is scaled: x *= K, y *= K, z = z * K - 4.0f, with K the 4-byte constant in the image, the
//    float 0x1.b99998p+1 (3.45f truncated toward zero);
//  * three more steps give r1, r2, r3 = (state >> 16) % 6 (unsigned); the particle position is
//    (r1 - 3 + position.x, r2 - 3 + position.y, r3 - 3 + position.z);
//  * P_AddParticle__FP11ParticleDefP6Point3T1f(fxDust, &particlePosition, &v, 0.0f) is called.
// eRandState is read before each particle and written back after its three position steps (not
// during the rejection draws). Nothing is returned. f13 is not read.
// Inferred: dust puffs thrown from each shadow point of the weapon model (the target is named
// fxDust and the velocity is randomised inside a sphere). Unknown: ParticleDef contents of fxDust (it is only passed through) and the model coordinate
// convention. Nothing here was executed on the EE.
extern "C" {
extern int eRandState;
extern unsigned char fxDust[];
void modelGetAttachmentMatrix__FP12_modelHeaderP14AnimationStateiP8Matrix34f(
    unsigned char* model, unsigned char* state, int attach, float* matrix, float scale);
void matMakeTransYaw__FP8Matrix34G6Point3s(float* matrix, const float* point, short yaw);
void matMul__FP8Matrix34N20(float* destination, float* left, float* right);
int modelGetNmShadowPoints__FP12_modelHeader(unsigned char* model);
void modelGetShadowPos__FP12_modelHeaderP8Matrix34iP6Point3Pf(unsigned char* model, float* matrix, int index,
                                                             float* position, float* height);
void P_AddParticle__FP11ParticleDefP6Point3T1f(const void* definition, float* position, float* velocity, float arg);
namespace {
inline unsigned advance(unsigned state) { return state * 1664525u + 1013904223u; }
}
void fxWeaponToDust__FP12_modelHeaderT0P14AnimationStateG6Point3sffi(
    unsigned char* m1, unsigned char* m2, unsigned char* state, const float* point, short yaw, float scale,
    float unused, int bone) {
    using namespace rend;
    (void)unused;
    float copy[3] = {point[0], point[1], point[2]};
    float a[12];
    float b[12];
    modelGetAttachmentMatrix__FP12_modelHeaderP14AnimationStateiP8Matrix34f(m1, state, at<i8>(m2, 19) + bone, a, scale);
    matMakeTransYaw__FP8Matrix34G6Point3s(b, copy, static_cast<short>(yaw + 0x4000));
    matMul__FP8Matrix34N20(a, a, b);
    const int count = modelGetNmShadowPoints__FP12_modelHeader(m2);
    for (int i = 0; i < count; ++i) {
        float position[3];
        float height;
        modelGetShadowPos__FP12_modelHeaderP8Matrix34iP6Point3Pf(m2, a, i, position, &height);
        for (int n = 0; n != 3; ++n) {
            float v[3];
            unsigned s = static_cast<unsigned>(eRandState);
            for (;;) {
                s = advance(s);
                const float rx = static_cast<float>(static_cast<int>(s >> 16)) * 0x1p-16f;
                s = advance(s);
                const float ry = static_cast<float>(static_cast<int>(s >> 16)) * 0x1p-16f;
                s = advance(s);
                const float rz = static_cast<float>(static_cast<int>(s >> 16)) * 0x1p-16f;
                const float x = (rx + rx) - 1.0f;
                const float y = (ry + ry) - 1.0f;
                const float z = (rz + rz) - 1.0f;
                v[0] = x;
                v[1] = y;
                v[2] = z;
                if (!(1.0f < x * x + y * y + z * z)) break;
            }
            const float K = 0x1.b99998p+1f;
            v[0] = v[0] * K;
            v[1] = v[1] * K;
            v[2] = v[2] * K - 4.0f;
            s = advance(s);
            const unsigned r1 = (s >> 16) % 6u;
            s = advance(s);
            const unsigned r2 = (s >> 16) % 6u;
            s = advance(s);
            const unsigned r3 = (s >> 16) % 6u;
            eRandState = static_cast<int>(s);
            float p[3];
            p[0] = static_cast<float>(static_cast<int>(r1) - 3) + position[0];
            p[1] = static_cast<float>(static_cast<int>(r2) - 3) + position[1];
            p[2] = static_cast<float>(static_cast<int>(r3) - 3) + position[2];
            P_AddParticle__FP11ParticleDefP6Point3T1f(fxDust, p, v, 0.0f);
        }
    }
}
}
