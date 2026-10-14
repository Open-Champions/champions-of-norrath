// Observed: Slerp__6VIQuatRC6VIQuatT1f at 0x010cfdc0 is 492 bytes. Arguments: out (a0), a (a1) and
// b (a2), each four floats (x, y, z, w), and t (float register 12). No value is returned.
// d = ((a0*b0 + a1*b1) + a2*b2) + a3*b3. If d > 0 the b weight starts as +t, otherwise d is
// negated and the b weight starts as -t. Let c = |d|-style value after that step (d for the first
// case, -d for the second). If 1 - c is greater than the constant 0x38d1b717 (the float nearest 1e-4) the
// calls are, in order: theta = acosf(c); inv = 1.0f / sinf(theta); wa = sinf((1-t)*theta) * inv;
// then sinf(t*theta) gives wb = sinf(t*theta) * inv in the first case and (-sinf(t*theta)) * inv
// in the second. Otherwise (the two quaternions are nearly equal) no library call is made and
// wa = 1 - t, wb = t in the first case and wb = -t in the second. Each output lane is
// a[i]*wa + b[i]*wb.
// Inferred: this is spherical interpolation that flips the sign of b when the quaternions are on
// opposite hemispheres, falling back to linear weights near zero angle. Unknown: whether callers
// guarantee unit length; nothing here normalizes, and a c above 1 gives a NaN from acosf.
extern "C" {
float acosf(float x);
float sinf(float x);

void Slerp__6VIQuatRC6VIQuatT1f(float* out, const float* a, const float* b, float t) {
    float d = a[0] * b[0] + a[1] * b[1] + a[2] * b[2] + a[3] * b[3];
    float wa, wb;
    if (d > 0.0f) {
        if (1.0f - d > 0.0001f) {
            float theta = acosf(d);
            float inv = 1.0f / sinf(theta);
            wa = sinf((1.0f - t) * theta) * inv;
            wb = sinf(t * theta) * inv;
        } else {
            wa = 1.0f - t;
            wb = t;
        }
    } else {
        d = -d;
        if (1.0f - d > 0.0001f) {
            float theta = acosf(d);
            float inv = 1.0f / sinf(theta);
            wa = sinf((1.0f - t) * theta) * inv;
            wb = -sinf(t * theta) * inv;
        } else {
            wa = 1.0f - t;
            wb = -t;
        }
    }
    out[0] = a[0] * wa + b[0] * wb;
    out[1] = a[1] * wa + b[1] * wb;
    out[2] = a[2] * wa + b[2] * wb;
    out[3] = a[3] * wa + b[3] * wb;
}
}
