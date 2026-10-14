#define NORRATH_HOST_ARENA 1
#include "cull_by_top.cpp"
#include <string.h>

static unsigned char arena[0x4000];
extern "C" unsigned char* norrath_host_pointer(unsigned a) { return arena + a; }

int main() {
    unsigned char* stack = arena;                 // clip stack at 0
    unsigned char* recs = arena + 0x100;          // records
    unsigned char* planes = arena + 0x400;        // planes
    *(int*)stack = 2; *(unsigned*)(stack + 8) = 0x100; *(unsigned*)(stack + 32) = 0x400;
    // record 0 is a decoy; record 1 (the top) has 2 planes starting at plane 3
    *(int*)(recs + 0) = 5; *(int*)(recs + 4) = 0;
    *(int*)(recs + 16) = 2; *(int*)(recs + 20) = 3;
    float box[6] = {-1, -2, -3, 4, 5, 6};
    auto plane = [&](int i, float a, float b, float c, float d) { float p[4] = {a, b, c, d}; memcpy(planes + 16 * i, p, 16); };
    // decoy planes that would cull
    for (int i = 0; i < 3; ++i) plane(i, 0, 0, 1, 100);
    // plane 3: normal (1,0,0), d = -4.5: min corner x = -1 -> -1 - 4.5 < 0, keep
    plane(3, 1, 0, 0, -4.5f); plane(4, 0, -1, 0, -4.5f);
    // plane 4: y uses max (5) because b < 0: -5 - 4.5 < 0, keep
    if (CullByTop__11VIClipStackRC6VIBBox(stack, box) != 0) return 1;
    plane(4, 0, -1, 0, 5.5f);                      // -5 + 5.5 > 0 -> culled
    if (CullByTop__11VIClipStackRC6VIBBox(stack, box) != 1) return 2;
    plane(4, 0, -1, 0, 5.0f);                      // exactly 0 -> not culled
    if (CullByTop__11VIClipStackRC6VIBBox(stack, box) != 0) return 3;
    plane(3, 1, 0, 0, 1.5f);                       // -1 + 1.5 > 0 (min corner for a >= 0) -> culled by the first plane
    if (CullByTop__11VIClipStackRC6VIBBox(stack, box) != 1) return 4;
    plane(3, 1, 0, 0, 0.5f);                       // -1 + 0.5 < 0
    plane(4, 0, 0, 0, -1.0f);                      // zero normal component picks the min corner; value -1
    if (CullByTop__11VIClipStackRC6VIBBox(stack, box) != 0) return 5;
    // a = -0.0 still counts as >= 0 (picks the min corner): 0 + (-2 * 2) + (-3 * 1) + 7 = 0 -> not culled
    plane(4, -0.0f, 2, 1, 7.0f);
    if (CullByTop__11VIClipStackRC6VIBBox(stack, box) != 0) return 6;
    plane(4, -0.0f, 2, 1, 7.5f);
    if (CullByTop__11VIClipStackRC6VIBBox(stack, box) != 1) return 7;
    *(int*)(recs + 16) = 0;                        // no planes
    if (CullByTop__11VIClipStackRC6VIBBox(stack, box) != 0) return 8;
    *(int*)(recs + 16) = 1; *(int*)(recs + 20) = 4; plane(4, 0, 0, 1, 3.5f);   // z = -3 (c >= 0) -> 0.5 > 0
    if (CullByTop__11VIClipStackRC6VIBBox(stack, box) != 1) return 9;
    plane(4, 0, 0, -1, 3.5f);                      // z = 6 (c < 0) -> -6 + 3.5 < 0
    if (CullByTop__11VIClipStackRC6VIBBox(stack, box) != 0) return 10;
    return 0;
}
