// Host contract for floorAdd; not an EE input.
#include "floor_add.cpp"
#include <cstdio>
#include <cstring>
int g_BloodLevel;
int spriteCount;
FloorBlock* freeList;
FloorBlock* sprGrid[900];
ExpandRecord expandList[16];
static int lightPoints;
int lightFloorSprite__FG6Point3(const float*) { ++lightPoints; return 0x030201; }
float icos__Fi(int a) { return a == 5 ? 0.5f : 1.0f; }
float isin__Fi(int a) { return a == 5 ? -0.25f : 0.0f; }
static FloorBlock a, b;
static int bad;
#define CHECK(c) do { if (!(c)) { std::printf("fail line %d: %s\n", __LINE__, #c); bad = 1; } } while (0)
int main() {
    float p[3] = {10.0f, -20.0f, 5.0f};
    // blood off: kind 0 ignored, kinds 1 and 2 ignored
    freeList = &a;
    floorAdd__FG6Point3sfii(p, 5, 2.0f, 0, 1);
    floorAdd__FG6Point3sfii(p, 5, 2.0f, 1, 1);
    floorAdd__FG6Point3sfii(p, 5, 2.0f, 2, 1);
    CHECK(spriteCount == 0 && freeList == &a);
    // kind 3 takes a block from the free list: cell (15, 14)
    std::memset(&a, 0, sizeof a); a.next = &b;
    a.zMin = 100; a.zMax = -100;
    floorAdd__FG6Point3sfii(p, 5, 2.0f, 3, 7);
    FloorBlock* cell = sprGrid[14 * 30 + 15];
    CHECK(cell == &a && freeList == &b && a.next == 0);
    CHECK(a.count == 1 && spriteCount == 1 && lightPoints == 1);
    FloorSprite& s = a.sprites[0];
    CHECK(s.colour[0] == 1 && s.colour[1] == 2 && s.colour[2] == 3 && s.fixed == 128);
    CHECK(s.x4 == 40 && s.y4 == -80 && s.z4 == 32 && s.small == 0);
    CHECK(s.dx == 4 && s.dy == -2 && s.texture == 979 + 6);
    CHECK(a.smallCount == 0 && a.zMin == 5 && a.zMax == 5);
    CHECK(expandList[0].sprite == &s.dx && expandList[0].flag == 7 && expandList[0].angle == 5 && expandList[0].scale == 2.0f);
    // blood on, kind 0: small flag set, z range extends, second expand slot
    g_BloodLevel = 1;
    p[2] = 9.5f;
    floorAdd__FG6Point3sfii(p, 5, 1.0f, 0, 1);
    CHECK(a.count == 2 && a.smallCount == 1 && a.sprites[1].small == 1 && a.sprites[1].texture == 979);
    CHECK(a.zMin == 5 && a.zMax == 9 && expandList[1].sprite == &a.sprites[1].dx);
    p[2] = -4.0f;
    floorAdd__FG6Point3sfii(p, 5, 1.0f, 0, 0);
    CHECK(a.zMin == -4 && a.count == 3 && expandList[2].sprite == 0);
    // out of range cells and full blocks
    float q[3] = {-3841.0f, 0, 0};
    int before = spriteCount;
    floorAdd__FG6Point3sfii(q, 0, 1.0f, 3, 0);
    q[0] = 3840.0f; floorAdd__FG6Point3sfii(q, 0, 1.0f, 3, 0);
    q[0] = 0; q[1] = 3840.0f; floorAdd__FG6Point3sfii(q, 0, 1.0f, 3, 0);
    q[1] = -3841.0f; floorAdd__FG6Point3sfii(q, 0, 1.0f, 3, 0);
    CHECK(spriteCount == before);
    a.count = 31;
    floorAdd__FG6Point3sfii(p, 5, 1.0f, 3, 0);
    CHECK(a.count == 31 && spriteCount == before);
    a.count = 30;
    floorAdd__FG6Point3sfii(p, 5, 1.0f, 3, 0);
    CHECK(a.count == 31 && spriteCount == before + 1);
    // empty free list and full expand list
    float r[3] = {-3000.0f, -3000.0f, 0};
    freeList = 0;
    floorAdd__FG6Point3sfii(r, 5, 1.0f, 3, 0);
    CHECK(spriteCount == before + 1);
    for (int i = 0; i < 16; ++i) expandList[i].sprite = reinterpret_cast<short*>(&expandList[i]);
    a.count = 0;
    floorAdd__FG6Point3sfii(p, 5, 1.0f, 3, 3);
    CHECK(a.count == 1 && expandList[15].flag == 0 && expandList[15].scale == 0.0f && expandList[15].sprite == reinterpret_cast<short*>(&expandList[15]));
    if (!bad) std::puts("floorAdd ok");
    return bad;
}
