// Host contract for norrath_body_init. Not an EE plan input. quatNormalize and
// matUntransform are recorded stubs; memset is the host one.
#include <cmath>
#include <cstdio>
#include <cstring>
int eRandState = 0;
struct Quat { float x, y, z, w; };
static int normalizes = 0, untransforms = 0;
static float* untransform_args[3];
extern "C" void quatNormalize__FP4Quat(Quat* q) {
    ++normalizes;
    const float s = 1.0f / std::sqrt(q->x * q->x + q->y * q->y + q->z * q->z + q->w * q->w);
    q->x *= s; q->y *= s; q->z *= s; q->w *= s;
}
extern "C" void matUntransform__FP6Point3P8Matrix33T0(float* out, float* m, float* in) {
    ++untransforms; untransform_args[0] = out; untransform_args[1] = m; untransform_args[2] = in;
}
extern "C" void norrath_body_init(float*, float, float, float, float, float);
int main() {
    float body[48];
    for (float& f : body) f = 99.0f;
    eRandState = 0;
    norrath_body_init(body, 2.0f, 3.0f, 1.0f, 2.0f, 4.0f);
    if (body[0] != 0.5f) return 1;
    if (body[1] != 12.0f / (6.0f * (4.0f + 16.0f))) return 2;
    if (body[2] != 12.0f / (6.0f * (1.0f + 16.0f))) return 3;
    if (body[3] != 12.0f / (6.0f * (1.0f + 4.0f))) return 4;
    if (eRandState != 0x3c6ef35f) return 5;
    // second normalize sees an already unit quaternion: (0, 0, z, w) with w = (state>>16)/65536
    const float w = float(0x3c6e) / 65536.0f;
    const float n = 1.0f / std::sqrt(1.0f + w * w);
    if (std::fabs(body[9] - n) > 1e-6f || std::fabs(body[10] - w * n) > 1e-6f) return 6;
    if (body[14] != 0 || body[15] != 0 || body[16] != 0) return 7;
    if (body[140 / 4 + 12] != 99.0f) return 8;                // nothing past offset 152 touched
    if (normalizes != 2 || untransforms != 1) return 9;
    if (untransform_args[0] != body + 35 || untransform_args[1] != body + 26 || untransform_args[2] != body + 14) return 10;
    // transposed rotation: both rows of a rotation about z agree with the closed form
    const float c = (1.0f - 2.0f * (body[9] * body[9])) , s2 = 2.0f * body[9] * body[10];
    if (std::fabs(body[17] - c) > 1e-5f || std::fabs(body[18] - (-s2)) > 1e-5f || std::fabs(body[20] - s2) > 1e-5f) return 11;
    if (std::fabs(body[25] - 1.0f) > 1e-6f) return 12;
    std::printf("body init contract passed\n");
    return 0;
}
