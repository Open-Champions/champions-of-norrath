// Host contract, not an EE plan input. Build: c++ -std=c++17 -w host_contract.cpp && ./a.out
#include "bbox-transform.cpp"
#include <cstring>

static float seen[8][3];
static int calls;
extern "C" void Mul__C10VIMatrix44RC7VIVect3R7VIVect3(const void* matrix, const float* in, float* out) {
    const float* m = static_cast<const float*>(matrix);
    std::memcpy(seen[calls++], in, 12);
    for (int i = 0; i < 3; ++i) out[i] = in[0] * m[i] + in[1] * m[4 + i] + in[2] * m[8 + i] + m[12 + i];
}

int main() {
    float box[6] = {-1.0f, 2.0f, -3.0f, 4.0f, 5.0f, 6.0f};
    float m[16] = {0, 1, 0, 0,  -1, 0, 0, 0,  0, 0, 2, 0,  10, 20, 30, 1};  // rotate 90 about z, scale z, move
    float out[6];
    for (int i = 0; i < 6; ++i) out[i] = 777.0f;
    if (Transform__C6VIBBoxRC10VIMatrix44R6VIBBox(box, m, out) != 0 || calls != 8) return 1;
    const float want_corner[8][3] = {
        {-1, 2, -3}, {4, 2, -3}, {4, 2, 6}, {-1, 2, 6}, {-1, 5, -3}, {4, 5, -3}, {4, 5, 6}, {-1, 5, 6},
    };
    for (int k = 0; k < 8; ++k)
        for (int j = 0; j < 3; ++j)
            if (seen[k][j] != want_corner[k][j]) return 2;
    // x' = -y + 10 in [5, 8]; y' = x + 20 in [19, 24]; z' = 2z + 30 in [24, 42]
    const float want[6] = {5.0f, 19.0f, 24.0f, 8.0f, 24.0f, 42.0f};
    for (int i = 0; i < 6; ++i)
        if (out[i] != want[i]) return 3;
    return 0;
}
