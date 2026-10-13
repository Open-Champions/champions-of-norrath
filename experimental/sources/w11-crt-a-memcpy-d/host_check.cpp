// Host contract for MemcpyD. Not an EE plan input. memcpy is a recording stub.
#include <cstdio>

#include "memcpy_d.cpp"
static int calls; static void* last_dst; static const void* last_src; static __SIZE_TYPE__ last_n;
extern "C" void* memcpy(void* d, const void* s, __SIZE_TYPE__ n) { ++calls; last_dst = d; last_src = s; last_n = n; return d; }
int main() {
    alignas(8) unsigned char obj[64] = {0}; unsigned char buf[128]; unsigned char src[64];
    *reinterpret_cast<int*>(obj + 48) = 100;
    // room = 100 - 4*5 = 80
    if (MemcpyD(obj, buf, 5, src, 80) != 0 || calls != 1 || last_dst != buf + 20 || last_src != src || last_n != 80) return 1;
    if (MemcpyD(obj, buf, 5, src, 81) != -543 || calls != 1) return 2;
    if (MemcpyD(obj, buf, 5, src, -1) != -543 || calls != 1) return 3;
    if (MemcpyD(obj, buf, 5, 0, 5000) != 0 || calls != 1) return 4;          // null source wins
    if (MemcpyD(obj, buf, 5, 0, -5) != 0 || calls != 1) return 5;
    if (MemcpyD(obj, buf, 1, src, 0) != 0 || calls != 2 || last_n != 0 || last_dst != buf + 4) return 6;
    if (MemcpyD(obj, buf, 30, src, 0) != -543 || calls != 2) return 7;        // room is -20: signed compare
    if (MemcpyD(obj, buf, 25, src, 0) != 0 || calls != 3) return 8;           // room exactly 0
    std::printf("memcpy_d cases passed\n");
    return 0;
}
