// Host contract for eventCollisionWall__7VehicleP6Point3T1. Not an EE plan input.
#include <cstdio>
#include <cstring>
extern "C" void eventCollisionWall__7VehicleP6Point3T1(void*, void*, void*);
int main() {
    unsigned char object[64], before[64];
    for (int i = 0; i < 64; ++i) object[i] = before[i] = static_cast<unsigned char>(i * 7 + 1);
    float a[3] = {1, 2, 3}, b[3] = {4, 5, 6};
    eventCollisionWall__7VehicleP6Point3T1(object, a, b);
    if (std::memcmp(object, before, 64) != 0) { std::fprintf(stderr, "fail: object changed\n"); return 1; }
    if (a[0] != 1 || a[2] != 3 || b[0] != 4 || b[2] != 6) { std::fprintf(stderr, "fail: argument changed\n"); return 1; }
    return 0;
}
