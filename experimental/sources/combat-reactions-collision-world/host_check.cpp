#include "collision_world.cpp"
#include <cstdio>
#include <cstring>
int main() {
    unsigned char object[64];
    unsigned char point[12];
    unsigned char before[64];
    std::memset(object, 0x5a, sizeof object);
    std::memset(point, 0x33, sizeof point);
    std::memcpy(before, object, sizeof object);
    msg_collisionWorld__10GameObjectG6Point3(object, point);
    if (std::memcmp(before, object, sizeof object) != 0) { std::fprintf(stderr, "fail: object changed\n"); return 1; }
    for (unsigned i = 0; i < sizeof point; ++i) if (point[i] != 0x33) { std::fprintf(stderr, "fail: point changed\n"); return 1; }
    return 0;
}
