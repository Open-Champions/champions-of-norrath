// Host check only; not a build input.
#include "collide.cpp"
int main() {
    alignas(4) unsigned char ray[64] = {0};
    *reinterpret_cast<unsigned*>(ray + 44) = 0x80000005u;
    float v[3] = {0, 0, 0};
    if (Collide__8VIEnvRayUifR7VIVect3(ray, 4, 0.0f, v) != 1) return 1;
    if (Collide__8VIEnvRayUifR7VIVect3(ray, 2, 1.0f, v) != 0) return 2;
    if (Collide__8VIEnvRayUifR7VIVect3(ray, 0x80000000u, 1.0f, v) != 1) return 3;
    if (Collide__8VIEnvRayUifR7VIVect3(ray, 0, 1.0f, v) != 0) return 4;
    return Collide__8VIEnvRayUifR7VIVect3(ray, 0xffffffffu, 1.0f, v) == 1 ? 0 : 5;
}
