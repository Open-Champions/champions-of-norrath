// Host contract for objectAddToGrid; not an EE input.
#include "object_add_to_grid.cpp"
#include <cstdio>
#include <cstring>
unsigned char objectGrid[57600], ghostObjectGrid[57600], offTheGrid[4];
static void* seenHead; static void* seenLink; static void* seenData; static int calls;
void listOnewayLink__FP8ListHeadT0Pv(void* h, void* l, void* d) { seenHead = h; seenLink = l; seenData = d; ++calls; }
int main() {
    int bad = 0;
    struct Case { float x, y; unsigned flags; int kind, gx, gy; };
    const Case cases[] = {
        {0.0f, 0.0f, 0, 0, 60, 60}, {-15360.0f, -15360.0f, 0, 0, 0, 0}, {15359.9f, 15359.9f, 0, 0, 119, 119},
        {-15360.5f, 0, 0, 0, 0, 60}, {15360.0f, 0, 0, 2, 0, 0}, {0, -15361.0f, 0, 2, 0, 0}, {0, 20000.0f, 0, 2, 0, 0},
        {300.7f, -300.7f, 0x800, 1, 61, 58}, {300.7f, -300.7f, 0x2, 2, 0, 0}, {300.7f, -300.7f, 0x802, 2, 0, 0},
        {255.0f, 256.0f, 0x800, 1, 60, 61}, {-1.0f, -255.9f, 0, 0, 59, 59}, {-256.5f, 5.0f, 0x801, 1, 59, 60},
    };
    for (const Case& c : cases) {
        alignas(8) unsigned char object[64] = {};
        std::memcpy(object + 16, &c.x, 4); std::memcpy(object + 20, &c.y, 4); std::memcpy(object + 12, &c.flags, 4);
        calls = 0;
        objectAddToGrid__FP10GameObject(object);
        unsigned char* want = c.kind == 2 ? offTheGrid : (c.kind == 1 ? ghostObjectGrid : objectGrid) + (c.gy * 120 + c.gx) * 4;
        if (calls != 1 || seenHead != want || seenLink != object + 8 || seenData != object) { std::printf("fail: case x=%g y=%g flags=%x\n", c.x, c.y, c.flags); bad = 1; }
    }
    if (!bad) std::puts("objectAddToGrid ok");
    return bad;
}
