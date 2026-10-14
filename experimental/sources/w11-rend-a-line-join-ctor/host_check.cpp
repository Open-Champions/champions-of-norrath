#include <cstdio>
#include <cstring>
#include "../level-anim-load-common/host_arena.hpp"
#include "../level-anim-load-common/host_check.hpp"
extern "C" {
static int allocBytes, initCalls;
static char allocLabel[32];
static unsigned char* initArg;
void* memAlloc__FiPc(int bytes, char* label) {
    allocBytes = bytes; std::strncpy(allocLabel, label, 31);
    return arena_at(0x10000);
}
void initializeMe__4Line(void* line) { ++initCalls; initArg = static_cast<unsigned char*>(line); }
void __4LineP4LineT1i(unsigned char* self, unsigned char* a, unsigned char* b, int k);
}
static void point(unsigned char* base, int i, float x, float y, float z) {
    sf_(base + i * 64, x); sf_(base + i * 64 + 4, y); sf_(base + i * 64 + 8, z);
}
int main() {
    unsigned char* self = arena_at(0x100);
    unsigned char* a = arena_at(0x200);
    unsigned char* b = arena_at(0x300);
    unsigned char* pa = arena_at(0x4000);
    unsigned char* pb = arena_at(0x6000);
    unsigned char* out = arena_at(0x10000);
    std::memset(self, 0xcd, 64);
    sw_(a, 0x1111); sw_(a + 4, 3); sw_(a + 8, 5); sw_(a + 12, 0x77); sw_(a + 20, norrath_host_address(pa));
    sw_(b, 0x2222); sw_(b + 4, 4); sw_(b + 8, 6); sw_(b + 12, 0x88); sw_(b + 20, norrath_host_address(pb));
    for (int i = 0; i != 3; ++i) point(pa, i, 10 + i, 20 + i, 30 + i);
    for (int i = 0; i != 4; ++i) point(pb, i, 100 + i, 200 + i, 300 + i);
    std::memset(out, 0xee, 64 * 8);
    // k = 0: count = 3 + 4 - 1 = 6; two points from a, then four from b.
    __4LineP4LineT1i(self, a, b, 0);
    if (hw(self) != 0x1111 || hw(self + 4) != 6 || hw(self + 8) != 5 || hw(self + 12) != 0x77) return 1;
    if (hw(self + 20) != norrath_host_address(out) || allocBytes != 6 * 64 || std::strcmp(allocLabel, "point list") != 0) return 2;
    if (initCalls != 1 || initArg != self) return 3;
    if (lf_(out) != 10 || lf_(out + 64 + 4) != 21 || lf_(out + 2 * 64) != 100 || lf_(out + 5 * 64 + 8) != 303) return 4;
    // Index words: the second loop writes e = a[4]-1+j into point j, over the indices of points
    // 0 and 1 and leaving later ones unwritten (still the 0xee fill).
    if (hw(out + 60) != 2 || hw(out + 64 + 60) != 3 || hw(out + 2 * 64 + 60) != 4 || hw(out + 3 * 64 + 60) != 5) return 5;
    if (hw(out + 4 * 64 + 60) != 0xeeeeeeee) return 6;
    // k = 1: word 8 becomes 1, count 5, the last point of b is dropped.
    std::memset(self, 0xcd, 64); std::memset(out, 0xee, 64 * 8); initCalls = 0;
    __4LineP4LineT1i(self, a, b, 1);
    if (hw(self + 8) != 1 || hw(self + 4) != 5 || allocBytes != 5 * 64) return 7;
    if (lf_(out + 2 * 64) != 100 || lf_(out + 4 * 64) != 102 || hw(out + 5 * 64) != 0xeeeeeeee) return 8;
    if (initCalls != 1) return 9;
    std::puts("Line join constructor contract passed");
    return 0;
}
