#include "cull.cpp"
#include <math.h>

int main() {
    const float nan = nanf("");
    const float vals[] = {-2.0f, 0.0f, 1.0f, 3.0f, nan};
    int n = sizeof vals / sizeof vals[0];
    // Box fixed, each segment coordinate swept: compare with an independent per-axis formulation.
    float box[6] = {-1.0f, -1.0f, -1.0f, 2.0f, 2.0f, 2.0f};
    for (int axis = 0; axis < 3; ++axis)
        for (int a = 0; a < n; ++a)
            for (int b = 0; b < n; ++b) {
                float seg[6] = {0, 0, 0, 1, 1, 1};
                seg[axis] = vals[a];
                seg[3 + axis] = vals[b];
                bool before = box[3 + axis] < seg[axis] && box[3 + axis] < seg[3 + axis];
                bool after = seg[axis] < box[axis] && seg[3 + axis] < box[axis];
                if (Cull__8VIRasterR13VIPickSegmentR6VIBBox(0, seg, box) != (before || after ? 1 : 0)) return 1;
            }
    float seg[6] = {0, 0, 0, 1, 1, 1};
    if (Cull__8VIRasterR13VIPickSegmentR6VIBBox(0, seg, box) != 0) return 2;
    return 0;
}
