// Observed: Frustum__8VIRasterR10VIMatrix44ffffff at 0x0110a938 is 192 bytes. The raster
// argument is unused. The six float arguments (call them a, b, c, d, n, f in order) fill the
// sixteen words of the matrix argument; the register order of the operations is kept below.
//   [0]  = 2n / (b - a)        [5]  = 2n / (c - d)
//   [8]  = (b + a) / (b - a)   [9]  = (c + d) / (c - d)
//   [10] = (f + n) / (f - n)   [11] = 1.0f
//   [14] = ((f * -2.0f) * n) / (f - n)
//   [1] [2] [3] [4] [6] [7] [12] [13] [15] = 0
// Nothing is returned.
// Inferred: this is a perspective-style frustum matrix; with n the near distance and f the far
// distance, (a, b) are the horizontal bounds and (c, d) the vertical bounds in the order the
// image divides by them (c is the larger vertical bound).
// Unknown: the exact side naming of a, b, c, d and the sign convention of the target pipeline.
extern "C" {
struct VIRaster;

struct VIMatrix44 {
    float m[16];
};

void Frustum__8VIRasterR10VIMatrix44ffffff(VIRaster*, VIMatrix44& matrix, float a, float b, float c,
                                           float d, float n, float f) {
    float* m = matrix.m;
    float sumH = b + a;
    float sumV = c + d;
    float scaled = f * -2.0f;
    float twoN = n + n;
    float diffH = b - a;
    float depth = f - n;
    float diffV = c - d;
    scaled = scaled * n;
    float sumD = f + n;
    m[15] = 0.0f;
    m[11] = 1.0f;
    m[1] = 0.0f;
    m[2] = 0.0f;
    m[3] = 0.0f;
    m[4] = 0.0f;
    m[6] = 0.0f;
    m[7] = 0.0f;
    m[12] = 0.0f;
    m[13] = 0.0f;
    m[8] = sumH / diffH;
    m[9] = sumV / diffV;
    m[5] = twoN / diffV;
    m[14] = scaled / depth;
    m[10] = sumD / depth;
    m[0] = twoN / diffH;
}
}
