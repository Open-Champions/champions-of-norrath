// Observed: Bound__6VIBBoxiRC7VIVect3 at 0x01072480 is 164 bytes in the pinned SLUS-20565 image. The box is this: a minimum corner at
// +0 and a maximum corner at +12 (three floats each). With a nonzero first argument both corners are set to the point (a2). With a
// zero first argument each minimum component becomes min.s(box.min, point) and each maximum component max.s(point, box.max); the
// operand order is box first for the minimum and point first for the maximum. Nothing is returned.
// Inferred: grows the box to contain the point; the flag starts a new box. This is the callee of Init__6VIBBoxiP7VIVect3 (w13-vi-a),
// which passes 1 for the first point.
// Unknown: the box size beyond 24 bytes; the EE min.s/max.s treatment of NaN operands is not examined here. Nothing here ran on the EE.
#if defined(__mips__)
static inline float fp_min(float a, float b) { float out; __asm__("min.s %0, %1, %2" : "=f"(out) : "f"(a), "f"(b)); return out; }
static inline float fp_max(float a, float b) { float out; __asm__("max.s %0, %1, %2" : "=f"(out) : "f"(a), "f"(b)); return out; }
#else
static inline float fp_min(float a, float b) { return a < b ? a : b; }
static inline float fp_max(float a, float b) { return a > b ? a : b; }
#endif

extern "C" {
void Bound__6VIBBoxiRC7VIVect3(float* box, int first, const float* point) {
    float* low = box;
    float* high = box + 3;
    if (first != 0) {
        low[0] = point[0];
        low[1] = point[1];
        low[2] = point[2];
        high[0] = point[0];
        high[1] = point[1];
        high[2] = point[2];
        return;
    }
    low[0] = fp_min(low[0], point[0]);
    low[1] = fp_min(low[1], point[1]);
    low[2] = fp_min(low[2], point[2]);
    high[0] = fp_max(point[0], high[0]);
    high[1] = fp_max(point[1], high[1]);
    high[2] = fp_max(point[2], high[2]);
}
}
