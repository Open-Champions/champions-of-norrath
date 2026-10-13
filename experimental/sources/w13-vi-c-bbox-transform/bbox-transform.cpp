// Observed: Transform__C6VIBBoxRC10VIMatrix44R6VIBBox at 0x01071da8, 668 bytes. The box
// holds a minimum corner (three floats at +0) and a maximum corner (three floats at +12);
// the arguments are the matrix and an output box. The eight corners of the box are
// transformed in this order, with x0 y0 z0 the minimum corner and x1 y1 z1 the maximum:
//   (x0,y0,z0) (x1,y0,z0) (x1,y0,z1) (x0,y0,z1) (x0,y1,z0) (x1,y1,z0) (x1,y1,z1) (x0,y1,z1)
// each by Mul__C10VIMatrix44RC7VIVect3R7VIVect3(matrix, corner, result). The first result
// initialises both corners of the output box; every later result is folded in with
// min.s into the output minimum (min.s of the old minimum and the result) and max.s into
// the output maximum (max.s of the result and the old maximum), componentwise. The box is
// re-read for every corner. The result is zero.
// Inferred: the output is the axis-aligned bounds of the transformed box. The host build
// uses plain comparisons for min and max, which does not model the hardware NaN handling.
// Unknown: member names.
#if defined(__mips__)
static inline float fp_min(float a, float b) {
    float out;
    __asm__("min.s %0, %1, %2" : "=f"(out) : "f"(a), "f"(b));
    return out;
}
static inline float fp_max(float a, float b) {
    float out;
    __asm__("max.s %0, %1, %2" : "=f"(out) : "f"(a), "f"(b));
    return out;
}
#else
static inline float fp_min(float a, float b) { return a < b ? a : b; }
static inline float fp_max(float a, float b) { return a > b ? a : b; }
#endif

extern "C" {
void Mul__C10VIMatrix44RC7VIVect3R7VIVect3(const void* matrix, const float* in, float* out);

int Transform__C6VIBBoxRC10VIMatrix44R6VIBBox(const float* box, const void* matrix, float* out) {
    // Bit 0: take x1, bit 1: take y1, bit 2: take z1.
    static const unsigned char corners[8] = {0, 1, 5, 4, 2, 3, 7, 6};
    float result[3];
    float* low = out;
    float* high = out + 3;
    for (int k = 0; k < 8; ++k) {
        unsigned bits = corners[k];
        float corner[3];
        corner[0] = box[(bits & 1) ? 3 : 0];
        corner[1] = box[(bits & 2) ? 4 : 1];
        corner[2] = box[(bits & 4) ? 5 : 2];
        Mul__C10VIMatrix44RC7VIVect3R7VIVect3(matrix, corner, result);
        if (k == 0) {
            low[1] = result[1];
            low[0] = result[0];
            low[2] = result[2];
            high[0] = result[0];
            high[1] = result[1];
            high[2] = result[2];
        } else {
            low[0] = fp_min(low[0], result[0]);
            low[1] = fp_min(low[1], result[1]);
            low[2] = fp_min(low[2], result[2]);
            high[0] = fp_max(result[0], high[0]);
            high[1] = fp_max(result[1], high[1]);
            high[2] = fp_max(result[2], high[2]);
        }
    }
    return 0;
}
}
