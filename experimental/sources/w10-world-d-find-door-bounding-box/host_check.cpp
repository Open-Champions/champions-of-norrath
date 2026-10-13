// Host contract for worldFindDoorBoundingBox__FP12_worldHeaderiP6Point3T2. Not an EE plan input.
#include <cstdio>
#include <cstring>
struct _worldHeader;
struct Point3 { float x, y, z; };
extern "C" void worldFindDoorBoundingBox__FP12_worldHeaderiP6Point3T2(_worldHeader*, int, Point3*, Point3*);
alignas(8) static unsigned char world[128];
alignas(8) static unsigned char doors[3 * 28];
alignas(8) static unsigned char recs[4 * 60];
static void rec(int i, float a, float b, float c, float d, float e, float f) {
    float* p = reinterpret_cast<float*>(recs + 60 * i);
    p[2] = a; p[3] = b; p[4] = c; p[5] = d; p[6] = e; p[7] = f;
}
int main() {
    *reinterpret_cast<unsigned char**>(world + 84) = doors;
    *reinterpret_cast<unsigned char**>(world + 36) = recs;
    // door 1: records 1..2 (count 2); door 2: no records.
    *reinterpret_cast<int*>(doors + 28) = 1;
    *reinterpret_cast<short*>(doors + 28 + 4) = 2;
    *reinterpret_cast<short*>(doors + 56 + 4) = 0;
    rec(1, 100, 200, 300, 400, 500, 600);
    rec(2, 150, 150, 350, 450, 450, 700);
    Point3 lo = {9, 9, 9}, hi = {9, 9, 9};
    auto w = reinterpret_cast<_worldHeader*>(world);
    worldFindDoorBoundingBox__FP12_worldHeaderiP6Point3T2(w, 1, &lo, &hi);
    if (lo.x != 148 || lo.y != 198 || lo.z != 348) return 1;     // per-axis minimum plus 48
    if (hi.x != 402 || hi.y != 452 || hi.z != 652) return 2;     // max - 48
    worldFindDoorBoundingBox__FP12_worldHeaderiP6Point3T2(w, 2, &lo, &hi);
    if (lo.x != 1000048.0f || hi.x != -1000048.0f) return 3;
    worldFindDoorBoundingBox__FP12_worldHeaderiP6Point3T2(w, -1, &lo, &hi);
    if (lo.x != 0 || lo.y != 0 || lo.z != 0 || hi.x != 0 || hi.y != 0 || hi.z != 0) return 4;
    std::printf("world find door bounding box contract passed\n");
    return 0;
}
