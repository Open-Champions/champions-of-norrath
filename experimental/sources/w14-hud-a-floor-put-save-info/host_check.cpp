#include <cstdio>
#include <cstring>
struct FloorPoint;
static int clears = 0, adds = 0;
struct Seen { float x, y, z; short a; float b; int c, d; } seen[4];
extern "C" void floorClear__Fv() { clears++; }
extern "C" void floorAdd__FG6Point3sfii(const FloorPoint* p, short a, float b, int c, int d);
#include "floor_put_save_info.cpp"
extern "C" void floorAdd__FG6Point3sfii(const FloorPoint* p, short a, float b, int c, int d) {
    if (adds < 4) { std::memcpy(&seen[adds], p, 12); seen[adds].a = a; seen[adds].b = b; seen[adds].c = c; seen[adds].d = d; }
    adds++;
}

int main() {
    int bad = 0;
    alignas(4) char buf[8 + 12 * 2] = {};
    int count = 2;
    std::memcpy(buf, &count, 4);
    short r0[5] = {40, -8, 100, 6, 77};
    short r1[5] = {1, 2, 3, 4, -5};
    std::memcpy(buf + 8, r0, 10); buf[18] = -3;
    std::memcpy(buf + 20, r1, 10); buf[30] = 9;
    if (floorPutSaveInfo__FPc(buf) != 32) bad++;
    if (clears != 1 || adds != 2) bad++;
    if (seen[0].x != 10.0f || seen[0].y != -2.0f || seen[0].z != 22.0f || seen[0].a != 77 || seen[0].b != 1.5f ||
        seen[0].c != -3 || seen[0].d != 0) bad++;
    if (seen[1].x != 0.25f || seen[1].z != 0.75f - 3.0f || seen[1].a != -5 || seen[1].b != 1.0f || seen[1].c != 9) bad++;
    count = 0; std::memcpy(buf, &count, 4); adds = 0;
    if (floorPutSaveInfo__FPc(buf) != 8 || adds != 0) bad++;
    count = -4; std::memcpy(buf, &count, 4);
    if (floorPutSaveInfo__FPc(buf) != 8 || adds != 0) bad++;
    if (!bad) std::puts("floorPutSaveInfo passed");
    return bad;
}
