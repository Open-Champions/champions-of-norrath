// Host check only; not a build input.
#define NORRATH_HOST_ARENA 1
static unsigned char arena[0x4000];
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + address; }
#include "memory_segm_write.cpp"
static void segment(int i, long long length, unsigned base) {
    *reinterpret_cast<long long*>(arena + 0x100 + 24 * i + 8) = length;
    *reinterpret_cast<unsigned*>(arena + 0x100 + 24 * i + 16) = base;
}
int main() {
    alignas(8) unsigned char f[96] = {};
    unsigned char in[16];
    for (int i = 0; i < 16; ++i) in[i] = static_cast<unsigned char>(i + 1);
    segment(0, 3, 0x1000);
    segment(1, 2, 0x2000);
    segment(2, 4, 0x3000);
    *reinterpret_cast<int*>(f + 52) = 2;
    *reinterpret_cast<unsigned*>(f + 56) = 0x100;
    *reinterpret_cast<long long*>(f + 32) = 9;
    *reinterpret_cast<long long*>(f + 40) = 2;
    *reinterpret_cast<int*>(f + 8) = 7;
    if (MemorySegmWrite__6VIFilePCvi(f, in, 4) != 0 || arena[0x1000] != 1 || arena[0x1002] != 3 || arena[0x2000] != 4) return 1;
    if (*reinterpret_cast<long long*>(f + 16) != 4 || *reinterpret_cast<long long*>(f + 40) != 4 || *reinterpret_cast<int*>(f + 8) != 0) return 2;
    if (*reinterpret_cast<int*>(f + 60) != 1 || *reinterpret_cast<long long*>(f + 64) != 1) return 3;
    if (MemorySegmWrite__6VIFilePCvi(f, in, 10) != -1 || arena[0x2001] != 1 || arena[0x3000] != 2 || arena[0x3003] != 5 || arena[0x3004] != 0) return 4;
    if (*reinterpret_cast<long long*>(f + 16) != 9 || *reinterpret_cast<long long*>(f + 40) != 9) return 5;
    if (MemorySegmWrite__6VIFilePCvi(f, in, 1) != -1 || *reinterpret_cast<long long*>(f + 16) != 9) return 6;
    return 0;
}
