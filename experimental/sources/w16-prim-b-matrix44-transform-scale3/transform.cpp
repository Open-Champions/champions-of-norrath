// Observed: Transform__10VIMatrix44RC6VIQuatRC7VIVect3T2 at 0x01099e78 is 300 bytes. Arguments:
// the matrix (a0, 16 floats), a quaternion q = (x, y, z, w) (a1), a scale vector sc (a2) and a
// translation vector t (a3). It uses the same doubled products as the uniform-scale sibling
// Transform__10VIMatrix44RC6VIQuatfRC7VIVect3 and stores
//   m[0] = (1-(2yy+2zz))*sc[0]  m[1] = (2xy+2wz)*sc[0]  m[2]  = (2xz-2wy)*sc[0]  m[3]  = 0
//   m[4] = (2xy-2wz)*sc[1]      m[5] = (1-(2xx+2zz))*sc[1] m[6] = (2yz+2wx)*sc[1]  m[7]  = 0
//   m[8] = (2xz+2wy)*sc[2]      m[9] = (2yz-2wx)*sc[2]  m[10] = (1-(2xx+2yy))*sc[2] m[11] = 0
//   m[12..14] = the translation's three floats          m[15] = 1.0f
// The scale component is re-read from memory for each product, after the earlier matrix stores.
// No value is returned.
// Inferred: the per-axis-scale form of the same quaternion expansion; the scale multiplies each
// row of the rotation. Unknown: whether the scale may alias the matrix (it is re-read, so aliasing
// would change the result; no caller was checked) and whether q must be unit length.
extern "C" {
void Transform__10VIMatrix44RC6VIQuatRC7VIVect3T2(float* m, const float* q, const float* sc, const float* t) {
    float z = q[2], y = q[1], x = q[0], w = q[3];
    float z2 = z + z, y2 = y + y, x2 = x + x, w2 = w + w;
    float zz = z2 * z;
    float yy = y2 * y;
    float xy = x2 * y;
    float wz = w2 * z;
    float wy = w2 * y;
    float xz = x2 * z;
    float xx = x2 * x;
    float wx = w2 * x;
    float yz = y2 * z;
    m[0] = (1.0f - (yy + zz)) * sc[0];
    m[1] = (xy + wz) * sc[0];
    m[3] = 0.0f;
    m[2] = (xz - wy) * sc[0];
    m[4] = (xy - wz) * sc[1];
    m[5] = (1.0f - (xx + zz)) * sc[1];
    m[7] = 0.0f;
    m[6] = (yz + wx) * sc[1];
    m[8] = (xz + wy) * sc[2];
    m[9] = (yz - wx) * sc[2];
    m[11] = 0.0f;
    m[10] = (1.0f - (xx + yy)) * sc[2];
    m[12] = t[0];
    m[13] = t[1];
    m[14] = t[2];
    m[15] = 1.0f;
}
}
