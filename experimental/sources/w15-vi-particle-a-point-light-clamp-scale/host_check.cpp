// Host check only; not a build input. Compares against an independent restatement of the rule.
#include "clamp_scale.cpp"
#include <cmath>
#include <cstring>
#define SYM_CALL ClampScale__12VIPointLightR10VIMatrix44
static float len(const float* r) { return std::sqrt(r[0] * r[0] + r[1] * r[1] + r[2] * r[2]); }
int main() {
    float cases[4][16] = {
        {1, 0, 0, 7, 0, 1, 0, 8, 0, 0, 1, 9, 5, 6, 7, 1},
        {8, 0, 0, 7, 0, 2, 0, 8, 0, 0, 0.5f, 9, 5, 6, 7, 1},
        {3, 4, 0, 7, 0, 30, 40, 8, 0, 0, 5, 9, 5, 6, 7, 1},
        {0, 0, 0, 1, 0, 0, 0, 2, 0, 0, 0, 3, 0, 0, 0, 4}};
    for (auto& c : cases) {
        float m[16]; std::memcpy(m, c, sizeof m);
        if (SYM_CALL(nullptr, m) != 0) return 1;
        for (int r = 0; r < 3; r++) {
            float before = len(c + 4 * r), after = len(m + 4 * r);
            if (before > 4.0f) { if (std::fabs(after - 4.0f) > 1e-4f) return 2; }
            else if (std::memcmp(m + 4 * r, c + 4 * r, 12)) return 3;
            if (m[4 * r + 3] != c[4 * r + 3]) return 4;
        }
        if (std::memcmp(m + 12, c + 12, 16)) return 5;
    }
    return 0;
}
