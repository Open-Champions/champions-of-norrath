// Observed: GetHitPoint__9VICollRay at 0x0109c1c8, 80 bytes. The first argument is the
// address of a three-float result (the hidden pointer for a vector returned by value) and
// the second is the ray. For each axis i it stores ray[i] + ray[6 + i] * ray[17], that is
// the three floats at +0 plus the three floats at +24 scaled by the float at +68. The
// result address is returned.
// Inferred: origin + direction * hit parameter, using the fields that Init__9VICollRay
// resets (+68 is initialised to one there); the +24 vector is the direction as
// recorded by Precalc. Unknown: member names.
extern "C" {
float* GetHitPoint__9VICollRay(float* out, const float* ray) {
    float t = ray[17];
    float x = ray[6] * t + ray[0];
    float y = ray[7] * t + ray[1];
    float z = ray[8] * t + ray[2];
    out[2] = z;
    out[0] = x;
    out[1] = y;
    return out;
}
}
