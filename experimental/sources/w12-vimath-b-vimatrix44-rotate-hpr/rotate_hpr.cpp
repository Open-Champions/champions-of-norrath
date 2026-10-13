// Observed: RotateHPR__10VIMatrix44RC7VIVect3 at 0x010996d8 is 356 bytes. Arguments: this (a
// 16-float matrix), v (three floats). The calls are sinf(v[0]), cosf(v[0]), sinf(v[1]), cosf(v[1]),
// sinf(v[2]), cosf(v[2]); name them sx, cx, sy, cy, sz, cz. No Identity call is made; all sixteen
// floats are stored:
//   m[0]  = cz*cx + (sz*sy)*sx      m[1]  = sz*cy      m[2]  = (-cz)*sx + (sz*sy)*cx
//   m[4]  = (-sz)*cx + (cz*sy)*sx   m[5]  = cz*cy      m[6]  = sz*sx + (cz*sy)*cx
//   m[8]  = cy*sx                   m[9]  = -sy        m[10] = cy*cx
//   m[3], m[7], m[11], m[12], m[13], m[14] = 0.0f      m[15] = 1.0f
// Floating-point association follows the observed multiply and add order.
// Inferred: the vector holds three angles in radians in the order heading, pitch, roll as the
// symbol name suggests; nothing here proves which axis each one drives.
// Unknown: whether callers read a returned register (none is set meaningfully).
extern "C" {
float sinf(float angle);
float cosf(float angle);

void RotateHPR__10VIMatrix44RC7VIVect3(void* self, const void* angles) {
    float* m = static_cast<float*>(self);
    const float* v = static_cast<const float*>(angles);
    float sx = sinf(v[0]);
    float cx = cosf(v[0]);
    float sy = sinf(v[1]);
    float cy = cosf(v[1]);
    float sz = sinf(v[2]);
    float cz = cosf(v[2]);
    float szsy = sz * sy;
    float czsy = cz * sy;
    float m0 = cz * cx + szsy * sx;
    float m2 = (-cz) * sx + szsy * cx;
    float m4 = (-sz) * cx + czsy * sx;
    float m6 = sz * sx + czsy * cx;
    m[0] = m0;
    m[1] = sz * cy;
    m[2] = m2;
    m[8] = cy * sx;
    m[9] = -sy;
    m[10] = cy * cx;
    m[15] = 1.0f;
    m[4] = m4;
    m[5] = cz * cy;
    m[6] = m6;
    m[3] = 0.0f;
    m[7] = 0.0f;
    m[11] = 0.0f;
    m[12] = 0.0f;
    m[13] = 0.0f;
    m[14] = 0.0f;
}
}
