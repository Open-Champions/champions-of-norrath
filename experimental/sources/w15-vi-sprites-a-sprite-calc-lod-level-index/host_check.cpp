// Host check only; not a build input.
#include "calc_lod_level_index.cpp"
#include <cstring>
static int run(float x, float y, float z, int count, const float* d) {
    alignas(8) unsigned char sprite[256] = {};
    float matrix[16] = {};
    matrix[12] = x; matrix[13] = y; matrix[14] = z;
    std::memcpy(sprite + 168, &count, 4);
    for (int i = 0; i < count; ++i) std::memcpy(sprite + 172 + 4 * i, &d[i], 4);
    return CalcLODLevelIndex__9VIHSpriteRC10VIMatrix44(sprite, matrix);
}
int main() {
    const float d[4] = {1.0f, 4.0f, 6.0f, 10.0f};
    if (run(3, 4, 0, 4, d) != 1) return 1;     // 25 < 1? no -> 0; 25 < 16? no -> 1; 25 < 36? yes -> stop at level 1
    if (run(0.5f, 0, 0, 4, d) != -1) return 2;  // inside the first distance
    if (run(100, 0, 0, 4, d) != 3) return 3;    // beyond all
    if (run(1, 2, 3, 0, d) != -1) return 4;
    if (run(1, 2, 3, -5, d) != -1) return 5;
    const float e[1] = {5.0f};
    if (run(3, 4, 0, 1, e) != 0) return 6;       // exactly equal passes the entry
    return 0;
}
