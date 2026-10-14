#include "../w11-rend-a-common/rend.hpp"
// Observed findLightsAtPoint__FG6Point3PP5Lighti at 0x0013c528, 464 bytes. Arguments: a 12-byte point
// (passed by value, so the callee gets a pointer to a copy), an output array of light pointers and an
// integer capacity. The point is copied first. Light words used: next at +0, signed byte +8, floats
// +28 (radius) and +48, +52, +56 (position). Global: interfaceMode (int) and the lightGrid head words.
//  1. Cell range, with gx = (int(x) + 11520) >> 8 and the same for y: columns from the lower bound
//     (int(x) + 11220) >> 8, clamped to 0, to (int(x) + 11820) >> 8, clamped to 89; rows likewise from y.
//     (11520 + or - 300: the range covers 300 units either side of the point.) A row range or column
//     range that is empty visits nothing.
//  2. Rows from low to high, columns low to high, each cell word of lightGrid (row * 90 + column) is
//     the head of a list linked through next. A light is taken when the point lies inside its box on
//     all three axes, tested as (pos + radius < p) or (p < pos - radius) rejecting it, when the count
//     is below capacity - 1 and when its signed byte +8 equals interfaceMode. A taken light is stored
//     at out[count] and the count increases.
//  3. out[count] = 0 ends the list. The count is not returned.
// Inferred: gathers lights that can affect a point, for shading characters. Unknown: why the byte
// equals interfaceMode, and the actual radius limit that makes 300 enough. Nothing here was executed
// on the EE.
extern "C" {
extern unsigned char lightGrid[90 * 90 * 4];
extern int interfaceMode;
void findLightsAtPoint__FG6Point3PP5Lighti(const float* point, unsigned char* out, int max) {
    using namespace rend;
    const float p[3] = {point[0], point[1], point[2]};
    int count = 0;
    const int ix = static_cast<int>(p[0]);
    const int iy = static_cast<int>(p[1]);
    int colLow = (ix + 11220) >> 8;
    if (colLow < 0) colLow = 0;
    int colHigh = (ix + 11820) >> 8;
    if (!(colHigh < 90)) colHigh = 89;
    int rowLow = (iy + 11220) >> 8;
    if (rowLow < 0) rowLow = 0;
    int rowHigh = (iy + 11820) >> 8;
    if (!(rowHigh < 90)) rowHigh = 89;
    for (int row = rowLow; row <= rowHigh; ++row) {
        for (int col = colLow; col <= colHigh; ++col) {
            for (unsigned char* light = ptr(lightGrid, (row * 90 + col) * 4); light != nullptr; light = ptr(light, 0)) {
                const float r = at<f32>(light, 28);
                if (at<f32>(light, 48) + r < p[0] || p[0] < at<f32>(light, 48) - r) continue;
                if (at<f32>(light, 52) + r < p[1] || p[1] < at<f32>(light, 52) - r) continue;
                if (at<f32>(light, 56) + r < p[2] || p[2] < at<f32>(light, 56) - r) continue;
                if (!(count < max - 1)) continue;
                if (at<i8>(light, 8) != interfaceMode) continue;
                set_ptr(out, count * 4, light);
                ++count;
            }
        }
    }
    at<u32>(out, count * 4) = 0;
}
}
