// Observed: Transform__10VIMatrix44RC6VIQuatfRC7VIVect3 at 0x01099d88 is 240 bytes. Arguments:
// the matrix (a0, 16 floats), a quaternion q = (x, y, z, w) (a1), a float scale s (float register
// 12) and a translation vector (a2). With the doubled products 2xx, 2yy, 2zz, 2xy, 2xz, 2yz, 2wx,
// 2wy, 2wz (each formed as (2*first)*second, 2*first being first+first) it stores
//   m[0] = (1-(2yy+2zz))*s   m[1] = (2xy+2wz)*s       m[2]  = (2xz-2wy)*s     m[3]  = 0
//   m[4] = (2xy-2wz)*s       m[5] = (1-(2xx+2zz))*s   m[6]  = (2yz+2wx)*s     m[7]  = 0
//   m[8] = (2xz+2wy)*s       m[9] = (2yz-2wx)*s       m[10] = (1-(2xx+2yy))*s  m[11] = 0
//   m[12..14] = the translation's three floats       m[15] = 1.0f
// The quaternion is read completely before any store; the translation is read after the
// matrix stores. No value is returned.
// Inferred: the standard quaternion-to-rotation expansion with a uniform scale, in the row-vector
// layout that RotateHPR__10VIMatrix44RC7VIVect3 also uses (translation in the fourth row).
// Unknown: whether q must be unit length (nothing normalizes it).
extern "C" {
void Transform__10VIMatrix44RC6VIQuatfRC7VIVect3(float* m, const float* q, float s, const float* t) {
    float z = q[2], y = q[1], x = q[0], w = q[3];
    float z2 = z + z, y2 = y + y, x2 = x + x, w2 = w + w;
    float zz = z2 * z;
    float yy = y2 * y;
    float xx = x2 * x;
    float xz = x2 * z;
    float wz = w2 * z;
    float yz = y2 * z;
    float wx = w2 * x;
    float xy = x2 * y;
    float wy = w2 * y;
    m[3] = 0.0f;
    m[7] = 0.0f;
    m[11] = 0.0f;
    m[0] = (1.0f - (yy + zz)) * s;
    m[10] = (1.0f - (xx + yy)) * s;
    m[1] = (xy + wz) * s;
    m[2] = (xz - wy) * s;
    m[4] = (xy - wz) * s;
    m[5] = (1.0f - (xx + zz)) * s;
    m[6] = (yz + wx) * s;
    m[8] = (xz + wy) * s;
    m[9] = (yz - wx) * s;
    m[12] = t[0];
    m[13] = t[1];
    m[14] = t[2];
    m[15] = 1.0f;
}
}
