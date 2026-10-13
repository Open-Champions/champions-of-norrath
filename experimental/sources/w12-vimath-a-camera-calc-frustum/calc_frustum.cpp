// Observed: CalcFrustum__8VICameraRC10VIMatrix44T1 at 0x01086d08 is 1188 bytes.
// Arguments: the camera (a0), a matrix (a1, named inverse below) and a second matrix (a2,
// named camera matrix below). It handles eight corner points (x, y, z, 1.0f) in this
// order, with signs for (x, y, z):
//   (-1,+1,-1) (+1,+1,-1) (+1,-1,-1) (-1,-1,-1) (-1,+1,+1) (+1,+1,+1) (+1,-1,+1) (-1,-1,+1)
// For each corner it (1) calls __ml__FRC7VIVect4RC10VIMatrix44(result, corner, inverse),
// an operator that returns a four-float value through a hidden result pointer; (2) copies
// the result's x, y, z, w into the corner buffer and multiplies all four by the
// reciprocal 1.0f / w (the division happens once, before the multiplies); (3) calls
// Mul__C10VIMatrix44RC7VIVect4R7VIVect3(camera matrix, buffer, camera + 304 + 12*i + 0)
// which writes three floats there. Corner i therefore lands at camera offsets 304, 316,
// 328, 340, 352, 364, 376, 388. After the eighth it calls CalcPlanes__9VIFrustum(camera +
// 304) and returns 0.
// Inferred: the corners are those of the clip cube in homogeneous coordinates, taken back
// through the inverse projection (a1) with a perspective divide and moved by a2, giving
// the frustum's eight corner points; they fill the first 96 bytes of the frustum object
// at camera+304, which ends where the plane records at +96 begin (Plane accessor).
// Unknown: the member names, and whether the loop was unrolled in the original source.
extern "C" {
void __ml__FRC7VIVect4RC10VIMatrix44(float* result, const float* vector, const float* matrix);
void Mul__C10VIMatrix44RC7VIVect4R7VIVect3(const float* matrix, const float* vector, float* out);
void CalcPlanes__9VIFrustum(void* frustum);

int CalcFrustum__8VICameraRC10VIMatrix44T1(unsigned char* camera, const float* inverse, const float* camera_matrix) {
    static const signed char corners[8][3] = {
        {-1, +1, -1}, {+1, +1, -1}, {+1, -1, -1}, {-1, -1, -1},
        {-1, +1, +1}, {+1, +1, +1}, {+1, -1, +1}, {-1, -1, +1},
    };
    float buffer[4];
    float result[4];
    for (int i = 0; i < 8; ++i) {
        buffer[0] = static_cast<float>(corners[i][0]);
        buffer[1] = static_cast<float>(corners[i][1]);
        buffer[2] = static_cast<float>(corners[i][2]);
        buffer[3] = 1.0f;
        __ml__FRC7VIVect4RC10VIMatrix44(result, buffer, inverse);
        float x = result[0], y = result[1], z = result[2], w = result[3];
        float reciprocal = 1.0f / w;
        buffer[0] = x * reciprocal;
        buffer[3] = w * reciprocal;
        buffer[1] = y * reciprocal;
        buffer[2] = z * reciprocal;
        Mul__C10VIMatrix44RC7VIVect4R7VIVect3(camera_matrix, buffer, reinterpret_cast<float*>(camera + 304 + 12 * i));
    }
    CalcPlanes__9VIFrustum(camera + 304);
    return 0;
}
}
