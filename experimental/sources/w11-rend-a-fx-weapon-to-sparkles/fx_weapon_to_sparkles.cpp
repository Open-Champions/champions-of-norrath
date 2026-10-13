#include "../w11-rend-a-common/rend.hpp"
// Observed fxWeaponToSparkles__FP12_modelHeaderT0P14AnimationStateG6Point3sffi at 0x001e6088, 260
// bytes. Arguments: model m1 (a0), model m2 (a1), the animation state (a2), a Point3 passed by
// address of a caller copy (a3; copied to a local on entry), a short yaw (t0), floats f12 and f13,
// and an int bone (t1). With attach = (signed byte at m2+19) + bone:
//   modelGetAttachmentMatrix__FP12_modelHeaderP14AnimationStateiP8Matrix34f(m1, state, attach,
//   &a, f12) fills a Matrix34 a (12 floats on the stack); f13 is not read.
//   matMakeTransYaw__FP8Matrix34G6Point3s(&b, &copy, (short)(yaw + 0x4000)) builds a second matrix;
//   matMul__FP8Matrix34N20(&a, &a, &b) replaces a by their product (argument order as in the
//   image: destination, a, b).
//   count = modelGetNmShadowPoints__FP12_modelHeader(m2); for each index from 0 below count:
//   modelGetShadowPos__FP12_modelHeaderP8Matrix34iP6Point3Pf(m2, &a, index, &position, &height)
//   and addEffect__FiG6Point3(14, &position). With count <= 0 nothing further is done.
// Nothing is returned.
// Inferred: effect kind 14 is spawned on each shadow point of the weapon model m2, placed by the
// attachment on m1 turned by the yaw. Unknown: units of the yaw, the float f12's role (it is only
// passed through), and the effect kind's meaning. Nothing here was executed on the EE.
extern "C" {
void modelGetAttachmentMatrix__FP12_modelHeaderP14AnimationStateiP8Matrix34f(
    unsigned char* model, unsigned char* state, int attach, float* matrix, float scale);
void matMakeTransYaw__FP8Matrix34G6Point3s(float* matrix, const float* point, short yaw);
void matMul__FP8Matrix34N20(float* destination, float* left, float* right);
int modelGetNmShadowPoints__FP12_modelHeader(unsigned char* model);
void modelGetShadowPos__FP12_modelHeaderP8Matrix34iP6Point3Pf(unsigned char* model, float* matrix, int index,
                                                             float* position, float* height);
void addEffect__FiG6Point3(int kind, const float* position);
void fxWeaponToSparkles__FP12_modelHeaderT0P14AnimationStateG6Point3sffi(
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
        addEffect__FiG6Point3(14, position);
    }
}
}
