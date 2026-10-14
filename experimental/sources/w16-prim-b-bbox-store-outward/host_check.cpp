#include "store_outward.cpp"
#include <string.h>

int main() {
    float box[6] = {-1.5f, -2.5f, -3.5f, 4.5f, 5.5f, 6.5f};
    float p[24];
    for (int i = 0; i < 24; ++i) p[i] = 99.0f;
    float want[24] = {-1, 0, 0, -1.5f,   1, 0, 0, -4.5f,   0, -1, 0, -2.5f,
                      0, 1, 0, -5.5f,    0, 0, -1, -3.5f,  0, 0, 1, -6.5f};
    if (StoreOutward__C6VIBBoxP7VIPlane(box, p) != 0) return 1;
    if (memcmp(p, want, sizeof p) != 0) return 2;
    // a point inside every plane has dot(n, p) + d <= 0 for all six
    float in[3] = {0.0f, 0.0f, 0.0f};
    for (int r = 0; r < 6; ++r)
        if (p[4 * r] * in[0] + p[4 * r + 1] * in[1] + p[4 * r + 2] * in[2] + p[4 * r + 3] > 0.0f) return 3;
    return 0;
}
