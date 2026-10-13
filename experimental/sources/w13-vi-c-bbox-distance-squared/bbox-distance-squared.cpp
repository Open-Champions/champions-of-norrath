// Observed: DistanceSquared__C6VIBBoxRC7VIVect3 at 0x010722d0, 312 bytes. The box holds
// the minimum corner as three floats at +0 and the maximum corner as three floats at +12;
// the argument is a point. For each axis i it forms d[i] = (0 < min[i] - p[i]) ? that
// difference : 0, then replaces d[i] by p[i] - max[i] when d[i] < p[i] - max[i]. The
// result is d[0]*d[0] + d[1]*d[1] + d[2]*d[2], added left to right. Nothing in the
// box or point is written.
// Inferred: the squared distance from the point to the box (zero inside it).
// Unknown: member names.
extern "C" {
float DistanceSquared__C6VIBBoxRC7VIVect3(const float* box, const float* point) {
    float d[3];
    for (int i = 0; i < 3; ++i) {
        float below = box[i] - point[i];
        d[i] = (0.0f < below) ? below : 0.0f;
    }
    for (int i = 0; i < 3; ++i) {
        float above = point[i] - box[3 + i];
        d[i] = (d[i] < above) ? above : d[i];
    }
    return d[0] * d[0] + d[1] * d[1] + d[2] * d[2];
}
}
