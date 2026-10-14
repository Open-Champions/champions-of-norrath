// Observed: Init__8VIEnvRayRC7VIVect3T1Ui at 0x0116d118, 240 bytes. With this = the ray
// object, a start vector, an end vector and an unsigned argument, it stores: the start
// vector's three floats at +0, the end vector's at +12, the difference end - start at +24
// (normalized in place by NormalizeMagn__7VIVect3, whose returned length is stored at +36),
// +40 = 1.0 / length (or 0.0 when the length compares equal to 0.0), +44 = the unsigned
// argument, +48 = 0, +52 = 1, +56 = 1.0f. It returns 0.
// Inferred: a segment-ray record with origin, end, unit direction, length and reciprocal
// length; +52, +48 and +56 look like state / hit-result fields reset for a new query (the
// scene Collide(VIEnvRay) path writes 1 to the word at +52 as well).
// Unknown: the member names, the meaning of the unsigned argument (a filter or flag word
// is assumed) and of the words at +48 / +52 / +56.
extern "C" {
float NormalizeMagn__7VIVect3(float* vector);

int Init__8VIEnvRayRC7VIVect3T1Ui(void* ray, const float* start, const float* end, unsigned int flags) {
    float* r = static_cast<float*>(ray);
    r[0] = start[0];
    r[1] = start[1];
    r[2] = start[2];
    r[3] = end[0];
    r[4] = end[1];
    r[5] = end[2];
    r[6] = r[3];
    r[7] = r[4];
    r[8] = r[5];
    r[6] = r[6] - r[0];
    r[7] = r[7] - r[1];
    r[8] = r[8] - r[2];
    float length = NormalizeMagn__7VIVect3(r + 6);
    r[9] = length;
    float inverse = 0.0f;
    if (length != 0.0f) inverse = 1.0f / length;
    r[10] = inverse;
    reinterpret_cast<unsigned int*>(r)[11] = flags;
    reinterpret_cast<int*>(r)[12] = 0;
    reinterpret_cast<int*>(r)[13] = 1;
    r[14] = 1.0f;
    return 0;
}
}
