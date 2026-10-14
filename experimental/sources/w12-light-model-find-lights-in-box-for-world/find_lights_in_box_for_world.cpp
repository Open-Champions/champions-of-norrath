#include "../w11-rend-a-common/rend.hpp"
// Observed findLightsInBox_forWorld__FG3BoxPP5Lighti at 0x0013c6f8, extent 540 (a local, not
// exported, symbol in the original). Arguments: a Box (24 bytes, passed as the address of a
// caller copy which is copied first), an output array of light pointers and an int capacity.
// Box floats: [0..2] and [3..5] are two corners, read as the low and high bounds on x, y and z.
// Light words used: next at +0, signed byte +8, floats +36 (a radius), +48, +52, +56 (position).
//  1. Cell range: columns from (int(box[0]) + 11220) >> 8, clamped to at least 0, to
//     (int(box[3]) + 11820) >> 8, clamped to at most 89; rows likewise from box[1] and box[4].
//     (int() is a truncating conversion.) If the row range is empty nothing is visited.
//  2. Rows low to high, columns low to high: the cell word at lightGrid + (row * 90 + column) * 4 is
//     the head of a list linked through next. A light is taken unless its radius (+36) is exactly
//     0.0f, or on some axis (position + radius < low bound) or (high bound < position - radius), or
//     the count already equals capacity - 1 or more, or its signed byte +8 differs from
//     interfaceMode. A taken light is stored at out[count] and the count increases.
//  3. out[count] = 0 ends the list. Returns nothing.
// Same cell mapping and test as findLightsAtPoint, but with a box instead of a point and the radius
// read at +36 rather than +28. Unknown: why the radius is kept twice and which copy the box test
// should use; what the Box type's corners mean beyond the comparisons. Nothing here was executed
// on the EE.
extern "C" {
extern unsigned char lightGrid[90 * 90 * 4];
extern int interfaceMode;
void findLightsInBox_forWorld__FG3BoxPP5Lighti(const float* boxArg, unsigned char* out, int max) {
    using namespace rend;
    float box[6];
    for (int i = 0; i < 6; ++i) box[i] = boxArg[i];
    int count = 0;
    int rowLow = (static_cast<int>(box[1]) + 11220) >> 8;
    if (rowLow < 0) rowLow = 0;
    int rowHigh = (static_cast<int>(box[4]) + 11820) >> 8;
    if (!(rowHigh < 90)) rowHigh = 89;
    int colLow = (static_cast<int>(box[0]) + 11220) >> 8;
    if (colLow < 0) colLow = 0;
    int colHigh = (static_cast<int>(box[3]) + 11820) >> 8;
    if (!(colHigh < 90)) colHigh = 89;
    for (int row = rowLow; row <= rowHigh; ++row) {
        for (int col = colLow; col <= colHigh; ++col) {
            for (unsigned char* light = ptr(lightGrid, (row * 90 + col) * 4); light != nullptr; light = ptr(light, 0)) {
                const float r = at<f32>(light, 36);
                if (r == 0.0f) continue;
                if (at<f32>(light, 48) + r < box[0] || box[3] < at<f32>(light, 48) - r) continue;
                if (at<f32>(light, 52) + r < box[1] || box[4] < at<f32>(light, 52) - r) continue;
                if (at<f32>(light, 56) + r < box[2] || box[5] < at<f32>(light, 56) - r) continue;
                if (!(count < max - 1)) continue;
                if (at<i8>(light, 8) != interfaceMode) continue;
                set_ptr(out, count * 4, light);
                ++count;
            }
        }
    }
    set_ptr(out, count * 4, nullptr);
}
}
