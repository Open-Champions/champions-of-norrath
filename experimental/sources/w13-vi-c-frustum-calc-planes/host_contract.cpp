// Host contract, not an EE plan input. Build: c++ -std=c++17 -w host_contract.cpp && ./a.out
#include "frustum-calc-planes.cpp"

static unsigned char* base;
static int seen[6][4], n;
extern "C" void __cl__7VIPlaneRC7VIVect3N21(void* plane, const float* p1, const float* p2, const float* p3) {
    seen[n][0] = static_cast<unsigned char*>(plane) - base;
    seen[n][1] = reinterpret_cast<const unsigned char*>(p1) - base;
    seen[n][2] = reinterpret_cast<const unsigned char*>(p2) - base;
    seen[n][3] = reinterpret_cast<const unsigned char*>(p3) - base;
    ++n;
}

int main() {
    alignas(16) unsigned char frustum[200] = {};
    base = frustum;
    if (CalcPlanes__9VIFrustum(frustum) != 0 || n != 6) return 1;
    const int want[6][4] = {
        {96, 12, 24, 36}, {112, 48, 84, 72}, {128, 0, 36, 84},
        {144, 60, 72, 24}, {160, 60, 12, 0}, {176, 24, 72, 84},
    };
    for (int i = 0; i < 6; ++i)
        for (int j = 0; j < 4; ++j)
            if (seen[i][j] != want[i][j]) return 2;
    return 0;
}
