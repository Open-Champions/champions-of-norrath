// Observed: Contains__C6VIBBoxRC7VIVect3 at 0x01071040, 144 bytes, in the pinned SLUS-20565 image. Arguments: the box (six consecutive floats) and
// a point (three floats). It returns 1 when box[0] <= p[0], box[1] <= p[1], box[2] <= p[2], p[0] <= box[3], p[1] <= box[4] and p[2] <= box[5], tested
// in that order with the first failing test returning 0; every comparison is inclusive (less-or-equal), so a point on a face counts as inside.
// Inferred: an axis-aligned box membership test with minima first, maxima second. Unknown: nothing beyond the raw lanes.
extern "C" {
int Contains__C6VIBBoxRC7VIVect3(const float* self, const float* point) {
    return self[0] <= point[0] && self[1] <= point[1] && self[2] <= point[2] && point[0] <= self[3] && point[1] <= self[4] &&
           point[2] <= self[5];
}
}
