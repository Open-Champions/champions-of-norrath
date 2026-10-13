// Host contract for blockFree__FPv. Not an EE plan input. The pool starts are set to host
// addresses in ascending order; the word2 field is pointer-sized in the source so this works.
#include <cstdint>
#include "block_free.cpp"

BlockRegionRow norrath_block_region_view[4];

int main() {
    alignas(16) static unsigned char pool[0x3000];
    unsigned char* p1 = pool + 0x1000;
    unsigned char* p2 = pool + 0x2000;
    BlockRegionRow* t = norrath_block_region_view;
    t[0] = {0x180, 0x200, 0, nullptr};
    t[1] = {0x500, 0x200, (uintptr_t)p1, nullptr};
    t[2] = {0xbb8, 0x20, (uintptr_t)p2, nullptr};
    t[3] = {0x7fffffff, 0, ~(uintptr_t)0, nullptr};
    void* a = pool + 0x100;
    blockFree__FPv(a);
    if (t[0].head != a || *(void**)a != nullptr) return 1;
    void* b = pool + 0x200;
    blockFree__FPv(b);
    if (t[0].head != b || *(void**)b != a) return 2;
    void* c = p1 + 0x10;
    blockFree__FPv(c);
    if (t[1].head != c || *(void**)c != nullptr || t[0].head != b) return 3;
    void* d = p2;      // exactly at the start of class 2: not below, so class 2
    blockFree__FPv(d);
    if (t[2].head != d || t[1].head != c) return 4;
    return 0;
}
