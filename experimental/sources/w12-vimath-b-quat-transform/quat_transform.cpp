// Observed: quatTransform__FP6Point3P4QuatT0 at 0x00157140 is 452 bytes. The mangled name types
// the arguments (Point3*, Quat*, Point3*): out, q = (qx, qy, qz, qw), p. All inputs are read before
// out is written, so out may alias p. It builds P = (px, py, pz, 0), c = (-qx, -qy, -qz, qw), then
//   T = q*P with the zero w term dropped:
//     T.x = qw*px + (qy*pz - qz*py)   T.y = qw*py + (qz*px - qx*pz)   T.z = qw*pz + (qx*py - qy*px)
//     T.w = -((qx*px + qy*py) + qz*pz)
//   R = T*c (full product, a=T b=c, same layout as quatMul__FP4QuatN20):
//     R.x = (Tw*cx + cw*Tx) + (Ty*cz - Tz*cy)   R.y = (Tw*cy + cw*Ty) + (Tz*cx - Tx*cz)
//     R.z = (Tw*cz + cw*Tz) + (Tx*cy - Ty*cx)
// and stores R.x, R.y, R.z to out[0..2]. R.w is computed in the original but never stored to out,
// so it is omitted. Returns out.
// Inferred: rotates the point p by the unit quaternion q (q p q^-1 with w last).
// Unknown: the Point3 and Quat type definitions beyond these floats.
extern "C" {
float* quatTransform__FP6Point3P4QuatT0(float* out, const float* q, const float* p) {
    const float qx = q[0], qy = q[1], qz = q[2], qw = q[3];
    const float px = p[0], py = p[1], pz = p[2];
    const float cx = -qx, cy = -qy, cz = -qz, cw = qw;
    const float tx = qw * px + (qy * pz - qz * py);
    const float ty = qw * py + (qz * px - qx * pz);
    const float tz = qw * pz + (qx * py - qy * px);
    const float tw = -((qx * px + qy * py) + qz * pz);
    const float rx = (tw * cx + cw * tx) + (ty * cz - tz * cy);
    const float ry = (tw * cy + cw * ty) + (tz * cx - tx * cz);
    const float rz = (tw * cz + cw * tz) + (tx * cy - ty * cx);
    out[0] = rx;
    out[1] = ry;
    out[2] = rz;
    return out;
}
}
