#include "collide_env_ray.cpp"
#include <string.h>

static void* seenWorld;
static void* seenRay;
static int seenFlag;
extern "C" int Collide__7VIWorldR8VIEnvRay(void* world, void* ray) {
    seenWorld = world; seenRay = ray;
    seenFlag = *reinterpret_cast<int*>(static_cast<unsigned char*>(ray) + 52);
    return 77;
}

int main() {
    static unsigned char scene[1400], ray[64];
    memset(scene, 0x11, sizeof scene);
    memset(ray, 0x22, sizeof ray);
    *reinterpret_cast<int*>(scene + 1348) = 9;
    unsigned char before[1400];
    memcpy(before, scene, sizeof scene);
    if (Collide__7VISceneR8VIEnvRay(scene, ray) != 77) return 1;
    if (seenWorld != scene + 4 || seenRay != ray || seenFlag != 1) return 2;
    if (*reinterpret_cast<int*>(ray + 52) != 1) return 3;
    if (*reinterpret_cast<int*>(scene + 1348) != 10) return 4;
    before[1348] = 10;
    if (memcmp(before, scene, sizeof scene) != 0) return 5;
    for (int i = 0; i < 64; ++i)
        if (i != 52 && i != 53 && i != 54 && i != 55 && ray[i] != 0x22) return 6;
    return 0;
}
