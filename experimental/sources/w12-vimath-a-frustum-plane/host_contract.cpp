#include "plane.cpp"

int main() {
    alignas(16) static unsigned char frustum[512];
    for (int i = 0; i < 8; ++i)
        if (Plane__C9VIFrustumi(frustum, i) != frustum + 96 + 16 * i) return 1 + i;
    if (Plane__C9VIFrustumi(frustum, -1) != frustum + 80) return 20;
    return 0;
}
