// Observed: GetHitPoint__C8VIEnvRay at 0x0116d208, 92 bytes. The first argument is the
// address of a three-float result (hidden pointer for a vector returned by value) and
// the second the ray. With a = the three floats at +0, b = the three floats at +12 and
// t = the float at +56, each result component is (b[i] - a[i]) * t + a[i]. The result
// address is returned.
// Inferred: a linear interpolation along a segment from a to b by the hit fraction t.
// Unknown: member names and which of a, b is the ray start.
extern "C" {
float* GetHitPoint__C8VIEnvRay(float* out, const float* ray) {
    float ax = ray[0], ay = ray[1], az = ray[2];
    float t = ray[14];
    float x = (ray[3] - ax) * t + ax;
    float y = (ray[4] - ay) * t + ay;
    float z = (ray[5] - az) * t + az;
    out[2] = z;
    out[0] = x;
    out[1] = y;
    return out;
}
}
