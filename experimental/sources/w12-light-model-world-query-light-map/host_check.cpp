// Host contract for worldQueryLightMap__FP12_worldHeaderG6Point3. Not an EE plan input.
#include <cstdio>
struct Point3 { float x, y, z; };
extern "C" float worldQueryLightMap__FP12_worldHeaderG6Point3(void*, Point3);
int main() {
    int failures = 0;
    static unsigned char world[64];
    void* worlds[] = {nullptr, world};
    Point3 pts[] = {{0, 0, 0}, {1, 2, 3}, {-1e30f, 5, 1e30f}};
    for (void* w : worlds)
        for (const Point3& p : pts) {
            float got = worldQueryLightMap__FP12_worldHeaderG6Point3(w, p);
            if (got != 1.0f) { std::fprintf(stderr, "fail: got %f\n", got); ++failures; }
        }
    if (failures) return 1;
    std::printf("worldQueryLightMap contract passed\n");
    return 0;
}
