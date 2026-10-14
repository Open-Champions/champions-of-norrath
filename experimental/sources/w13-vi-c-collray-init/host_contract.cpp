// Host contract, not an EE plan input. Build: c++ -std=c++17 -w host_contract.cpp && ./a.out
#include "collray-init.cpp"
#include <cstring>

static void* precalc_arg;
static float seen[6];
extern "C" void Precalc__9VICollRay(void* ray) {
    precalc_arg = ray;
    std::memcpy(seen, ray, sizeof seen);
    float* r = static_cast<float*>(ray);
    r[9] = 7.0f; r[10] = 8.0f; r[11] = 9.0f;
}

int main() {
    alignas(16) float ray[24];
    for (int i = 0; i < 24; ++i) ray[i] = 100.0f + i;
    float a[3] = {1, 2, 3}, b[3] = {4, 5, 6};
    if (Init__9VICollRayRC7VIVect3T1(ray, a, b) != 0) return 1;
    if (precalc_arg != ray) return 2;
    for (int i = 0; i < 3; ++i) if (seen[i] != a[i] || seen[3 + i] != b[i]) return 3;
    if (ray[17] != 1.0f) return 4;
    int index; std::memcpy(&index, ray + 18, 4);
    if (index != -1) return 5;
    if (ray[19] != 0 || ray[20] != 0 || ray[21] != 0) return 6;
    if (ray[14] != 7.0f || ray[15] != 8.0f || ray[16] != 9.0f) return 7;
    if (ray[22] != 122.0f || ray[12] != 112.0f || ray[13] != 113.0f) return 8;  // untouched
    return 0;
}
